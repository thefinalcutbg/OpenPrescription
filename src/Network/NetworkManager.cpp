﻿#include "NetworkManager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslKey>
#include "PKCS11.h"
#include "AbstractReplyHandler.h"
#include "Network/HIS/HisToken.h"
#include <set>
#include "View/ModalDialogBuilder.h"
#include <QApplication>

QNetworkAccessManager* getManager() {

    static QNetworkAccessManager* s_manager{ nullptr };

    if (!s_manager) {
        s_manager = new QNetworkAccessManager();
        s_manager->setTransferTimeout(15000);
        // s_manager->setAutoDeleteReplies(true); //produces crashes sometimes lol
        QObject::connect(s_manager, &QNetworkAccessManager::sslErrors, [=] {
            qDebug() << "ERRORRRR";
            });
    }

    return s_manager;
}

std::set<AbstractReplyHandler*> handlers;


void NetworkManager::sendRequestToPis(
                                const std::string& soapRequest,
                                PKCS11& token,
                                AbstractReplyHandler* handler,
                                const char* soapHeader
                              )
{

    auto manager = getManager();

    handlers.insert(handler);

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    //config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::SslProtocol::TlsV1_2);
    config.setLocalCertificate(QSslCertificate(token.pem_x509cert().data()));
    config.setPrivateKey(QSslKey(Qt::HANDLE(token.takePrivateKey()), QSsl::KeyType::PrivateKey));

    QNetworkRequest request(QUrl("https://pis.nhif.bg/ws/PISService"));
    request.setSslConfiguration(config);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "text/xml;charset=\"utf-8\"");
    request.setRawHeader("SOAPAction", soapHeader);
    request.setRawHeader("accept", "\"application/xml\"");

    auto reply = manager->post(request, soapRequest.data());

    

    QApplication::setOverrideCursor(Qt::BusyCursor);

    QObject::connect(reply, &QNetworkReply::errorOccurred,
        [=](QNetworkReply::NetworkError code)
        {
            qDebug() << "ReplyError: " << code;
        }
    );

    QObject::connect(reply, &QNetworkReply::finished, 
        [=] {
            
            QApplication::restoreOverrideCursor();

            if (handlers.count(handler) == 0) return;

            std::string replyString = reply->readAll().toStdString();

            //the html error reply from PIS begins with <!DOCTYPE ...
            //otherwise it begins with <?xml ....
            if (replyString[1] == '!') {
                replyString.clear(); 
            }
            
            handler->getReply(replyString);

            reply->deleteLater();
            
        });

    QObject::connect(reply, &QNetworkReply::sslErrors, [=] {

        QApplication::restoreOverrideCursor();

       // ModalDialogBuilder::showError("Неуспешна автентификация");

        handler->getReply("");

        reply->deleteLater();

    });


    
}
void NetworkManager::sendRequestToHis(
                    AbstractReplyHandler* handler,
                    const std::string& nhifMessage,
                    const std::string& token,
                    const std::string& urlAndServicePath
)
{
    auto manager = getManager();

    QApplication::setOverrideCursor(Qt::BusyCursor);

    handlers.insert(handler);

    QUrl url(urlAndServicePath.c_str());

    QString authValue = "Bearer ";
    authValue.append(token.c_str());

    QSslConfiguration config(QSslConfiguration::defaultConfiguration());
    config.setProtocol(QSsl::SslProtocol::TlsV1_3);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/xml;charset=\"utf-8\"");
    request.setRawHeader("accept", "\"application/xml\"");
    request.setRawHeader("Connection", "Keep-Alive");
    request.setRawHeader("Authorization",  authValue.toUtf8());

    QNetworkReply* reply = manager->post(request, nhifMessage.data());

    QObject::connect(reply, &QNetworkReply::finished,
        [=]() {

            QApplication::restoreOverrideCursor();

            if (handlers.count(handler) == 0) return;

            handler->getReply(reply->readAll().toStdString());

            reply->deleteLater();


        });
}
#include <QDebug>
void NetworkManager::sendRequestToHisNoAuth(AbstractReplyHandler* handler, const std::string& nhifMessage, const std::string& urlAndServicePath)
{

    auto manager = getManager();

    QApplication::setOverrideCursor(Qt::BusyCursor);

    handlers.insert(handler);

    QUrl url(urlAndServicePath.c_str());

    QSslConfiguration config(QSslConfiguration::defaultConfiguration());
    config.setProtocol(QSsl::SslProtocol::TlsV1_3);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/xml;charset=\"utf-8\"");
    request.setRawHeader("accept", "\"application/xml\"");
    request.setRawHeader("Connection", "Keep-Alive");

    QNetworkReply* reply = manager->post(request, nhifMessage.data());

    QObject::connect(reply, &QNetworkReply::finished,
        [=]() {
            
            QApplication::restoreOverrideCursor();
            
            if (handlers.count(handler) == 0) return;
   
            handler->getReply(reply->readAll().toStdString());

            reply->deleteLater();


        });
}

void NetworkManager::requestChallenge()
{

    auto manager = getManager();

    QUrl url("https://auth.his.bg/token");

    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);

    QApplication::setOverrideCursor(Qt::BusyCursor);

    QObject::connect(reply, &QNetworkReply::finished,
        [=] {

            QApplication::restoreOverrideCursor();

            HisToken::setChallengeMessage(reply->readAll().toStdString());

            reply->deleteLater();

        });
}

void NetworkManager::requestToken(const std::string& signedChallenge)
{
    auto manager = getManager();

    QUrl url("https://auth.his.bg/token");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/xml;charset=\"utf-8\"");
    request.setRawHeader("accept", "\"application/xml\"");
    request.setRawHeader("Connection", "Keep-Alive");

    QNetworkReply* reply = manager->post(request, signedChallenge.c_str());
    
    QObject::connect(reply, &QNetworkReply::finished,
        [=] {

            QApplication::restoreOverrideCursor();

            HisToken::setAuthRepy(reply->readAll().toStdString());

            reply->deleteLater();

        });
}


void NetworkManager::clearAccessCache()
{
    getManager()->clearAccessCache();
}

void NetworkManager::unsubscribeHandler(AbstractReplyHandler* handler)
{
    if (handlers.count(handler)) {
        handlers.erase(handler);
    }
}


QNetworkReply* NetworkManager::simpleRequest(const char* url)
{
    QNetworkRequest request(QUrl{url});
    
    return getManager()->get(request);
}
