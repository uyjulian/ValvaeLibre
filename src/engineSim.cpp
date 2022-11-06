
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "table.h"

int RPM = 60;

using namespace std;

int waitTime(int);
void CPS(toneWheel, int);
void signalOut(int, bool);

struct toneWheel
{
	int numOfTeeth = 35;
	int degOfSeperation = 10;
	int missingTooth = 36;

};

int main(int ac, char** av)
{
	int sleepTime = waitTime(720);
	//cout << sleepTime;
}

int waitTime(int tableEntries) {
	double RPS = RPM / 60;
	int nanoSleep = round((RPS / tableEntries) * 1000000000);
	return nanoSleep;
}

void CPS(toneWheel tw, int sleepTime) {
	while (true) {
		int tooth = 1;
		for (int i = 0; i < 720; i++) {
			if (i % tw.degOfSeperation < (tw.degOfSeperation / 2)) signalOut(55000, true);
			else signalOut(55000, false);
		}
	}
}

void signlOut(int port,bool on) {
	
}
