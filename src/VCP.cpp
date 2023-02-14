#include <stdlib.h>
#include <chrono>
#include <cmath>
#include <set>

#include "table.h"
#ifdef _WIN32
#include <windows.h> 
#include <iostream>
#include <stdlib.h>
#elif defined(COMPILING_FOR_RASPI_PICO)
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#endif

using namespace std;
using namespace std::chrono;

set<short> gaps_locations;
bool start_signal;
time_point<steady_clock> teeth_gap_start, guess, last_big_gap;
#ifdef _WIN32
duration<long long int, ratio<1, 1000000000>> elapsed;
long long signal_length, signal_length_2, signal_length_3, signal_length_6, signal_length_3_2, signal_length_1_2, full_rotation, temp, temp_2, temp_3, rpm, teeth_gap_passed, angle, start_angle;
HANDLE valve_pipe, CPS_pipe;
long long average_latency, latency_count;
bool  chBuf[512];
DWORD  cbRead, cbToWrite, cbWritten, dwMode;
LPTSTR valve_pipe_name, CPS_pipe_name;
#elif defined(COMPILING_FOR_RASPI_PICO)
duration<int, ratio<1, 1000000000>> elapsed;
int signal_length, signal_length_2, signal_length_3, signal_length_6, signal_length_3_2, signal_length_1_2, full_rotation, temp, temp_2, temp_3, rpm, teeth_gap_passed, angle, start_angle, late_count;
bool volatile chBuf;
int ignore_bit;
void blink_error() {
	while (true) {
		gpio_put(PICO_DEFAULT_LED_PIN, 1);
		sleep_ms(2000);
		gpio_put(PICO_DEFAULT_LED_PIN, 0);
		sleep_ms(2000);
	}
}
#endif

