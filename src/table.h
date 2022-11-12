#ifndef __TABLE_H__ 
#define __TABLE_H__

#include <stdio.h>
#define __STDIO_H__

#include <cstdlib>
#define  __CSTDLIB__

struct ValveTable {
	const unsigned int* table;
	int table_size;
	int num_of_cylinders;
};

extern ValveTable test_table;

static inline bool check_bit(unsigned char bit, unsigned short angle) {
	if (angle > 719) {
		printf("\"angle\" value must be less than 720!");
		exit(EXIT_FAILURE);
	}
	if (bit > 31) {
		printf("\"bit\" value must be less than 32!");
		exit(EXIT_FAILURE);
	}
	return test_table.table[angle] & (1 << bit);
};

#endif // !__TABLE_H__
