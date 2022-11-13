#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <table.h>
#include <windows.h> 
using namespace std;
using namespace std::chrono;

unsigned long i, latency_count, teeth_gap_passed, angle, start_angle, signal_length, signal_length_offset, signal_length_3;
unordered_map<int,int> gaps_locations;
bool start_signal;
time_point<system_clock> teeth_gap_start, guess, last_big_gap;
duration<double> elapsed;
double rpm;
long full_rotation, average_latency, temp;
ValveTable table;
HANDLE valve_pipe, CPS_pipe;
TCHAR  chBuf[1024];
DWORD  cbRead, cbToWrite, cbWritten, dwMode;
LPTSTR valve_pipe_name, CPS_pipe_name;

int main() {
	// OFF:
	rpm = 600.0; // Replace this with whatever the starting value is
	signal_length = ((35.0 * 2 / 360.0) / (rpm / 60.0)) * 1000000.0; // Value in microseconds
	signal_length_offset = signal_length * 1.1;
	signal_length_3 = signal_length * 3;
	full_rotation = signal_length * 35 + signal_length * 34 + signal_length_3;
	elapsed = duration_cast<microseconds>(elapsed);
	teeth_gap_passed = i = 0;
	average_latency = 0;
	latency_count = 70;
	valve_pipe_name = TEXT("\\\\.\\pipe\\Valves");
	CPS_pipe_name = TEXT("\\\\.\\pipe\\CPS");
	cbToWrite = 4;
	while (true) {
		valve_pipe = CreateFile(valve_pipe_name, GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL); 
		CPS_pipe = CreateFile(valve_pipe_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL)
		if (valve_pipe != INVALID_HANDLE_VALUE && CPS_pipe != INVALID_HANDLE_VALUE)
			break;
	}
	teeth_gap_start = system_clock::now();
	ReadFile(CPS_pipe, chBuf, 512 * sizeof(TCHAR), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
	// VERIFY:
	while (teeth_gap_passed < 70) {
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(TCHAR), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		elapsed = system_clock::now() - teeth_gap_start;
		if (chBuf[0] != start_signal) {
			teeth_gap_start = system_clock::now();
			if (elapsed.count() > signal_length_offset) {
				gaps_locations[teeth_gap_passed] = i;
				last_big_gap = teeth_gap_start;
				i++;
			}
			teeth_gap_passed++;
			start_signal = chBuf[0];
		}
	}
	teeth_gap_passed = 0;
	while (teeth_gap_passed < 70) {
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(TCHAR), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		elapsed = system_clock::now() - teeth_gap_start;
		if (chBuf[0] != start_signal) {
			teeth_gap_start = system_clock::now();
			if (gaps_locations[teeth_gap_passed]) {
				temp = signal_length_3;
			}
			else {
				temp = signal_length;
			}
			average_latency += elapsed.count() - temp;
			latency_count++;
			teeth_gap_passed++;
			start_signal = chBuf[0];
		}
	}
	cout << "AVERAGE LATENCY DURING SETUP: " << average_latency / latency_count << endl;
	cout << "CURRENT AVERAGE LATENCY:" << endl << "\r";
	// SEND + VERIFY:
	// WILL BE REFORMATTED FOR PROPER FILE
	if (gaps_locations.size() == 0) {
		gaps_locations[0] = i;
		i++;
		last_big_gap = system_clock::now() - (microseconds(full_rotation) - microseconds(signal_length_3));
	}
	elapsed = system_clock::now() - last_big_gap;
	angle = elapsed.count() / signal_length * 360.0;
	start_angle = angle;
	teeth_gap_passed = angle / 5;
	for (size_t j = 0; j < i; j++) {
		for (auto &k : gaps_locations) {
			if (k.second == j) {
				auto node = gaps_locations.extract(k.first);
				node.key() = 70;
				gaps_locations.insert(move(node));
			}
		}
	}
	// WILL BE REFORMATTED FOR PROPER FILE
	while (true) {
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(TCHAR), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		elapsed = system_clock::now() - teeth_gap_start;
		angle = start_angle + (elapsed.count() / temp * 5.0);
		WriteFile(valve_pipe,angle,cbToWrite,&cbWritten,NULL); // SENDING angle TO CPS
		if (chBuf[0] != start_signal) {
			if (gaps_locations[teeth_gap_passed]) {
				temp = signal_length_3;
			}
			else {
				temp = signal_length;
			}
			teeth_gap_start = system_clock::now();
			rpm = (elapsed / temp) * rpm;
			signal_length = ((35.0 * 2 / 360.0) / (rpm / 60.0)) * 1000000.0;
			signal_length_3 = signal_length * 3;
			average_latency += elapsed.count() - temp;
			latency_count++;
			teeth_gap_passed = teeth_gap_passed + 1 % 71;
			start_signal = chBuf[0];
			start_angle = 5 * teeth_gap_passed;
			cout << average_latency / latency_count << "          \r";
		}
	}
}