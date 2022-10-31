#include <iostream>
#include "table.h"

using namespace std;

int main(int ac, char **av)
{
	for (int i = 0; i < testTable.table_size; i++) {
		cout<< testTable.table[i] << endl;
	}

	return 0;
}
