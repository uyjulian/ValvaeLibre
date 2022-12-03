
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <chrono>
#include <ratio>
#include <cmath>



#ifdef COMPILING_FOR_RASPI_PICO
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/multicore.h"
#endif

struct toneWheel
{
	int numOfTeeth = 35;
	int degOfSeperation = 10;	//from leading edge to leading edge
	int missingTooth = 36;		//expand to multiple later

};

int RPM = 6;
int ROTATION_DEGREE = 0;
bool SIGNAL = true;
std::chrono::time_point<std::chrono::steady_clock> ENG_TICK_CLOCK = std::chrono::steady_clock::now();
#ifdef COMPILING_FOR_RASPI_PICO
uint LED_PIN = PICO_DEFAULT_LED_PIN;
uint CPS_PIN = 27;
#endif // COMPILING_FOR_RASPI_PICO


#ifdef COMPILING_FOR_RASPI_PICO
uint64_t waitTime(int);
void CPS(toneWheel, uint64_t);
#else
std::chrono::microseconds waitTime(int);
void CPS(toneWheel, std::chrono::microseconds);
#endif

void sendSignal();

int main()
{
#ifdef COMPILING_FOR_RASPI_PICO
	gpio_init(CPS_PIN);
	gpio_init(LED_PIN);
	gpio_set_dir(CPS_PIN, 1);
	gpio_set_dir(LED_PIN, 1);
	gpio_put(LED_PIN, 1);
    uint64_t sleepTime = waitTime(720);
#else
	std::chrono::microseconds sleepTime = waitTime(720);
#endif
	toneWheel toneWheel;
	CPS(toneWheel, sleepTime);
	return 0;
}

#ifdef COMPILING_FOR_RASPI_PICO
uint64_t waitTime(int tableEntries) {
	double RPS = RPM / 60.0;
	//std::chrono::microseconds myTime((long long)round(1000000 / ((RPS / 2) * tableEntries)));
	uint64_t myTime = round(1000000 / ((RPS / 2) * tableEntries));
	return myTime;
}
#else
std::chrono::microseconds waitTime(int tableEntries) {
	double RPS = RPM / 60.0;
	std::chrono::microseconds myTime((long long)round(1000000 / ((RPS / 2) * tableEntries)));
	//uint64_t myTime = round(1000000 / ((RPS / 2) * tableEntries));
	return myTime;
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
void CPS(toneWheel tw, uint64_t sleepTime) {
	while (true) {
		bool on = true;
		int tooth = 1;
		ROTATION_DEGREE = 0;
		for (; ROTATION_DEGREE < 720; ++ROTATION_DEGREE) {
			ENG_TICK_CLOCK = std::chrono::steady_clock::now();

			if (ROTATION_DEGREE % tw.degOfSeperation < tw.degOfSeperation / 2) {
				if (!on) tooth++;
				on = true;

			}
			else {
				on = false;
				if (tooth == tw.missingTooth) tooth = 0;

			}

			if (on && (tooth != tw.missingTooth)) {
				SIGNAL = true;
			}
			else {
				SIGNAL = false;
			}
			// chrono::time_point endLoop = chrono::steady_clock::now();
			busy_wait_us(sleepTime);

			sendSignal();

			if (ROTATION_DEGREE == 360) tooth = 1;
			// if (DEBUG) {
			// 	cout << setw(4) << ROTATION_DEGREE
			// 		<< "  Tooth " << setw(3) << tooth
			// 		<< "  Signal " << SIGNAL
			// 		<< "  Time Stamp "
			// 		<< chrono::duration_cast<chrono::microseconds>(ENG_TICK_CLOCK.time_since_epoch()).count()
			// 		<< "  Loop time " << chrono::duration_cast<chrono::microseconds>(endLoop.time_since_epoch() - ENG_TICK_CLOCK.time_since_epoch()).count()
			// 		<< "  Sleep time " << setw(5) << chrono::duration_cast<chrono::microseconds>(timeSlept - endLoop).count()
			// 		<< "  Write time " << chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - ENG_TICK_CLOCK).count()
			// 		<< " \n";
			// }
			//if (ROTATION_DEGREE == 360) return;
		}
	}
}
#else
void CPS(toneWheel tw, std::chrono::microseconds sleepTime) {
	while (true) {
		bool on = true;
		int tooth = 1;
		ROTATION_DEGREE = 0;
		for (; ROTATION_DEGREE < 720; ++ROTATION_DEGREE) {
			ENG_TICK_CLOCK = std::chrono::steady_clock::now();

			if (ROTATION_DEGREE % tw.degOfSeperation < tw.degOfSeperation / 2) {
				if (!on) tooth++;
				on = true;

			}
			else {
				on = false;
				if (tooth == tw.missingTooth) tooth = 0;

			}

			if (on && (tooth != tw.missingTooth)) {
				SIGNAL = true;
			}
			else {
				SIGNAL = false;
			}
			// chrono::time_point endLoop = chrono::steady_clock::now();
			bool sleep = true;
			while (sleep) {
				std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();
				// std::chrono::time_point waitTill = ENG_TICK_CLOCK + sleepTime;
				auto waitTill = ENG_TICK_CLOCK + sleepTime;
				//cout << chrono::duration_cast<chrono::nanoseconds>(time.time_since_epoch()).count()
				//	<< "     "
				//	<< chrono::duration_cast<chrono::nanoseconds>(waitTill.time_since_epoch()).count()
				//	<< "\n";
				sleep = time < waitTill;
			}

			sendSignal();

			if (ROTATION_DEGREE == 360) tooth = 1;
			// if (DEBUG) {
			// 	cout << setw(4) << ROTATION_DEGREE
			// 		<< "  Tooth " << setw(3) << tooth
			// 		<< "  Signal " << SIGNAL
			// 		<< "  Time Stamp "
			// 		<< chrono::duration_cast<chrono::microseconds>(ENG_TICK_CLOCK.time_since_epoch()).count()
			// 		<< "  Loop time " << chrono::duration_cast<chrono::microseconds>(endLoop.time_since_epoch() - ENG_TICK_CLOCK.time_since_epoch()).count()
			// 		<< "  Sleep time " << setw(5) << chrono::duration_cast<chrono::microseconds>(timeSlept - endLoop).count()
			// 		<< "  Write time " << chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - ENG_TICK_CLOCK).count()
			// 		<< " \n";
			// }
			//if (ROTATION_DEGREE == 360) return;
		}
	}
}
#endif

void sendSignal() {
#ifdef COMPILING_FOR_RASPI_PICO
	gpio_put(CPS_PIN, SIGNAL);
	gpio_put(LED_PIN, SIGNAL);
#endif // COMPILING_FOR_RASPI_PICO


	
}