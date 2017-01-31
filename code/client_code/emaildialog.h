#ifndef EMAILDIALOG_H
#define EMAILDIALOG_H

#include <QDialog>
#include <winsock.h>
#include <string>

using std::string;

namespace Ui {
class EmailDialog;
}

class EmailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailDialog(QWidget *parent = 0);
    ~EmailDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EmailDialog *ui;
public:
    SOCKET clientsocket;
    string c_code;
};

#endif // EMAILDIALOG_H
