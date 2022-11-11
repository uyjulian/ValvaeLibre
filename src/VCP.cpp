#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <cmath>
#include <unordered_map>
using namespace std;
using namespace std::chrono;

int teeth_passed, gaps_passed;
size_t i;
unordered_map<int,int> gaps_locations;
unordered_map<int,time_point<system_clock>> gaps_locations_times;
bool signal_in, start_signal;
time_point<system_clock> universal_start, teeth_gap_start, guess;
duration<double> elapsed;
double rpm;
long long signal_length, signal_length_offset, signal_length_2, full_rotation, angle, average_latency;

int main() {
off:
	signal_length = ((35.0 * 2.0 / 360.0) / (rpm / 60.0)) * 1000000.0; // Value in microseconds
	signal_length_offset = signal_length + signal_length * 0.1;
	signal_length_2 = signal_length * 2;
	full_rotation = signal_length * 35 + signal_length * 34 + signal_length_2;
	universal_start = system_clock::now();
	elapsed = duration_cast<microseconds>(elapsed);
	teeth_passed = gaps_passed = i = 0;
	rpm = 600.0; // Replace this with whatever the starting value is
	average_latency = 0;
	while (true) {

	}
verify: // Receives start_signal and teeth_gap_start from off
	while (teeth_passed < 35 && gaps_passed < 35) {
		signal_in = true; // REPLACE THIS WITH READING FROM ACTUAL PIPE
		elapsed = system_clock::now() - teeth_gap_start;
		if (signal_in != start_signal) {
			teeth_gap_start = system_clock::now();
			if (elapsed.count() > signal_length_offset) {
				gaps_locations[i] = gaps_passed;
				gaps_locations_times[i] = teeth_gap_start;
				i++;
			}
			if (start_signal) {
				teeth_passed++;
			}
			else {
				gaps_passed++;
			}
			start_signal = signal_in;
		}
	}
	gaps_passed = teeth_passed = 0;
	while (teeth_passed < 35 && gaps_passed < 35) {
		signal_in = true; // REPLACE THIS WITH READING FROM ACTUAL PIPE
		elapsed = system_clock::now() - teeth_gap_start;
		if (signal_in != start_signal) {
			teeth_gap_start = system_clock::now();
			if (gaps_locations[gaps_passed]) {
				average_latency += elapsed.count() - signal_length_2;
			}
			else {
				average_latency += elapsed.count() - signal_length;
			}
			if (start_signal) {
				teeth_passed++;
			}
			else {
				gaps_passed++;
			}
			start_signal = signal_in;
		}
	}
send_verify:
}