#ifndef __TABLE_H__ 
#define __TABLE_H__

namespace masks {
	const unsigned BIT_32_MASK = 1 << 31;
	const unsigned BIT_31_MASK = 1 << 30;
	const unsigned BIT_30_MASK = 1 << 29;
	const unsigned BIT_29_MASK = 1 << 28;

	const unsigned BIT_16_MASK = 1 << 15;
	const unsigned BIT_15_MASK = 1 << 14;
	const unsigned BIT_14_MASK = 1 << 13;
	const unsigned BIT_13_MASK = 1 << 12;
}

struct ValveTable {
	const unsigned int* table;
	int table_size;
	int num_of_cylinders;
};

extern ValveTable test_table;

#endif
