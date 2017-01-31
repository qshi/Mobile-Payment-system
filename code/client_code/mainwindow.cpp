#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "emaildialog.h"
#include <QMessageBox>
#include <winsock.h>
#include <iostream>
#include "AES.h"

using std::cout;
using std::endl;

unsigned char key_info[16];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cout<<"Starting mainwindow!"<<endl;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pinButton_clicked()
{

//        unsigned char key[16];
        int pre[4];
        pre[0] = this->pre_key;
        pre[1] = this->pre_key+1;
        pre[2] = this->pre_key+2;
        pre[3] = this->pre_key+3;
        memcpy(key_info,pre,16);
        AES aes(key_info);
        QString cadn = this->ui->CardLineEdit->text();
        QString pswd = this->ui->pwLineEdit->text();
        QString chnm = this->ui->namelineEdit->text();
        char *cd;
        char *pw;
        char *nm;
        char cds[32];
        char pws[32];
        char nms[32];
        char reciint[4];
        int res;
        QByteArray ba1 = cadn.toLatin1();
        cd=ba1.data();
        QByteArray ba2 = pswd.toLatin1();
        pw=ba2.data();
        QByteArray ba3 = chnm.toLatin1();
        nm=ba3.data();
        memcpy(cds,cd,32);
        memcpy(pws,pw,32);
        memcpy(nms,nm,32);
        aes.Cipher((void*)cds,21);
        aes.Cipher((void*)pws,21);
        aes.Cipher((void*)nms,21);
        send(this->clientsocket,cds,sizeof(cds)+1,0);
        send(this->clientsocket,pws,sizeof(pws)+1,0);
        send(this->clientsocket,nms,sizeof(nms)+1,0);
        recv(this->clientsocket,reciint,sizeof(reciint)+1,0);
        memcpy(&res,reciint,4);
        if(res==0)
        {
            QMessageBox::warning(this, tr("Waring"),
                                 tr("Account number or password error!"),
                                 QMessageBox::Yes);
        }
        else if(res==1)
        {
            QMessageBox::warning(this, tr("Success!"),
                             tr("You have already paid successfully!"),
                             QMessageBox::Yes);
        }
        else
        {
            EmailDialog *e = new EmailDialog(this);
            e->clientsocket = this->clientsocket;
            e->c_code = this->c_code;
            e->show();
        }


}

