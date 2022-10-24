#ifndef __TABLE_H__ 
#define __TABLE_H__

struct ValveTable {
	double ra1;
	int ra2;
	int lobe_sep;

	const int* duration;
	const double* lift;
	const int* open;
	const int* close;
	const int* lobe_close;

	int durationSize;
	int liftSize;
	int openSize;
	int closeSize;
	int lobeSize;
};

extern ValveTable testTable;

#endif
