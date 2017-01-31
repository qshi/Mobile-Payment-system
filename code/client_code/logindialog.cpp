#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "emaildialog.h"
#include <winsock.h>
#include "AES.h"
#include <iostream>
using std::cout;
using std::endl;

unsigned char key[16];/* =
{
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c
};*/


LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_loginbtn_clicked()
{
    int pkey[4];
    for(int i=0;i<4;i++)
    {
        pkey[i] = this->pre_key + i;
    }
    memcpy(key,pkey,16);
    int flag = 0;
    char sendint[4];
    memcpy(sendint,&flag,4);
    send(this->clientsocket,sendint,sizeof(sendint)+1,0);
    AES aes(key);
    QString usrn = this->ui->usrLineEdit->text();
    QString pswd = this->ui->pwdLineEdit->text();
    char *us;
    char *ps;
    char uss[32];
    char pss[32];
    char reciint[4];
    int res;
    QByteArray ba1 = usrn.toLatin1();
    us=ba1.data();
    QByteArray ba2 = pswd.toLatin1();
    ps=ba2.data();
    memcpy(uss,us,32);
    memcpy(pss,ps,32);
    aes.Cipher((void*)uss,21);
    aes.Cipher((void*)pss,21);
    send(this->clientsocket,uss,sizeof(uss)+1,0);
    send(this->clientsocket,pss,sizeof(pss)+1,0);
    recv(this->clientsocket,reciint,sizeof(reciint)+1,0);
 //   cout<<"received"<<endl;
    memcpy(&res,&reciint[0],4);
 //   cout<<"res="<<res<<endl;
    if(res==1)
    {
        cout<<"Log in success!"<<endl;
        accept();
    }
    else
    {
        QMessageBox::warning(this,tr("error"),
    tr("Invalid Username or Password"),
    QMessageBox::Yes);
    }

}

void LoginDialog::on_pushButton_2_clicked()
{
    int pkey[4];
    for(int i=0;i<4;i++)
    {
        pkey[i] = this->pre_key + i;
    }
    cout<<this->pre_key<<endl;
    memcpy(key,pkey,16);
    int flag = 1;
    char sendint[4];
    memcpy(sendint,&flag,4);
    send(this->clientsocket,sendint,sizeof(sendint)+1,0);
    cout<<"flag sent"<<endl;
    AES aes(key);
    QString usrn = this->ui->usrLineEdit->text();
    QString pswd = this->ui->pwdLineEdit->text();
    QString emad = this->ui->emlineEdit->text();
    char *us;
    char *ps;
    char *em;
    char uss[32];
    char pss[32];
    char ems[32];
    char reciint[4];
    int res;
    QByteArray ba1 = usrn.toLatin1();
    us=ba1.data();
    QByteArray ba2 = pswd.toLatin1();
    ps=ba2.data();
    QByteArray ba3 = emad.toLatin1();
    em=ba3.data();
    memcpy(uss,us,32);
    memcpy(pss,ps,32);
    memcpy(ems,em,32);
    aes.Cipher((void*)uss,21);
    aes.Cipher((void*)pss,21);
    aes.Cipher((void*)ems,21);
    cout<<"data ciphered"<<endl;
    send(this->clientsocket,uss,sizeof(uss)+1,0);
    send(this->clientsocket,pss,sizeof(pss)+1,0);
    send(this->clientsocket,ems,sizeof(pss)+1,0);
    cout<<"data sent"<<endl;
    recv(this->clientsocket,reciint,sizeof(reciint)+1,0);
    cout<<"got reply"<<endl;
    memcpy(&res,&reciint[0],4);
    if(res==1)
    {
        QMessageBox::warning(this,tr("Welcome!"),
        tr("You have signed in successfully!"),
        QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(this,tr("Error!"),
        tr("Invalid Information, please try again!"),
        QMessageBox::Yes);
    }
}
