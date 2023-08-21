#include "Network.h"


void Network::handleResponse(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject.contains("status") && jsonObject["status"].isString() && !jsonObject.contains("data")) {
                QString status = jsonObject["status"].toString();
                QString message = jsonObject["message"].toString();
                QString responseText = status + ": " + message;

                // Испускание сигнала с текстом ответа
                emit responseReceived(responseText);
            }
            else if (jsonObject.contains("data") && jsonObject["data"].isArray()) {
                QJsonArray dataArray = jsonObject["data"].toArray();
                QString parsedData;

                for (const QJsonValue& value : dataArray) {
                    if (value.isObject()) {
                        QJsonObject productObject = value.toObject();
                        int id = productObject["id"].toInt();
                        QString name = productObject["name"].toString();
                        QString price = productObject["price"].toString();
                        int quantity = productObject["quantity"].toInt();

                        parsedData += QString("ID: %1\nName: %2\nPrice: %3\nQuantity: %4\n\n")
                            .arg(id)
                            .arg(name)
                            .arg(price)
                            .arg(quantity);
                    }
                }

                // Испускание сигнала с текстом ответа
                emit responseReceived(parsedData);
            }
            else {
                QString responseText = "Invalid response format";

                // Испускание сигнала с текстом ответа
                emit responseReceived(responseText);
            }
        }
    }
}



void Network::sendRequest(const QString& method, const QString& url, const QJsonObject& data)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument jsonDoc(data);
    QByteArray jsonData = jsonDoc.toJson();

    QNetworkReply* reply = nullptr;

    if (method == "PUT") {
        reply = manager->put(request, jsonData);
    }
    else if (method == "DELETE") {
        reply = manager->deleteResource(request);
    }
    else if (method == "POST") {
        reply = manager->post(request, jsonData);
    }
    else if (method == "GET") {
        reply = manager->get(request);
    }

    if (reply) {
        connect(reply, &QNetworkReply::finished, this, [=]() {
            handleResponse(reply);
            reply->deleteLater();
            manager->deleteLater();
            });
    }
}

