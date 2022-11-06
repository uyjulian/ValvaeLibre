
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <ratio>
#include "table.h"

int RPM = 6000;

using namespace std;

chrono::nanoseconds waitTime(int);
void CPS(toneWheel, chrono::nanoseconds);
void signalOut(int, bool);

struct toneWheel
{
	int numOfTeeth = 35;		
	int degOfSeperation = 10;	//from leading edge to leading edge
	int missingTooth = 36;		//expand to multiple later

};


int main(int ac, char** av)
{
	auto sleepTime = waitTime(720);
	//cout << sleepTime.count() << "\n";
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
