#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <fileapi.h>
#include <stdlib.h>
#include<algorithm>
using namespace std;


int networking_test() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	int VCP_socket_desc, sending_fd;
	char buffer[] = "AAAA";
	int a = 17;
	int b = 56;
	// Setting up serv_addr and sending_fd as client to send valve signals
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = 8081;
	VCP_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	if ((sending_fd
		= connect(VCP_socket_desc, (struct sockaddr*)&serv_addr,
			sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	while (true) {
		copy(reinterpret_cast<char*>(&a), reinterpret_cast<char*>(&a) + 4, &buffer[0]);
		send(VCP_socket_desc, buffer, 4, 0);
		copy(reinterpret_cast<char*>(&b), reinterpret_cast<char*>(&b) + 4, &buffer[0]);
		send(VCP_socket_desc, buffer, 4, 0);
	}
	closesocket(VCP_socket_desc);
}