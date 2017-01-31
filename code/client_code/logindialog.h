#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <winsock.h>
#include "AES.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_loginbtn_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LoginDialog *ui;
public:
 //   unsigned char key[16] = {};
    SOCKET clientsocket;
    int pre_key;
};

#endif // LOGINDIALOG_H
