

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef COMPILING_FOR_RASPI_PICO
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#else
#include <unistd.h>
#endif

#ifdef COMPILING_FOR_RASPI_PICO
static int gpio_state = 0;
void gpio_interrupt_handler_callback(uint gpio, uint32_t events)
{
    if ((events & 0x4) != 0) // EDGE_FALL
    {
    	gpio_state = 0;
    }
    else if ((events & 0x8) != 0) // EDGE_RISE
    {
    	gpio_state = 1;
    }
}
#endif

int read_from_gpio(void)
{
#ifdef COMPILING_FOR_RASPI_PICO
	return gpio_state != 0;
#else
	return (rand() % 2) != 0;
#endif
}


int main(int ac, char **av)
{
	(void)ac;
	(void)av;

#ifdef COMPILING_FOR_RASPI_PICO
	stdio_init_all();
	gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_interrupt_handler_callback);
#endif
	setbuffer(stdout, NULL, 0);
	for (;;)
	{
		fwrite(read_from_gpio() != 0 ? "1" : "0", 1, 1, stdout);
#ifdef COMPILING_FOR_RASPI_PICO
		sleep_ms(1000);
#else
		usleep(100000);
#endif
	}
	return 0;
}
