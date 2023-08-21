#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_client.h"
#include "Network.h"
#include <QMessageBox>

#include <QDebug>
#include <QString>
#include <QByteArray>

#include <QTextBrowser>

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
    Network network;

private slots:
    void AddProduct();
    void deleteData();
    void updateData();
    void getDataById();
    void getAllData();
    void rememberAdress();
    void sendData();

    void handleNetworkResponse(const QString& response);
};

