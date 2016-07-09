#pragma once
#include"Sender.h"
Sender::Sender(){}
int Sender::mSet_Connect_S(std::string IP,std::string Port){
	if (WSAStartup(MAKEWORD(2, 2), &mvWsa_Data) != 0)
		return 0;

	mvS_Socket = socket(PF_INET, SOCK_STREAM, 0);
	if (mvS_Socket == INVALID_SOCKET)
		return 0;
	memset(&mvServ_Adr, 0, sizeof(mvServ_Adr));
	mvServ_Adr.sin_family = AF_INET;
	mvServ_Adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	mvServ_Adr.sin_port = htons(atoi("9190")); 
	if (connect(mvS_Socket, (SOCKADDR*)&mvServ_Adr, sizeof(mvServ_Adr)) == SOCKET_ERROR)
		return 0;
	else
		printf("connected...........\n");
	return 1;
}

void Sender::mSend( std::string Info){
	int recv_cnt, recv_len=0;
	char tmp[LEN];
	for (int i = 0; i < LEN;i++)
		tmp[i] = Info[i];
	
	send(mvS_Socket, tmp, LEN, 0);
	while (recv_len < LEN){
		recv_cnt = recv(mvS_Socket, tmp, LEN, 0);
		recv_len += recv_cnt;
	}
}