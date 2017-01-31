n#include <thread>
using std::thread;
#include <mutex>
using std::mutex;

#include <WINSOCK.H>
#include <stdio.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

#include <fstream>
using std::ofstream;
using std::fstream;
using std::ios;

#include "AES.h"
#include "Smtp.h"
#include "Infoch.h"


#pragma comment(lib,"ws2_32.lib")

mutex monitor;
mutex channel;


class Certificate
{
public:
	string cert_info[9];

	void display()
	{
		cout << "/*********Certificate***********/" << endl;
		cout << "  Version:       " << cert_info[0] << endl;
		cout << "  Serial Number: " << cert_info[1] << endl;
		cout << "  Object Name:   " << cert_info[2] << endl;
		cout << "  Issued By:     " << cert_info[3] << endl;
		cout << "  Activation:    " << cert_info[4] << endl;
		cout << "  Expiration:    " << cert_info[5] << endl;
		cout << endl;
		cout << "  Algorithm:     " << cert_info[6] << endl;
		cout << "  Public Key:    " << cert_info[7] << endl;
		cout << endl;
		cout << "Digital Signature:" << endl;
		char sign[32];
		memcpy(sign, cert_info[8].c_str(), 32);
		cout << "  ";
		for (int j = 0; j<16; j++)printf("%X", (unsigned char)sign[j]);
		cout << endl;
		cout << "  ";
		for (int j = 16; j<32; j++)printf("%X", (unsigned char)sign[j]);
		cout << endl;
		cout << "/*********************************/" << endl;
		cout << endl;
	}


};



//unsigned char key[16] =
//{
//	0x2b, 0x7e, 0x15, 0x16,
//	0x28, 0xae, 0xd2, 0xa6,
//	0xab, 0xf7, 0x15, 0x88,
//	0x09, 0xcf, 0x4f, 0x3c
//};




