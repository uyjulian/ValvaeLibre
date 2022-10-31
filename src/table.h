#ifndef __TABLE_H__ 
#define __TABLE_H__

struct ValveTable {
	const unsigned int* table;
	int table_size;
	int num_of_cylinders;
};

extern ValveTable testTable;

#endif
