#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <table.h>
using namespace std;
using namespace std::chrono;

size_t i, latency_count, teeth_gap_passed;
unordered_map<int,int> gaps_locations;
bool signal_in, start_signal;
time_point<system_clock> teeth_gap_start, guess, last_big_gap;
duration<double> elapsed;
double rpm;
long long signal_length, signal_length_offset, signal_length_3, full_rotation, angle, start_angle, average_latency, temp;
ValveTable table;

int main() {
	// OFF:
	rpm = 600.0;
	signal_length = ((35.0 * 2 / 360.0) / (rpm / 60.0)) * 1000000.0; // Value in microseconds
	signal_length_offset = signal_length + signal_length * 0.1;
	signal_length_3 = signal_length * 3;
	full_rotation = signal_length * 35 + signal_length * 34 + signal_length_3;
	elapsed = duration_cast<microseconds>(elapsed);
	teeth_gap_passed = i = 0; // Replace this with whatever the starting value is
	average_latency = 0;
	latency_count = 70;
	while (true) {

	}
	// VERIFY:
	// Receives start_signal and teeth_gap_start from off
	while (teeth_gap_passed < 70) {
		signal_in = true; // REPLACE THIS WITH READING FROM ACTUAL PIPE
		elapsed = system_clock::now() - teeth_gap_start;
		if (signal_in != start_signal) {
			teeth_gap_start = system_clock::now();
			if (elapsed.count() > signal_length_offset) {
				gaps_locations[teeth_gap_passed] = i;
				last_big_gap = teeth_gap_start;
				i++;
			}
			teeth_gap_passed++;
			start_signal = signal_in;
		}
	}
	teeth_gap_passed = 0;
	while (teeth_gap_passed < 70) {
		signal_in = true; // REPLACE THIS WITH READING FROM ACTUAL PIPE
		elapsed = system_clock::now() - teeth_gap_start;
		if (signal_in != start_signal) {
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
			start_signal = signal_in;
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
		signal_in = true; // REPLACE THIS WITH READING FROM ACTUAL PIPE
		elapsed = system_clock::now() - teeth_gap_start;
		angle = start_angle + (elapsed.count() / temp * 5.0);
		// SEND table[angle] TO PIPE
		if (signal_in != start_signal) {
			if (gaps_locations[teeth_gap_passed]) {
				temp = signal_length_3;
			}
			else {
				temp = signal_length;
			}
			rpm = (elapsed / temp) * rpm;
			signal_length = ((35.0 * 2 / 360.0) / (rpm / 60.0)) * 1000000.0;
			signal_length_3 = signal_length * 3;
			average_latency += elapsed.count() - temp;
			latency_count++;
			teeth_gap_start = system_clock::now();
			teeth_gap_passed = teeth_gap_passed + 1 % 71;
			start_signal = signal_in;
			start_angle = 5 * teeth_gap_passed;
			cout << average_latency / latency_count << "          \r";
		}
	}
}