#ifndef __TABLE_H__ 
#define __TABLE_H__

struct ValveTable {
	const unsigned int* table;
	int table_size;
	int num_of_cylinders;
};

extern ValveTable test_table;

static inline bool check_bit(unsigned char bit, unsigned short angle) {
	if (angle > 719)
		throw "\"angle\" must be positive and less than 720!";
	if (bit > 31)
		throw "\"bit\" must be positive less than 32!";
	return test_table.table[angle] & (1 << bit);
};

#endif // !__TABLE_H__
