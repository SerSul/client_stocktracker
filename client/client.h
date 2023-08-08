#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_client.h"

#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextBrowser>
#include <QtNetwork>
#include <iostream>

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget* parent = nullptr);
    ~client();

private:
    Ui::clientClass* ui;
    QTcpSocket* socket;

private slots:
    void sendData();
    void deleteData();
    void updateData();
    void getDataById();
    void getAllData();
    void sendRequest(const QString& method, const QString& url, const QJsonObject& data = QJsonObject());
    void handleResponse(QNetworkReply* reply);
};