void connection(LPVOID lparam, string IP)
{
//	AES aes(key);
	Info info;
	SOCKET serConn = (SOCKET)(LPVOID)lparam;
	string emailadd;
//	char authentication[32]="paymentsystemclient";
	char certificate[32];
	unsigned long auth;
	char authbuf[4];
	Certificate cert;
	for (int i = 0; i < 9; i++)
	{
		recv(serConn, certificate, sizeof(certificate)+1, 0);
		cert.cert_info[i] = certificate;
	}

	cert.display();

	recv(serConn, authbuf, sizeof(authbuf)+1, 0);
	memcpy(&auth, authbuf, 4);
	if (auth != info.lh_strhash(cert.cert_info[8].c_str()))
	{
		cout << "Authentication Failed! Exiting..." << endl;
//		return;
	}
	else
	{
		cout << "Authentication Successful!" << endl << endl;
	}
	//等待客户端的连接
//	serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);
//	while (!monitor.try_lock());
	cout << "Client " << IP << " has connected!" << endl;             //客户端已连接
	info.DH_exchang(serConn);
/*	while (info.log_in(serConn, key, 1) == 0)
	{
		cout << "Invalid Username or Password, Please try again!" << endl;
	}*/
//	monitor.unlock();
	srand(time(0));
	int confirm_code[6];
	string cc = "";
	for (int i = 0; i < 6; i++)
	{
		confirm_code[i] = rand() % 10;
		cc += (confirm_code[i] + 48);
	}
	send(serConn, cc.c_str(), cc.length() + 1, 0);
	while (1) {
		char receiveBuf[32];
		char sendBuf[32] = "Payment Authorized!";
		char intbuf[4];
		fstream file;
		int flag = 0;
		recv(serConn, intbuf, sizeof(intbuf)+1, 0);
		memcpy(&flag, intbuf, 4);
		if (flag==1)
		{
			if (info.client_sign_up(serConn) == 0)
			{
				int a = 0;
				memcpy(intbuf, &a, 4);
				send(serConn, intbuf, sizeof(intbuf)+1, 0);
				continue;
			}
			else
			{
				int a = 1;
				memcpy(intbuf, &a, 4);
				send(serConn, intbuf, sizeof(intbuf)+1, 0);
			}
			flag = 0;
			continue;
		}
		emailadd = info.log_in(serConn, 1);
		if (emailadd == "0")
		{
			int a = 0;
			memcpy(intbuf, &a, 4);
			send(serConn, intbuf, sizeof(intbuf)+1, 0);
			continue;
		}
		else
		{
			int a = 1;
			memcpy(intbuf, &a, 4);
			send(serConn, intbuf, sizeof(intbuf)+1, 0);
		}

		cout << "checking card info..." << endl;
		if (info.check_info(serConn) == 0)
		{
			int a = 0;
			memcpy(intbuf, &a, 4);
			send(serConn, intbuf, sizeof(intbuf)+1, 0);
			cout << "Card info wrong! transaction denied!" << endl;
			continue;
		}
		file.open("ip.txt",ios::in);
		string s;
		int check = 0;
		while (getline(file, s))
		{
			if (IP == s)
			{
				check = 1;
				break;
			}
		}
		if (!check)
		{
			int a = 2;
			memcpy(intbuf, &a, 4);
			send(serConn, intbuf, sizeof(intbuf)+1, 0);
		/*	srand(time(0));
			int confirm_code[6];
			string cc = "";
			for (int i = 0; i < 6; i++)
			{
				confirm_code[i] = rand() % 10;
				cc += (confirm_code[i] + 48);
			}*/
			CSmtp smtp(
				25,								//*smtp端口
				"smtp.qq.com",					//*smtp服务器地址
				"360518909@qq.com",	//*你的邮箱地址
				"jmiwqtcrkcwrbgee",					//*邮箱密码
				emailadd,			//*目的邮箱地址
				"Comfirmation Test",							//*主题
				"Your confirmation code is:"+cc		//*邮件正文
				);
			if ((smtp.SendEmail_Ex()) != 0)
			{
				cout << "Send fail!" << endl;
				continue;
			}
			recv(serConn, receiveBuf, sizeof(receiveBuf)+1, 0);
			string code = receiveBuf;
			if (code != cc)
			{
				int a = 0;
				memcpy(intbuf, &a, 4);
				send(serConn, intbuf, sizeof(intbuf)+1, 0);
				cout << "Code wrong! transaction denied!" << endl;
				continue;
			}
			else
			{
				int a = 1;
				memcpy(intbuf, &a, 4);
				send(serConn, intbuf, sizeof(intbuf)+1, 0);
				cout << "payment success!" << endl;
				ofstream out("ip.txt", ios::app);
				if (out.is_open())
				{
					out << IP << endl;
				}
			}
	/*		aes.Cipher(sendBuf);
			send(serConn, sendBuf, sizeof(sendBuf) + 1, 0);*/
		}
		else
		{
			int a = 1;
			memcpy(intbuf, &a, 4);
			send(serConn, intbuf, sizeof(intbuf)+1, 0);
		}
	}
	closesocket(serConn);   //关闭
	WSACleanup();           //释放资源的操作
}


void main()
{
	//创建套接字
	
	WORD myVersionRequest;
	WSADATA wsaData;                    //包含系统所支持的WinStock版本信息
	myVersionRequest = MAKEWORD(1, 1);  //初始化版本1.1
	int err;
	err = WSAStartup(myVersionRequest, &wsaData);
	if (!err){
		printf("Socket Opened.\n");
	}
	else{
		//进一步绑定套接字
		printf("Socket Open Failed!");
		return;
	}
	SOCKET serSocket = socket(AF_INET, SOCK_STREAM, 0);//创建了可识别套接字
	//需要绑定的参数
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ip地址
	addr.sin_port = htons(3000);//绑定端口
	//将套接字绑定到指定的网络地址
	int b = bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//绑定完成
	if (b != 0)
	{
		cout << "Bind error" << endl;
	}
	int l = listen(serSocket, 10);                              //第二个参数代表能够接收的最多的连接数
	if (l != 0)
	{
		cout << "listening failed!" << endl;
	}

//	serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);

	while (1)
	{
		
		SOCKADDR_IN clientsocket;
		int len = sizeof(SOCKADDR);
		SOCKET serConn;
		serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);
		string IP = inet_ntoa(clientsocket.sin_addr);
//		string IP = "127.0.0.1";
		Sleep(1000);
		thread* th = new thread(connection, (LPVOID)serConn,IP);
		th->detach();
		Sleep(1000);
	}
}