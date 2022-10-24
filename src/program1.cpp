#include <iostream>
#include "table.h"

using namespace std;

int main(int ac, char **av)
{
	cout << testTable.ra1 << endl;

	for (int i = 0; i < testTable.durationSize; i++) {
		cout<< testTable.duration[i] << endl;
	}

	return 0;
}
