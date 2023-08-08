#include "client.h"

client::client(QWidget* parent) : QMainWindow(parent)
{
    ui = new Ui::clientClass;
    ui->setupUi(this);

    connect(ui->B1, &QPushButton::clicked, this, &client::sendData);
    connect(ui->B2, &QPushButton::clicked, this, &client::deleteData);
    connect(ui->B3, &QPushButton::clicked, this, &client::updateData);
    connect(ui->B4, &QPushButton::clicked, this, &client::getDataById);
    connect(ui->B5, &QPushButton::clicked, this, &client::getAllData);
    connect(ui->B6, &QPushButton::clicked, this, &client::close);
}

client::~client()
{
}

void client::sendData()
{
    QString id = ui->idLINE->text();
    QString name = ui->nameLINE->text();
    QString price = ui->priceLINE->text();
    QString quantity = ui->quantityLINE->text();

    QJsonObject json;
    json["id"] = id;

    if (!name.isEmpty()) {
        json["name"] = name;
    }
    else {
        json["name"] = QJsonValue::Null;
    }

    if (!price.isEmpty()) {
        json["price"] = price;
    }
    else {
        json["price"] = QJsonValue::Null;
    }

    if (!quantity.isEmpty()) {
        json["quantity"] = quantity;
    }
    else {
        json["quantity"] = QJsonValue::Null;
    }

    sendRequest("PUT", QString("http://192.168.1.4:5000/products/add"), json);
}

void client::deleteData()
{
    QString id = ui->idLINE->text();
    sendRequest("DELETE", QString("http://192.168.1.4:5000/products/%1").arg(id));
}

void client::updateData()
{
    QString id = ui->idLINE->text();
    QString name = ui->nameLINE->text();
    QString price = ui->priceLINE->text();
    QString quantity = ui->quantityLINE->text();

    QJsonObject json;
    json["id"] = id;


    if (!name.isEmpty()) {
        json["name"] = name;
    }


    if (!price.isEmpty()) {
        json["price"] = price;
    }

    if (!quantity.isEmpty()) {
        json["quantity"] = quantity;
    }
    sendRequest("PUT", QString("http://192.168.1.4:5000/products/%1").arg(id), json);
}

void client::getDataById()
{
    QString id = ui->idLINE->text();
    sendRequest("GET", QString("http://192.168.1.4:5000/products/%1").arg(id));
}

void client::getAllData()
{
    sendRequest("GET", "http://192.168.1.4:5000/products");
}


void client::sendRequest(const QString& method, const QString& url, const QJsonObject& data)
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


void client::handleResponse(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject.contains("status") && jsonObject["status"].isString() && !jsonObject.contains("data")) {
                QString status = jsonObject["status"].toString();
                QString message = jsonObject["message"].toString();
                ui->responseTXT->setText(status + ": " + message);
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

                ui->responseTXT->setText(parsedData);
            }
            else {
                ui->responseTXT->setText("Invalid response format");
            }
        }
    }

}
