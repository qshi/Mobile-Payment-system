#include <WINSOCK.H>
#include <iostream>
#include <fstream>
#include <string>
#include "AES.h"
#include "Infoch.h"
#include <time.h>

using std::ofstream;
using std::fstream;
using std::ios;
using std::string;
using std::cout;
using std::endl;

int prime = 97;
int proot = 5;
int Xs;
int Ys;
int Yc;
int K;
int pre_key[4];


unsigned char key[16];/* =
{
	0x2b, 0x7e, 0x15, 0x16,
	0x28, 0xae, 0xd2, 0xa6,
	0xab, 0xf7, 0x15, 0x88,
	0x09, 0xcf, 0x4f, 0x3c
};*/

unsigned long Info::lh_strhash(const char *str)
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

int Info::client_sign_up(SOCKET serverSocket)
{
	char username[32];
	char password[32];
	char emailadd[32];
	AES aes(key);
	ofstream out("client.txt", ios::app);
	if (recv(serverSocket, username, sizeof(username)+1, 0) == 0)
	{
		return 0;
	}
	aes.InvCipher((void*)username, 21);
	string un = username;
	if (recv(serverSocket, password, sizeof(password)+1, 0) == 0)
	{
		return 0;
	}
	aes.InvCipher((void*)password, 21);
	string pw = password;
	if (recv(serverSocket, emailadd, sizeof(emailadd)+1, 0) == 0)
	{
		return 0;
	}
	aes.InvCipher((void*)emailadd, 21);
	string em = emailadd;
	
	if (out.is_open())
	{
		out << un <<"#";
		out << pw <<"#";
		out << em << "\n";
		out.close();
		return 1;
	}
	else
	{
		return 0;
	}

}

int Info::merchent_sign_up(SOCKET serverSocket)
{
	char username[32];
	char password[32];
	AES aes(key);
	ofstream out("merchant.txt", ios::app);
	if (recv(serverSocket, username, sizeof(username)+1, 0) == 0)
	{
		return 0;
	}
	if (recv(serverSocket, password, sizeof(password)+1, 0) == 0)
	{
		return 0;
	}
	aes.Cipher((void*)username, 21);
	aes.Cipher((void*)password, 21);
	string un = username;
	string pw = password;
	if (out.is_open())
	{
		out << un << "#";
		out << pw << "\n";
		out.close();
		return 1;
	}
	else
	{
		return 0;
	}
}

string Info::log_in(SOCKET serverSocket,int ID)
{
	char username[32];
	char password[32];
	AES aes(key);
	fstream file;
	if (ID == 0)
		file.open("merchant.txt", ios::in);
	else
		file.open("client.txt", ios::in);
	if (recv(serverSocket, username, sizeof(username)+1, 0) == 0)
	{
		return 0;
	}
	cout << "The data recieved:" << endl;
	for (int j = 0; j<32; j++)printf("%X", (unsigned char)username[j]);
	printf("\n");
	cout << "The data after decryption:" << endl;
	aes.InvCipher((void*)username, 21);
	string un = username;
	cout << un << endl;
	if (recv(serverSocket, password, sizeof(password)+1, 0) == 0)
	{
		return 0;
	}
	cout << "The data recieved:" << endl;
	for (int j = 0; j<32; j++)printf("%X", (unsigned char)password[j]);
	printf("\n");
	cout << "The data after decryption:" << endl;
	aes.InvCipher((void*)password, 21);
	string pw = password;
	cout << pw << endl;
	string cur;
	while (getline(file,cur))
	{
		int p = cur.find_first_of('#');
		string s1 = cur.substr(0, p);
		string s2 = cur.substr(p + 1);
		p = s2.find_first_of('#');
		string s3 = s2.substr(p + 1);
		s2 = s2.substr(0, p);
		if (s1 == un && s2 == pw)
		{
			return s3;
		}
	}
	return "0";
}

int Info::check_info(SOCKET serverSocket)
{
	char cardnumber[32];
	char securenumber[32];
	char name[32];
	AES aes(key);
	if (recv(serverSocket, cardnumber, sizeof(cardnumber)+1, 0) == 0)
	{
		return 0;
	}
	cout << "The data recieved:" << endl;
	for (int j = 0; j<32; j++)printf("%X", (unsigned char)cardnumber[j]);
	printf("\n");
	cout << "The data after decryption:" << endl;
	aes.InvCipher((void*)cardnumber, 21);
	string cn = cardnumber;
	cout << cn << endl;
	if (recv(serverSocket, securenumber, sizeof(securenumber)+1, 0) == 0)
	{
		return 0;
	}
	cout << "The data recieved:" << endl;
	for (int j = 0; j<32; j++)printf("%X", (unsigned char)securenumber[j]);
	printf("\n");
	cout << "The data after decryption:" << endl;
	aes.InvCipher((void*)securenumber, 21);
	string sn = securenumber;
	cout << sn << endl;
	if (recv(serverSocket, name, sizeof(name)+1, 0) == 0)
	{
		return 0;
	}
	cout << "The data recieved:" << endl;
	for (int j = 0; j<32; j++)printf("%X", (unsigned char)name[j]);
	printf("\n");
	cout << "The data after decryption:" << endl;
	aes.InvCipher((void*)name, 21);
	string nm = name;
	cout << nm << endl;



	fstream file;
	file.open("card.txt", ios::in);
	string cur;
	while (getline(file, cur))
	{
		int p = cur.find_first_of('#');
		string s1 = cur.substr(0, p);
		string s2 = cur.substr(p + 1);
		p = s2.find_first_of('#');
		string s3 = s2.substr(p + 1);
		s2 = s2.substr(0, p);
		if (s1 == cn && s2 == sn && s3 == nm)
		{
			return 1;
		}
	}
	return 0;
}

void Info::DH_exchang(SOCKET serverSocket)
{
	char sendbuf[20];
	char recibuf[20];
	char sendint[4];
	char reciint[4];
	string s = "hello";
	char *hello = "hello";

	do
	{
		recv(serverSocket, recibuf, sizeof(recibuf)+1, 0);
	} while (strcmp(recibuf, hello));
	memcpy(sendbuf, s.c_str(), 20);
	send(serverSocket, sendbuf, sizeof(sendbuf)+1, 0);

	memcpy(sendint, &prime, 4);
	send(serverSocket, sendint, sizeof(reciint)+1, 0);
	memcpy(sendint, &proot, 4);
	send(serverSocket, sendint, sizeof(reciint)+1, 0);

	srand(time(NULL));
	Xs = rand() % prime;
	cout << "Xs=" << Xs << endl;
	Ys = 1;
	for (int i = 0; i<Xs; i++){
		Ys = Ys * proot;
		Ys = Ys % prime;
	}
	cout << "Ys=" << Ys << endl;
	memcpy(sendint, &Ys, 4);
	recv(serverSocket, reciint, sizeof(reciint)+1, 0);
	memcpy(&Yc, reciint, 4);
	cout << "Yc=" << Yc << endl;
	send(serverSocket, sendint, sizeof(sendint)+1, 0);

	K = 1;
	for (int i = 0; i<Xs; i++){
		K = K * Yc;
		K = K % prime;
	}
	pre_key[0] = K;
	for (int i = 1; i <= 3; i++)
	{
		pre_key[i] = K + i;
	}
	memcpy(key, pre_key, 16);
	cout << K;
}