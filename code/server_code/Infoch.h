#ifndef __INFOCH_H__ //±‹√‚÷ÿ∏¥∞¸∫¨
#define __INFOCH_H__

#include<string>
using std::string;

class Info
{

public:
	unsigned long lh_strhash(const char *str);
	int client_sign_up(SOCKET serverSocket);
	int merchent_sign_up(SOCKET serverSocket);
	string log_in(SOCKET serverSocket,int ID);
	int check_info(SOCKET serverSocket);
	void DH_exchang(SOCKET serverSocket);
};



#endif