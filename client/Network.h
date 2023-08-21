#pragma once
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTcpSocket>
#include <QHostAddress>
class Network : public QObject {
	Q_OBJECT
public:
	void sendRequest(const QString& method, const QString& url, const QJsonObject& data = QJsonObject());
	void handleResponse(QNetworkReply* reply);

signals:
	void responseReceived(const QString& response);
};

