
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "table.h"

int RPM = 60;

using namespace std;

int waitTime(int);

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
