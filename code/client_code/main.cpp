#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"
#include "emaildialog.h"
#include <WINSOCK.H>
#include <time.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <string>
using std::string;
#include <conio.h>
#include "AES.h"


#pragma comment(lib,"ws2_32.lib")


int prime;
int proot;
int Xc;
int Yc;
int Ys;
int K;
int pre_key[4];


unsigned long lh_strhash(const char *str)
{
    int i, l;
    unsigned long ret = 0;
    unsigned short *s;

    if (str == NULL) return(0);
    l = (strlen(str) + 1) / 2;
    s = (unsigned short *)str;
    for (i = 0; i<l;i++)
        ret ^= (s[i] << (i & 0x0f));
        return(ret);
}


void DH_exchang(SOCKET clientSocket)
{
    char sendbuf[20];
    char recibuf[20];
    char sendint[4];
    char reciint[4];
    string s = "hello";
    char hello[6] = "hello";
    memcpy(sendbuf, s.c_str(), 20);
    send(clientSocket, sendbuf, sizeof(sendbuf)+1, 0);
    recv(clientSocket, recibuf, sizeof(recibuf)+1, 0);
    if (strcmp(recibuf, hello))
    {
        cout << "Do not get reply from server!" << endl;
        exit(0);
    }
    recv(clientSocket, reciint, sizeof(reciint)+1, 0);
    memcpy(&prime, reciint, 4);
    recv(clientSocket, reciint, sizeof(reciint)+1, 0);
    memcpy(&proot, reciint, 4);
    cout<<prime<<" "<<proot<<endl;
    srand(time(NULL));
    Xc = rand() % prime;
    cout<<"Xc="<<Xc<<endl;
    Yc = 1;
    for (int i = 0; i<Xc; i++)
    {
        Yc = Yc * proot;
        Yc = Yc % prime;
    }
    cout<<"Yc="<<Yc<<endl;
    memcpy(sendint, &Yc, 4);
    send(clientSocket, sendint, sizeof(sendint)+1, 0);
    recv(clientSocket, reciint, sizeof(reciint)+1, 0);
    memcpy(&Ys, reciint, 4);
    Ys = Yc;
    cout<<"Ys="<<Ys<<endl;
    K = 1;
    for (int i = 0; i<Xc; i++)
    {
        K = K * Ys;
        K = K % prime;
    }
 /*   pre_key[0] = K;
    cout<<K<<endl;
    for (int i = 1; i <= 3; i++)
    {
        pre_key[i] = K+i;
    }
    memcpy(key, pre_key, 16);*/
}


int main(int argc, char *argv[])
{
    string certificate[9];
    certificate[0] = "1";
    certificate[1] = "123";
    certificate[2] = "Payment Client";
    certificate[3] = "XXXXX";
    certificate[4] = "20151001";
    certificate[5] = "20161001";
    certificate[6] = "DH";
    certificate[7] = "YC";
    certificate[8] = "paymentsystemclientdonebyxiaomu";

    QApplication a(argc, argv);
    MainWindow w;
//    EmailDialog s;
    LoginDialog dlg;
/*    if(dlg.exec() == QDialog::Accepted)
    {
    w.show();
//    return a.exec();
    }*/
    int err;
        WORD versionRequired;
        WSADATA wsaData;                            //Include the information of WINSOCK
        versionRequired = MAKEWORD(1, 1);           //Initial Version 1.1
        //注册WinStock，返回状态
        err = WSAStartup(versionRequired, &wsaData);//Information of protocol library
        if (!err)                                   //Return 0 means initial error
        {
            cout << LPSTR("Client Socket Opened!\n");
        }
        else
        {
            //use WSAGetLastError() to check error message
            cout << ("Client Socket Failed") << WSAGetLastError()<<endl;
            return 0;//End
        }
        /*
        Create Socket：
        Stream Socket：   SOCK_STREAM , IPPROTO_TCP
        Datagram Socket: SOCK_DGRAM  , IPPROTO_UDP
        */
        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    //Create Stream Socket
        SOCKADDR_IN clientsock_in;                                          //Especially Internet communication area Winsock address
        clientsock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");        //Through inet_addr structure to determine the server's IP
        clientsock_in.sin_family = AF_INET;                                 //Determine protocol family:AF_INET
        clientsock_in.sin_port = htons(3000);                               //Determine port：3000
        cout << "initial success!" << endl;
        int fail = connect(clientSocket, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR));//Start to connect
    //	cout << fail << endl;
        if (fail){
            cout << "Failed to connect to server, exiting..." << endl;
            _getch();
            return 0;
        }
   //     string X = "qwertyuiop";
   //     AES aes(key);
        char recicode[7];
        char authbuf[4];
        unsigned long auth;

        for(int i=0;i<9;i++)
        {
            send(clientSocket,certificate[i].c_str(),33,0);
        }

        auth = lh_strhash(certificate[8].c_str());
        memcpy(authbuf,&auth,4);
        send(clientSocket,authbuf,sizeof(authbuf)+1,0);
        DH_exchang(clientSocket);
        recv(clientSocket,recicode,sizeof(recicode)+1,0);
        string con_code = recicode;
        w.clientsocket = clientSocket;
        w.pre_key = K;
        w.c_code = con_code;
        dlg.clientsocket = clientSocket;
        dlg.pre_key = K;

        if(dlg.exec() == QDialog::Accepted)
        {
            w.show();
            return a.exec();
        }


        system("pause");
        return 0;
   }


