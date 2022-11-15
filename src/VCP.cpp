#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <cmath>
#include <set>

#include "table.h"
#ifdef _WIN32
#include <windows.h> 
#else

#endif

using namespace std;
using namespace std::chrono;

set<int> gaps_locations;
bool start_signal;
time_point<system_clock> teeth_gap_start, guess, last_big_gap;
duration<double, ratio<1, 1000000>> elapsed;
double rpm, temp_2;
long long average_latency, latency_count, teeth_gap_passed, angle, start_angle, signal_length, signal_length_2, signal_length_3, full_rotation, temp, temp_3;
ValveTable table;
#ifdef _WIN32
HANDLE valve_pipe, CPS_pipe;
bool  chBuf[512];
DWORD  cbRead, cbToWrite, cbWritten, dwMode;
LPTSTR valve_pipe_name, CPS_pipe_name;
#else
#endif

int main() {
	// OFF:
	rpm = 60.0; // Replace this with whatever the starting value is
	signal_length = ((1.0 / (36.0 * 2.0)) / (rpm / 60.0)) * 1000000.0; // Value in microseconds
	signal_length_2 = signal_length * 2;
	signal_length_3 = signal_length * 3;
	full_rotation = signal_length * 35 + signal_length * 34 + signal_length_3;
	teeth_gap_passed = 0;
	average_latency = 0;
	latency_count = 0;
	temp_3 = 5;
#if _WIN32
	valve_pipe_name = TEXT("\\\\.\\pipe\\Valves");
	CPS_pipe_name = TEXT("\\\\.\\pipe\\CPS");
	cbToWrite = 4;
#else
#endif
	while (true) {
#if _WIN32
		valve_pipe = CreateFile(valve_pipe_name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		CPS_pipe = CreateFile(CPS_pipe_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (valve_pipe != INVALID_HANDLE_VALUE && CPS_pipe != INVALID_HANDLE_VALUE) {
			break;
		}
#else
#endif
	}
	teeth_gap_start = system_clock::now();
#if _WIN32
	ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
	start_signal = chBuf[0];
#else
#endif
	// VERIFY
	while (teeth_gap_passed < 70) {
#if _WIN32
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		if (chBuf[0] != start_signal) {
			elapsed = system_clock::now() - teeth_gap_start;
			teeth_gap_start = system_clock::now();
			if (elapsed.count() > signal_length_2) {
				gaps_locations.insert(teeth_gap_passed);
			}
			teeth_gap_passed++;
			start_signal = chBuf[0];
		}
#else
#endif
	}
	// NO GAPS FOUND
	if (gaps_locations.size() == 0) {
		gaps_locations.insert(0);
		last_big_gap = system_clock::now() - (microseconds(full_rotation) - microseconds(signal_length_3));
	}
	// NO GAPS FOUND
	teeth_gap_passed = 0;
	teeth_gap_start = system_clock::now();
	while (teeth_gap_passed < 70) {
#if _WIN32
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		if (chBuf[0] != start_signal) {
			elapsed = system_clock::now() - teeth_gap_start;
			teeth_gap_start = system_clock::now();
			teeth_gap_passed++;
			average_latency += elapsed.count() - temp;
			latency_count++;
			cout << "LAT = " << elapsed.count() - temp;
			start_signal = chBuf[0];
			if (gaps_locations.find(teeth_gap_passed) != gaps_locations.end()) {
				temp = signal_length_3;
				last_big_gap = teeth_gap_start;
			}
			else {
				temp = signal_length;
			}
		}
#else
#endif
	}
	cout << "AVERAGE LATENCY DURING SETUP: " << average_latency / latency_count << endl;
	cout << "CURRENT AVERAGE LATENCY | CURRENT SENDING:" << endl << "\r";
	// SEND + VERIFY:
	// WILL BE REFORMATTED FOR PROPER FILE
	for (auto& element : gaps_locations) {
		auto hold = element;
		gaps_locations.erase(element);
		gaps_locations.insert(69);
	}
	elapsed = system_clock::now() - last_big_gap;
	teeth_gap_passed = ((elapsed.count() - signal_length_3) / signal_length);
	start_angle = teeth_gap_passed * 5;
	// WILL BE REFORMATTED FOR PROPER FILE
	while (true) {
#if _WIN32
		elapsed = system_clock::now() - teeth_gap_start;
		angle = (long long)round(start_angle + (((elapsed.count() / temp)) * temp_3)) % 720;
		//cout << "START RPM: " << rpm << " LATENCY: " << temp_2 << " ANGLE: " << angle << " TEETH PASSED: " << teeth_gap_passed << " buf: " << chBuf[0] << endl;
		cout << average_latency / latency_count << "          | " << angle << "          \r";
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		WriteFile(valve_pipe, &angle, cbToWrite, &cbWritten, NULL); // SENDING angle TO CPS
		if (chBuf[0] != start_signal) {
			elapsed = system_clock::now() - teeth_gap_start;
			teeth_gap_start = system_clock::now();
			start_angle = (start_angle + temp_3) % 720;
			angle = start_angle;
			temp_2 = elapsed.count() - temp;
			average_latency += temp_2;
			latency_count++;
			teeth_gap_passed = (teeth_gap_passed + 1) % 70;
			if (-1000 > temp_2 || temp_2 > 1000) {
				rpm *= (temp_2 / temp) + 1;
				signal_length = ((1.0 / (36.0 * 2.0)) / (rpm / 60.0)) * 1000000.0;
				signal_length_3 = signal_length * 3;
			}
			if (gaps_locations.find(teeth_gap_passed) != gaps_locations.end()) {
				temp = signal_length_3;
				temp_3 = 15;
			}
			else {
				temp = signal_length;
				temp_3 = 5;
			}
			start_signal = chBuf[0];
		}
#else
#endif
	}
}