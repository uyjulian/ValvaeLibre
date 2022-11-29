
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef COMPILING_FOR_RASPI_PICO
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/multicore.h"
#else
//#include <unistd.h>
#endif


int main(int ac, char **av)
{


	return 0;
}
