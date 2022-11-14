
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <ratio>
#include "table.h"

#ifdef COMPILING_FOR_RASPI_PICO
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#endif

int RPM = 60;
int ROTATION_DEGREE = 0;
bool SIGNAL = true;
bool DEBUG = false;
std::chrono::time_point ENG_TICK_CLOCK = std::chrono::steady_clock::now();

HANDLE cpsPipe = INVALID_HANDLE_VALUE;
HANDLE valvePipe = INVALID_HANDLE_VALUE;

using namespace std;

//chrono::nanoseconds waitTime(int);
//void CPS(toneWheel, chrono::nanoseconds);
//void signalOut(int, bool);

struct toneWheel
{
	int numOfTeeth = 35;
	int degOfSeperation = 10;	//from leading edge to leading edge
	int missingTooth = 36;		//expand to multiple later

};

chrono::nanoseconds waitTime(int);
void CPS(toneWheel, chrono::nanoseconds);
void winConnect();

int main(int ac, char** av)
{
	winConnect();
	auto sleepTime = waitTime(720);
	//cout << sleepTime.count() << "\n";
	toneWheel toneWheel;
	//cout << toneWheel.degOfSeperation << "\n" << toneWheel.missingTooth << "\n" << toneWheel.numOfTeeth;
	//CPS(toneWheel, sleepTime);
	thread CPS(CPS, toneWheel, sleepTime);
	CPS.detach();

	fprintf(stdout, "%s\n", "Hello World from Amazing program 3!");
	return 0;
}

chrono::nanoseconds waitTime(int tableEntries) {
	double RPS = RPM / 60;
	//cout << RPS << "\n" << (RPS / tableEntries) << "\n";
	chrono::nanoseconds myTime(long long(round(1000000000 / ((RPS / 2) * tableEntries))));
	cout << myTime.count() << "\n";
	return myTime;
}


void CPS(toneWheel tw, chrono::nanoseconds sleepTime) {
	bool on = true;
	while (true) {
		int tooth = 1;
		ROTATION_DEGREE = 0;
		for (; ROTATION_DEGREE < 720; ++ROTATION_DEGREE) {
			ENG_TICK_CLOCK = chrono::steady_clock::now();

			if (ROTATION_DEGREE % tw.degOfSeperation < tw.degOfSeperation / 2) {
				if (!on) tooth++;
				on = true;

			}
			else {
				on = false;
				if (tooth == tw.missingTooth) tooth = 0;

			}
			//if (ROTATION_DEGREE % tw.degOfSeperation != tw.degOfSeperation / 2 && on) {
			//	on = false;
			//	if (tooth == tw.missingTooth) tooth = 0;
			//}
			if (on && (tooth != tw.missingTooth)) {
				SIGNAL = true;
			}
			else {
				SIGNAL = false;
			}
			chrono::time_point endLoop = chrono::steady_clock::now();
			//this_thread::sleep_for(chrono::microseconds(1));

			bool sleep = true;
			while (sleep) {
				chrono::time_point time = chrono::steady_clock::now();
				chrono::time_point waitTill = ENG_TICK_CLOCK + sleepTime;
				//cout << chrono::duration_cast<chrono::nanoseconds>(time.time_since_epoch()).count()
				//	<< "     "
				//	<< chrono::duration_cast<chrono::nanoseconds>(waitTill.time_since_epoch()).count()
				//	<< "\n";
				sleep = time < waitTill;
			}
			chrono::time_point timeSlept = chrono::steady_clock::now();

			if (ROTATION_DEGREE == 360) tooth = 1;
			if (DEBUG) {
				cout << setw(4) << ROTATION_DEGREE
					<< "  Tooth " << setw(3) << tooth
					<< "  Signal " << SIGNAL
					<< "  Time Stamp "
					<< chrono::duration_cast<chrono::microseconds>(ENG_TICK_CLOCK.time_since_epoch()).count()
					<< "  Loop time " << chrono::duration_cast<chrono::microseconds>(endLoop.time_since_epoch() - ENG_TICK_CLOCK.time_since_epoch()).count()
					<< "  Sleep time " << setw(5) << chrono::duration_cast<chrono::microseconds>(timeSlept - endLoop).count()
					<< "  Write time " << chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - ENG_TICK_CLOCK).count()
					<< " \n";
			}
			//if (ROTATION_DEGREE == 360) return;
		}
	}
}

void winConnect() {
	LPTSTR CPSPipeName = TEXT("\\\\.\\pipe\\CPS");
	LPTSTR valvePipeName = TEXT("\\\\.\\pipe\\Valves");

	cpsPipe = CreateNamedPipe(
		CPSPipeName,
		PIPE_ACCESS_OUTBOUND,
		PIPE_TYPE_BYTE |
		PIPE
	)

}