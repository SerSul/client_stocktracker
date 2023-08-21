#include "client.h"

client::client(QWidget* parent) : QMainWindow(parent)
{
    connect(&network, SIGNAL(responseReceived(QString)), this, SLOT(handleNetworkResponse(QString)));
    ui = new Ui::clientClass;
    ui->setupUi(this);
    ui->idLINE->hide();
    ui->nameLINE->hide();
    ui->priceLINE->hide();
    ui->quantityLINE->hide();
    ui->IDlabel->hide();
    ui->Namelabel->hide();
    ui->Pricelabel->hide();
    ui->Quantitylabel->hide();
    connect(ui->B1, &QPushButton::clicked, this, &client::AddProduct);
    connect(ui->B2, &QPushButton::clicked, this, &client::deleteData);
    connect(ui->B3, &QPushButton::clicked, this, &client::updateData);
    connect(ui->B4, &QPushButton::clicked, this, &client::getDataById);
    connect(ui->B5, &QPushButton::clicked, this, &client::getAllData);
    connect(ui->B6, &QPushButton::clicked, this, &client::close);
    connect(ui->rememberBTN, &QPushButton::clicked, this, &client::rememberAdress);
    connect(ui->SendBTN, &QPushButton::clicked, this, &client::sendData);
}



void client::sendData()
{
    if (ui->B1->isChecked()) {
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

        network.sendRequest("PUT", address + QString("/products/add"), json);
    }

    if (ui->B2->isChecked()) {
        QString id = ui->idLINE->text();
        network.sendRequest("DELETE", address + QString("/products/%1").arg(id));
    }
    if (ui->B3->isChecked()) {
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
        network.sendRequest("PUT", address + QString("/products/%1").arg(id), json);
    }
    if (ui->B4->isChecked()) {
        QString id = ui->idLINE->text();
        network.sendRequest("GET", address + QString("/products/%1").arg(id));
    }


}

void client::AddProduct()
{
    if (ui->B1->isChecked()) {
        ui->idLINE->show();
        ui->nameLINE->show();
        ui->priceLINE->show();
        ui->quantityLINE->show();
        ui->IDlabel->show();
        ui->Namelabel->show();
        ui->Pricelabel->show();
        ui->Quantitylabel->show();
        disableOtherButtons(ui->B1);
    }
    else
    {
        ui->nameLINE->clear();
        ui->priceLINE->clear();
        ui->quantityLINE->clear();
        ui->idLINE->clear();
        ui->nameLINE->hide();
        ui->priceLINE->hide();
        ui->quantityLINE->hide();
        ui->idLINE->hide();
        ui->IDlabel->hide();
        ui->Namelabel->hide();
        ui->Pricelabel->hide();
        ui->Quantitylabel->hide();
        enableAllButtons();
    }
    
}

void client::deleteData()
{
    if (ui->B2->isChecked()) {
        ui->idLINE->show();
        ui->IDlabel->show();
        disableOtherButtons(ui->B2);
    }
    else
    {
        ui->idLINE->clear();
        ui->idLINE->hide();
        ui->IDlabel->hide();
        enableAllButtons();
    }
}

void client::updateData()
{
    if (ui->B3->isChecked()) {
        ui->idLINE->show();
        ui->nameLINE->show();
        ui->priceLINE->show();
        ui->quantityLINE->show();
        ui->IDlabel->show();
        ui->Namelabel->show();
        ui->Pricelabel->show();
        ui->Quantitylabel->show();
        disableOtherButtons(ui->B3);
    }
    else
    {
        ui->nameLINE->clear();
        ui->priceLINE->clear();
        ui->quantityLINE->clear();
        ui->idLINE->clear();
        ui->nameLINE->hide();
        ui->priceLINE->hide();
        ui->quantityLINE->hide();
        ui->idLINE->hide();
        ui->IDlabel->hide();
        ui->Namelabel->hide();
        ui->Pricelabel->hide();
        ui->Quantitylabel->hide();
        enableAllButtons();
    }
}

void client::getDataById()
{
    if (ui->B4->isChecked()) {
        ui->idLINE->show();
        ui->IDlabel->show();
        disableOtherButtons(ui->B4);
    }
    else
    {
        ui->idLINE->clear();
        ui->idLINE->hide();
        ui->IDlabel->hide();
        enableAllButtons();
    }
}

void client::getAllData()
{
    network.sendRequest("GET", address + "/products");
}

void client::rememberAdress()
{
    if (ui->rememberBTN->isChecked()) {
        QString address_ = ui->AddressLine->text();
        address = address_;
    }
    else {
        address.clear(); // Очищаем адрес, когда кнопка отпущена
    }
}


client::~client()
{
}


void client::disableOtherButtons(QPushButton* activeButton)
{
    QList<QPushButton*> buttons = {
        ui->B1,
        ui->B2,
        ui->B3,
        ui->B4,
        ui->B5,
        ui->B6,
    };

    for (QPushButton* button : buttons) {
        if (button != activeButton) {
            button->setEnabled(false);
        }
    }
}

void client::enableAllButtons()
{
    QList<QPushButton*> buttons = {
        ui->B1,
        ui->B2,
        ui->B3,
        ui->B4,
        ui->B5,
        ui->B6,
    };

    for (QPushButton* button : buttons) {
        button->setEnabled(true);
    }
}

void client::handleNetworkResponse(const QString& response) {
    ui->responseTXT->setText(response);
}
