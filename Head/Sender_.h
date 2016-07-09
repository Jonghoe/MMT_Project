#pragma once
#include<WinSock2.h>
#include<string>
#define LEN 12
class Sender{
private:
	WSADATA mvWsa_Data;
	SOCKET mvS_Socket;
	SOCKADDR_IN mvServ_Adr;
public:
	Sender();
	void mSend(std::string);
	int mSet_Connect_S(std::string IP = "127.0.0.1",std::string Port = "9190");
};