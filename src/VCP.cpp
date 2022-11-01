#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <fileapi.h>
#include <stdlib.h>
using namespace std;

int network_testing() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	int sending_fd, receiving_fd, receiving_dump, VCP_socket_desc, valsend, valread, buffer_reinterp;
	char buffer[] = "    ";
	const char opt = '1';
	// Setting up own_addr and receiving_fd as server waiting for crankshaft signals
	struct sockaddr_in own_addr;
	int addrlen = sizeof(own_addr);
	if ((receiving_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(receiving_fd, SOL_SOCKET,
		SO_REUSEADDR, &opt,
		sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	own_addr.sin_family = AF_INET;
	own_addr.sin_addr.s_addr = INADDR_ANY;
	own_addr.sin_port = 8081;
	if (bind(receiving_fd, (struct sockaddr*)&own_addr,
		sizeof(own_addr))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(receiving_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((receiving_dump
		= accept(receiving_fd, (struct sockaddr*)&own_addr,
			(socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	do {
		valread = recv(receiving_dump, buffer, 4, 0);
		buffer_reinterp = *(reinterpret_cast<int*>(buffer));
		printf("Checking: %d %d\n", valread, buffer_reinterp);
		Sleep(1000);
	} while (valread != -1);
	exit(EXIT_SUCCESS);
	// Setting up serv_addr and sending_fd as client to send valve signals
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = 8080;
	VCP_socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	if ((sending_fd
		= connect(VCP_socket_desc, (struct sockaddr*)&serv_addr,
			sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
}