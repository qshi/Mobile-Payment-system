#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <WINSOCK.H>
#include <string>
#include "AES.h"

using std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pinButton_clicked();

private:
    Ui::MainWindow *ui;
public:
    SOCKET clientsocket;
    int pre_key;
    string c_code;
};

#endif // MAINWINDOW_H
