#include "HisService.h"
#include "Network/XmlSigner.h"
#include "Network/PKCS11.h"
#include "Network/NetworkManager.h"
#include "HisToken.h"
#include "View/ModalDialogBuilder.h"
#include "Model/User.h"
#include "View/ModalDialogBuilder.h"
#include "Model/FreeFunctions.h"
#include "Model/Patient.h"
#include <QDateTime>

constexpr const char* hisUrl = "https://api.his.bg/";
//constexpr const char* hisUrl = "https://ptest-api.his.bg/";

std::string timeNow() {
	auto t = QDateTime::currentDateTime();
	return t.toString(Qt::DateFormat::ISODate).toStdString();
}


bool HisService::sendRequestToHis(const std::string& query)
{
	if (awaiting_reply) return false;
	//ModalDialogBuilder::showMultilineDialog(buildMessage(query)); return true;

	if (HisToken::getToken().empty()) {
		return HisToken::requestToken(this, query);
	}

	auto signedMsg = signMessage(buildMessage(query));

	if (signedMsg.empty()) return false;

	awaiting_reply = true;

	NetworkManager::sendRequestToHis(
		  this, 
		  signedMsg, 
		  HisToken::getToken(), 
		  hisUrl + servicePath
	);

	return true;
	
}

bool HisService::sendRequestToHisNoAuth(const std::string& query)
{
	NetworkManager::sendRequestToHisNoAuth(this, buildMessage(query), hisUrl + servicePath);
	return true;
}

const std::string HisService::signMessage(const std::string& message)
{
	PKCS11 signer;

	if (!signer.hsmLoaded())
	{
		ModalDialogBuilder::showMessage("Не е открит КЕП");
		return {};
	}

	if (signer.loginRequired()) {

		NetworkManager::clearAccessCache();
		auto pin = ModalDialogBuilder::pinPromptDialog(signer.pem_x509cert());

		if (pin.empty()) {
			return {};
		}


		if (!signer.login(pin))
		{
			ModalDialogBuilder::showError("Грешна парола или блокирана карта");
			return {};
		};
	}

	return XmlSigner::signNhifMessage(message, signer.takePrivateKey(), signer.pem_x509cert());
}




const std::string HisService::buildMessage(const std::string& query)
{

	constexpr const char* softwareName = "OpenPrescription";

	std::string result;

	std::string senderValue =
		User::doctor().specialty.isDoctor() ? "1" : "2";

	result.reserve(4000);

	result +=

	"<nhis:message "
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
		"xmlns:nhis=\"https://www.his.bg\" "
		"xsi:schemaLocation=\"https://www.his.bg/api/v1/NHIS-" + messageType + ".xsd\">"
	
			"<nhis:header>"
				"<nhis:sender value=\""+senderValue+"\"/>"
				"<nhis:senderId value=\"" + User::doctor().LPK + "\"/>"
				"<nhis:senderISName value=\"" + softwareName + "\"/>"
				"<nhis:recipient value=\"4\"/>"
				"<nhis:recipientId value=\"NHIS\"/>"
				"<nhis:messageId value=\"" + FreeFn::getUuid() + "\"/>"
				"<nhis:messageType value=\"" + messageType + "\"/>"
				"<nhis:createdOn  value=\"" + timeNow() + "\"/>"
			"</nhis:header>"
		
			"<nhis:contents>"
				+query+
			"</nhis:contents>"
	"</nhis:message>"
	
	;

	return result;
	
}

std::string HisService::subject(const Patient& p)
{
	std::string middleNameTag = p.MiddleName.size()?
		"<nhis:middle value=\"" + p.MiddleName + "\"/>" : "";

	std::string subject =
	"<nhis:subject>"
		"<nhis:identifierType value=\"" + std::to_string(p.type) + "\"/>"
		"<nhis:identifier value=\"" + p.id + "\"/>"
		"<nhis:birthDate value=\"" + p.birth.to8601() + "\"/>"
		"<nhis:gender value=\"" + std::to_string(static_cast<int>(p.sex) + 1) + "\"/>"
		"<nhis:name>"
			"<nhis:given value=\"" + p.FirstName + "\"/>"
			+ middleNameTag + //because it is optional
			"<nhis:family value=\"" + p.LastName + "\"/>"
		"</nhis:name>"
		"<nhis:address>"
			"<nhis:country value=\"BG\"/>"
			//<!-- Optional: -->
			"<nhis:ekatte value=\"" + p.city.ekatte() + "\"/>"
			"<nhis:city value=\"" + p.city.getString() + "\"/>"
		"</nhis:address>"
		"<nhis:phone value=\""+p.phone+"\"/>"
		//<nhis:email value="[string]"/>
		"<nhis:various>"
			+bind("age", p.getAge())+
		"</nhis:various>"
	"</nhis:subject>"
	;

	return subject;
}

std::string HisService::requester(bool nhif)
{
	//cl008 numenclature
	auto lambda = []()->std::string{
		return FreeFn::leadZeroes(User::doctor().specialty.getIdx(), 4);
	};



	std::string qualification =
		"<nhis:qualification value=\"" + lambda() + "\"/>";

	std::string requester =
		"<nhis:requester>"
			"<nhis:pmi value=\"" + User::doctor().LPK + "\"/>"
			+ qualification +
			"<nhis:practiceNumber value=\"" + User::doctor().RZI + "\"/>"
			"<nhis:nhifNumber value=\"" + User::doctor().RHIF() + "\"/>"
			"<nhis:phone value=\"" + User::doctor().phone + "\"/>"
		"</nhis:requester>"
		;
	return requester;

}

std::string HisService::bind(const std::string& name, double value)
{
	return value ? bind(name, FreeFn::formatDouble(value)) : "";
}

std::string HisService::bind(const std::string& name, const char* value)
{
	if (value == "") return "";

	return "<nhis:" + name + " value=\"" + value + "\" />";
}

std::string HisService::bind(const std::string& name, std::string value)
{
	if (value.empty()) return "";

	return "<nhis:" + name + " value=\"" + value + "\" />";
}

std::string HisService::bind(const std::string& name, int value, bool ommitZero)
{
	if (!value && ommitZero) return {};

	return bind(name, std::to_string(value));
}

std::string HisService::bind(const std::string& name, bool value)
{
	return bind(name, value ? "true" : "false");
}

#include <TinyXML/tinyxml.h>

std::string HisService::getErrors(const std::string& reply)
{
	TiXmlDocument doc;

	doc.Parse(reply.data(), 0, TIXML_ENCODING_UTF8);

	TiXmlHandle docHandle(&doc);

	auto contentChild = docHandle.
			FirstChild(). //message
			Child(1).	  //contents
			FirstChildElement(). //possible error
			ToElement();

	while (contentChild)
	{
		if (contentChild->ValueStr() == "nhis:error") {

			return
				contentChild->
					FirstChild()->
						NextSiblingElement()->
								Attribute("value");
		}

		contentChild = contentChild->NextSiblingElement();
	}

	return std::string{};

}