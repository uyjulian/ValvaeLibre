/*
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include <windows.h>
#include <ws2tcpip.h>
#include <fileapi.h>
#include <stdio.h>
*/
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <vector>
using namespace std;
using namespace std::chrono;

/*
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
*/

int teeth_passed, gaps_passed;
size_t i;
vector<int> gaps_locations;
vector<time_point<system_clock>> gaps_locations_times;
bool signal_in, start_signal, in_big_gap;
time_point<system_clock> universal_start, teeth_gap_start;
duration<double> elapsed, guess;
double rpm, signal_length, signal_length_2, full_rotation, angle;

int main() {
	signal_length = ((35 * 2 / 360) / (rpm / 60)) * 1000000000; // Value in nanoseconds
	signal_length_2 = signal_length * 2;
	full_rotation = signal_length * 35 + signal_length * 34 + signal_length_2;
	universal_start = system_clock::now();
	elapsed = duration_cast<nanoseconds>(elapsed);
off:
	teeth_passed = gaps_passed = i = 0;
	rpm = 600.0; // Replace this with whatever the starting value is
	while (true) {
		
	}
verify: // Receives start_signal and teeth_gap_start from off
	while (teeth_passed < 35 && gaps_passed < 35) {
		signal_in = true; // REPLACE THIS WITH READING FROM ACTUAL PIPE
		elapsed = system_clock::now() - teeth_gap_start;
		if (!in_big_gap && elapsed.count() > signal_length) {
			gaps_locations[i] = gaps_passed;
			in_big_gap = true;
		}
		if (signal_in != start_signal) {
			teeth_gap_start = system_clock::now();
			start_signal = signal_in;
			if (in_big_gap) {
				gaps_locations_times[i] = teeth_gap_start;
				i++;
			}
			in_big_gap = false;
			if (signal_in) {
				teeth_passed++;
			}
			else {
				gaps_passed++;
			}
		}
	}
	//WILL BE REFORMATTED WHEN WE USE A PROPER FILE OR TABLE
	elapsed = system_clock::now() - gaps_locations_times[0];
	angle = (elapsed.count()/full_rotation) * 360.0;
	gaps_passed = teeth_passed = 0;
	//WILL BE REFORMATTED WHEN WE USE A PROPER FILE OR TABLE
	do {
		for (size_t j = 0; j < gaps_locations.size(); j++) {
			if (gaps_passed = gaps_locations[j]) {
				in_big_gap = true;
				break;
			}
		}
		if (in_big_gap) {
			guess = (system_clock::now() + nanoseconds(full_rotation)) - system_clock::now();
			// INCOMPLETE
		}
	} while (angle != 0.0);
send_verify:
	while (true) {

	}
}