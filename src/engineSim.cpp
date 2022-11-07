
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <ratio>
#include "table.h"

#ifdef COMPILING_FOR_RASPI_PICO
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#endif

int RPM = 6000;
int ROTATION_DEGREE = 0;

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
//void signalOut(int, bool);


int main(int ac, char** av)
{
	auto sleepTime = waitTime(720);
	//cout << sleepTime.count() << "\n";
	toneWheel toneWheel;
	cout << toneWheel.degOfSeperation << "\n" << toneWheel.missingTooth << "\n" << toneWheel.numOfTeeth;
	return 0;
}

chrono::nanoseconds waitTime(int tableEntries) {
	double RPS = RPM / 60;
	//cout << RPS << "\n" << (RPS / tableEntries) << "\n";
	chrono::nanoseconds myTime(long long(round(1000000000 / (RPS * tableEntries))));
	//cout << myTime.count() <<"\n";
	return myTime;
}
	

void CPS(toneWheel tw, chrono::nanoseconds sleepTime) {
	int tooth = 1;
	bool on = true;
	while (true) {
		for (; ROTATION_DEGREE < 720; ROTATION_DEGREE++) {
			if (ROTATION_DEGREE % tw.degOfSeperation == tw.degOfSeperation / 2 && !on) {
				tooth++;
				on = true;

			}
			if (ROTATION_DEGREE % tw.degOfSeperation != tw.degOfSeperation / 2 && on) {
				on = false;
				if (tooth == tw.missingTooth) tooth = 0;
			}
			if (on && (tooth != tw.missingTooth) {
				//send signal
			}
			else {
				//stop signal
			}

		}
	}
}

//void signlOut(int port,bool on) {
//	return;
//}