int main() {
	// OFF:
#if defined(_WIN32) || defined(COMPILING_FOR_RASPI_PICO)
	rpm = 6; // Replace this with whatever the starting value is
	signal_length = ((1.0 / (36.0 * 2.0)) / (rpm / 60.0)) * 1000000000.0; // Value in nanoseconds
	signal_length_2 = signal_length * 2;
	signal_length_3 = signal_length * 3;
	signal_length_1_2 = signal_length * 0.5;
	signal_length_3_2 = signal_length_3 * 0.5;
	signal_length_6 = signal_length_3 * 2;
	full_rotation = signal_length * 35 + signal_length * 34 + signal_length_3;
	teeth_gap_passed = 0;
	temp_3 = 5;
#endif
#if _WIN32
	average_latency = 0;
	latency_count = 0;
	valve_pipe_name = TEXT("\\\\.\\pipe\\Valves");
	CPS_pipe_name = TEXT("\\\\.\\pipe\\CPS");
	cbToWrite = 4;
#elif defined(COMPILING_FOR_RASPI_PICO)
	stdio_init_all();
	gpio_init(0);
	gpio_set_dir(0, GPIO_OUT);
	gpio_init(1);
	gpio_set_dir(1, GPIO_OUT);
	gpio_init(2);
	gpio_set_dir(2, GPIO_OUT);
	gpio_init(3);
	gpio_set_dir(3, GPIO_OUT);
	gpio_init(16);
	gpio_set_dir(16, GPIO_OUT);
	gpio_init(17);
	gpio_set_dir(17, GPIO_OUT);
	gpio_init(18);
	gpio_set_dir(18, GPIO_OUT);
	gpio_init(19);
	gpio_set_dir(19, GPIO_OUT);
	gpio_init(28);
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
	chBuf = false;
	ignore_bit = ~(1 << 28) & ~(1 << PICO_DEFAULT_LED_PIN);
	gpio_set_dir(28, GPIO_IN);
	late_count = 0;
#endif
	while (true) {
#if _WIN32
		valve_pipe = CreateFile(valve_pipe_name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		CPS_pipe = CreateFile(CPS_pipe_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (valve_pipe != INVALID_HANDLE_VALUE && CPS_pipe != INVALID_HANDLE_VALUE) {
			break;
		}
#elif defined(COMPILING_FOR_RASPI_PICO)
		chBuf = gpio_get(28);
		if (chBuf) {
			break;
		}
#endif
	}
	teeth_gap_start = steady_clock::now();
#if _WIN32
	ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
	start_signal = chBuf[0];
#elif defined(COMPILING_FOR_RASPI_PICO)
	chBuf = gpio_get(28);
	start_signal = chBuf;
#endif
	// VERIFY
#if defined(_WIN32) || defined(COMPILING_FOR_RASPI_PICO)
	while (teeth_gap_passed < 70)
#endif
	{
#if _WIN32
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		if (chBuf[0] != start_signal) {
			elapsed = steady_clock::now() - teeth_gap_start;
			teeth_gap_start = steady_clock::now();
			if (elapsed.count() > signal_length_2) {
				gaps_locations.insert(teeth_gap_passed);
			}
			teeth_gap_passed++;
			start_signal = chBuf[0];
		}
#elif defined(COMPILING_FOR_RASPI_PICO)
		chBuf = gpio_get(28);
		if (chBuf != start_signal) {
			elapsed = steady_clock::now() - teeth_gap_start;
			teeth_gap_start = steady_clock::now();
			if (elapsed.count() > signal_length_2) {
				gaps_locations.insert(teeth_gap_passed);
			}
			teeth_gap_passed++;
			start_signal = chBuf;
		}
#endif
	}
#if defined(_WIN32) || defined(COMPILING_FOR_RASPI_PICO)
	// NO GAPS FOUND
	if (gaps_locations.size() == 0) {
		gaps_locations.insert(0);
		last_big_gap = steady_clock::now() - (nanoseconds(full_rotation) - nanoseconds(signal_length_3));
	}
	// NO GAPS FOUND
	teeth_gap_passed = 0;
	teeth_gap_start = steady_clock::now();
#endif
#ifdef _WIN32
	average_latency -= signal_length;
#endif
#if defined(_WIN32) || defined(COMPILING_FOR_RASPI_PICO)
	while (teeth_gap_passed < 70)
#endif
	{
#if _WIN32
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		if (chBuf[0] != start_signal) {
			elapsed = steady_clock::now() - teeth_gap_start;
			teeth_gap_start = steady_clock::now();
			teeth_gap_passed++;
			average_latency += elapsed.count() - temp;
			latency_count++;
			start_signal = chBuf[0];
			if (gaps_locations.find(teeth_gap_passed) != gaps_locations.end()) {
				temp = signal_length_3;
				last_big_gap = teeth_gap_start;
			}
			else {
				temp = signal_length;
			}
		}
#elif defined(COMPILING_FOR_RASPI_PICO)
		chBuf = gpio_get(28);
		if (chBuf != start_signal) {
			elapsed = steady_clock::now() - teeth_gap_start;
			teeth_gap_start = steady_clock::now();
			teeth_gap_passed++;
			start_signal = chBuf;
			if (gaps_locations.find(teeth_gap_passed) != gaps_locations.end()) {
				temp = signal_length_3;
				last_big_gap = teeth_gap_start;
			}
			else {
				temp = signal_length;
			}
		}
#endif
	}
#ifdef _WIN32
	std::cout << "AVERAGE LATENCY DURING SETUP: " << average_latency / latency_count << endl;
	std::cout << "CURRENT AVERAGE LATENCY | CURRENT SENDING:" << endl << "\r";
#endif
	// SEND + VERIFY:
	// WILL BE REFORMATTED FOR PROPER FILE
	gaps_locations.erase(gaps_locations.begin());
	gaps_locations.insert(69);
#if defined(_WIN32) || defined(COMPILING_FOR_RASPI_PICO)
	elapsed = steady_clock::now() - last_big_gap;
	teeth_gap_passed = ((elapsed.count() - signal_length_3) / signal_length);
	start_angle = teeth_gap_passed * 5;
#endif
	// WILL BE REFORMATTED FOR PROPER FILE
	while (true) {
#if _WIN32
		elapsed = steady_clock::now() - teeth_gap_start;
		angle = (long long)(start_angle + (((((double)elapsed.count() / (double)temp))) * temp_3)) % 720;
		//cout << "START RPM: " << rpm << " LATENCY: " << temp_2 << " ANGLE: " << angle << " TEETH PASSED: " << teeth_gap_passed << " buf: " << chBuf[0] << endl;
		std::cout << average_latency / latency_count << "          | " << angle << "          \r";
		ReadFile(CPS_pipe, chBuf, 512 * sizeof(bool), &cbRead, NULL); // READ FROM PIPE. chBuf[0] EQUALS SIGNAL RECEIVED
		WriteFile(valve_pipe, &angle, cbToWrite, &cbWritten, NULL); // SENDING angle TO CPS
		if (chBuf[0] != start_signal) {
			elapsed = steady_clock::now() - teeth_gap_start;
			teeth_gap_start = steady_clock::now();
			start_angle = (start_angle + temp_3) % 720;
			angle = start_angle;
			temp_2 = elapsed.count() - temp;
			average_latency += temp_2;
			latency_count++;
			teeth_gap_passed = (teeth_gap_passed + 1) % 70;
			if (-1000000 > temp_2 || temp_2 > 1000000) {
				rpm *= ((double)temp / (double)elapsed.count());
				signal_length = ((1.0 / (36.0 * 2.0)) / (rpm / 60.0)) * 1000000000.0;
				signal_length_3 = signal_length * 3;
				signal_length_1_2 = signal_length / 2;
				signal_length_3_2 = signal_length_3 / 2;
				signal_length_2 = signal_length * 2;
				signal_length_6 = signal_length_3 * 2;

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
#elif defined(COMPILING_FOR_RASPI_PICO)
		elapsed = steady_clock::now() - teeth_gap_start;
		angle = (int)(start_angle + (((((float)elapsed.count() / (float)temp))) * temp_3)) % 720;
		chBuf = gpio_get(28);
		gpio_put_all(test_table.table[angle]);
		if (chBuf != start_signal) {
			elapsed = steady_clock::now() - teeth_gap_start;
			teeth_gap_start = steady_clock::now();
			start_angle = (start_angle + temp_3) % 720;
			angle = start_angle;
			gpio_put_all(test_table.table[angle]);
			temp_2 = elapsed.count() - temp;
			/*
			if (temp = signal_length_3) {
				if (elapsed.count() < signal_length_3_2) {
					blink_error();
				}
			}
			else {
				if (elapsed.count() < signal_length_1_2) {
					blink_error();
				}
			}
			*/
			teeth_gap_passed = (teeth_gap_passed + 1) % 70;
			/*
			if (-1000000 > temp_2 || temp_2 > 1000000) {
				rpm *= ((float)temp / (float)elapsed.count());
				signal_length = ((1.0 / (36.0 * 2.0)) / (rpm / 60.0)) * 1000000000.0;
				signal_length_3 = signal_length * 3;
				signal_length_1_2 = signal_length / 2;
				signal_length_3_2 = signal_length_3 / 2;
				signal_length_2 = signal_length * 2;
				signal_length_6 = signal_length_3 * 2;

			}
			*/
			if (gaps_locations.find(teeth_gap_passed) != gaps_locations.end()) {
				temp = signal_length_3;
				temp_3 = 15;
			}
			else {
				temp = signal_length;
				temp_3 = 5;
			}
			start_signal = chBuf;
		}
#endif
	}
}
