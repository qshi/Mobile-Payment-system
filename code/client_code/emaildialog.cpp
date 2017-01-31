#include "emaildialog.h"
#include "ui_emaildialog.h"
#include <QMessageBox>
#include <winsock.h>
#include <iostream>

using std::cout;
using std::endl;

EmailDialog::EmailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailDialog)
{
    ui->setupUi(this);
}

EmailDialog::~EmailDialog()
{
    delete ui;
}

void EmailDialog::on_pushButton_clicked()
{
    QString code = this->ui->emailLineEdit->text();
//    QString code = this->ui->
    char *cd;
    char cds[32];
//    char reciint[4];
//    int res=1;
    QByteArray ba1 = code.toLatin1();
    cd=ba1.data();
    memcpy(cds,cd,32);
    string cc = cds;
    send(this->clientsocket,cds,sizeof(cds)+1,0);
 //   recv(this->clientsocket,reciint,sizeof(reciint)+1,0);
 //   cout<<"111:"<<res<<endl;
//    memcpy(&res,reciint,4);
//    cout<<"111:"<<res<<endl;
    if(cc==this->c_code)
    {
        QMessageBox::warning(this, tr("Thank you"),
                             tr("You have already paid for it!"),
                             QMessageBox::Yes);
        ui->emailLineEdit->clear();
        ui->emailLineEdit->setFocus();

    }
    else {
        QMessageBox::warning(this, tr("Waring"),
                             tr("The code you enter is error!"),
                             QMessageBox::Yes);

        ui->emailLineEdit->clear();
        ui->emailLineEdit->setFocus();
    }
//    close();
}
