#if defined(_WIN32) && defined(_MSC_VER)
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <ratio>
#include "table.h"

#define BUFFSIZE 512
#define TIMEOUT 20000

#ifdef COMPILING_FOR_RASPI_PICO
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#endif

int RPM = 60;
int ROTATION_DEGREE = 0;
bool SIGNAL = true;
bool DEBUG = true;
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
void testing();
void sendSignal();

int main(int ac, char** av)
{
	//thread connectPipes(winConnect);
	auto sleepTime = waitTime(720);
	//cout << sleepTime.count() << "\n";
	toneWheel toneWheel;
	//cout << toneWheel.degOfSeperation << "\n" << toneWheel.missingTooth << "\n" << toneWheel.numOfTeeth;
	CPS(toneWheel, sleepTime);
	//connectPipes.join();

	//thread crankSensor(CPS, toneWheel, sleepTime);
	//crankSensor.detach();

	//testing();

	return 0;
}

chrono::nanoseconds waitTime(int tableEntries) {
	double RPS = RPM / 60;
	chrono::nanoseconds myTime((long long)round(1000000000 / ((RPS / 2) * tableEntries)));
	cout << myTime.count() << "\n";
	return myTime;
}


void CPS(toneWheel tw, chrono::nanoseconds sleepTime) {
	while (true) {
		bool on = true;
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
			if (on && (tooth != tw.missingTooth)) {
				SIGNAL = true;
			}
			else {
				SIGNAL = false;
			}
			chrono::time_point endLoop = chrono::steady_clock::now();

			bool sleep = true;
			while (sleep) {
				chrono::time_point time = chrono::steady_clock::now();
				chrono::time_point waitTill = ENG_TICK_CLOCK + sleepTime;
				sleep = time < waitTill;
			}
			chrono::time_point timeSlept = chrono::steady_clock::now();

			if (!DEBUG)sendSignal();

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
		PIPE_TYPE_BYTE,
		1,
		BUFFSIZE,
		BUFFSIZE,
		TIMEOUT,
		NULL
	);

	if (cpsPipe == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
		return;
	}

	valvePipe = CreateNamedPipe(
		valvePipeName,
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_BYTE,
		1,
		BUFFSIZE,
		BUFFSIZE,
		TIMEOUT,
		NULL
	);

	if (valvePipe == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
		return;
	}

	if (ConnectNamedPipe(cpsPipe, NULL)) printf("CPS connected!");
	else _tprintf(TEXT("CPS connection failed, GLE=%d.\n"), GetLastError());

	if (ConnectNamedPipe(valvePipe, NULL)) printf("Valves connected!");
	else _tprintf(TEXT("Valves connection failed, GLE=%d.\n"), GetLastError());


}

void testing()
{
	HANDLE hHeap = GetProcessHeap();
	BOOL* cpsBuff = (BOOL*)HeapAlloc(hHeap, 0, BUFFSIZE * sizeof(BOOL));
	int* valveBuff = (int*)HeapAlloc(hHeap, 0, BUFFSIZE * sizeof(int));

	DWORD bytesRead = 0, bytesToWrite = 0, bytesWritten = 0;
	BOOL success = FALSE;
	int valveData = -1;
	chrono::time_point valveTimeStamp = chrono::steady_clock::now();
	long long avgVar = 0;
	long tick = 0;

	while (true) {
		
		success = ReadFile(
			valvePipe,
			valveBuff,
			BUFFSIZE * sizeof(int),
			&bytesRead,
			NULL);

		if ((success && bytesRead == 0) || valveBuff == nullptr) {
			_tprintf(TEXT("Nothing to read.\n"));
		}
		else {
			if (valveBuff[0] != valveData) {
				valveData = valveBuff[0];
				valveTimeStamp = chrono::steady_clock::now();
			}
			
		}

		if (valveData == ROTATION_DEGREE ||
			valveData == (ROTATION_DEGREE - 360) ||
			valveData == (ROTATION_DEGREE + 360)
			) {
			chrono::duration variance(valveTimeStamp - ENG_TICK_CLOCK);
			avgVar += variance.count();
			avgVar /= 2;
		}
		
		cout << "Average latency is: " << avgVar << "\r";
	}
}

void sendSignal() {
	HANDLE hHeap = GetProcessHeap();
	BOOL* cpsBuff = (BOOL*)HeapAlloc(hHeap, 0, BUFFSIZE * sizeof(BOOL));

	DWORD bytesToWrite = 0, bytesWritten = 0;
	BOOL success = FALSE;

	cpsBuff[0] = SIGNAL;

	success = WriteFile(
		cpsPipe,
		cpsBuff,
		1,
		&bytesWritten,
		NULL);

	if (!success || 1 != bytesWritten)
	{
		_tprintf(TEXT("InstanceThread WriteFile failed, GLE=%d.\n"), GetLastError());
	}
}
#else
int main(int ac, char **av)
{
	return 0;
}
#endif
