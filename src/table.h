#ifndef __TABLE_H__ 
#define __TABLE_H__

struct ValveTable {
	double ra1;
	int ra2;
	int lobe_sep;
	int* duration;
	double* lift;
	int* open;
	int* close;
	int* lobe_close;
};

extern ValveTable testTable;

#endif
