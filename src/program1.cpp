#include <iostream>
#include "table.h"

using namespace std;

int main(int ac, char **av)
{
	cout << masks::BIT_32_MASK << endl;
	cout << (masks::BIT_32_MASK & test_table.table[0]) << endl;
	cout << !(masks::BIT_32_MASK & test_table.table[0]) << endl;

	//for (int i = 0; i < test_table.table_size; i++) {
	//	cout<< test_table.table[i] << endl;
	//}

	return 0;
}
