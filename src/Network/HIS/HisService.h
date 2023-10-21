#pragma once
#include "Network/AbstractReplyHandler.h"

#include <string_view>

struct Patient;

class HisService : protected AbstractReplyHandler
{

private:

	std::string buildMessage(const std::string& query);
	std::string signMessage(const std::string& message);

	const std::string messageType;
	const std::string servicePath;

protected:

	bool getBool(TiXmlElement* parent, const std::string& tag);
	std::string getString(TiXmlElement* parent, const std::string& tag);
	int getInt(TiXmlElement* parent, const std::string& tag);
	double getDouble(TiXmlElement* parent, const std::string& tag);

	std::string subject(const Patient& p, bool isPregnant = false, bool isBreastfeeding = false);
	std::string requester(bool includeNhifCode = false);

	std::string bind(const std::string& name, const std::string& value, bool isUserInput = false);
	std::string bind(const std::string& name, int value, bool ommitZero = true);
	std::string bind(const std::string& name, bool value);
	std::string bind(const std::string& name, double value);
	std::string bind(const std::string& name, const char* value, bool isUserInput = false);

	std::string getErrors(const std::string& reply);

	HisService(const std::string& messageType, const std::string& servPath)
		:
		messageType{ messageType },
		servicePath{ servPath }
	{}
public:
	bool sendRequestToHis(const std::string& contents);
	bool sendRequestToHisNoAuth(const std::string& contents);

};

