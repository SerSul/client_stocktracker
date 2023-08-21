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
    void enableAllButtons();
    void disableOtherButtons(QPushButton* activeButton);
private:
    Ui::clientClass* ui;
    QTcpSocket* socket;
    QString address;
    QPushButton* activeButton;
private slots:
    void AddProduct();
    void deleteData();
    void updateData();
    void getDataById();
    void getAllData();
    void sendRequest(const QString& method, const QString& url, const QJsonObject& data = QJsonObject());
    void handleResponse(QNetworkReply* reply);
    void rememberAdress();
    void sendData();
};

