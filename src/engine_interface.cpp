#include <stdio.h>
#include <string>

#ifdef COMPILING_FOR_RASPI_PICO

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "lcd_interface.h"

#endif

//-----------SPECIFING PINS-----------------
#define SDA_PIN 0
#define SCL_PIN 1
#define DEFAULT_LED 25

#define GAS_SENSOR_PIN 26
#define HALLO_SENSOR_PIN 27


//------------STRUCT FOR DISPLAY OUTPUTS-----------------
struct DisplayValue
{
    std::string header = "";
    int header_row = 0;
    int header_column = 0;
    std::string val = "4095";
    int val_row = 0;
    int val_column = 0;
}hall_sensor, gas_sensor;


#ifdef COMPILING_FOR_RASPI_PICO
int main() {
    stdio_init_all();

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));
    lcd_init();

    hall_sensor.header = "Hall: ";
    hall_sensor.val_column = 10;
    hall_sensor.val = "4095";

    gas_sensor.header = "Gas: ";
    gas_sensor.header_row = 1;
    gas_sensor.val_row = 1;
    gas_sensor.val_column = 10;
    gas_sensor.val = "4095";

    lcd_set_cursor(hall_sensor.header_row, hall_sensor.header_column);
    lcd_send_string(hall_sensor.header, hall_sensor.header.length());
    lcd_set_cursor(hall_sensor.val_row, hall_sensor.val_column);
    lcd_send_string(hall_sensor.val, hall_sensor.val.length());

    lcd_set_cursor(gas_sensor.header_row, gas_sensor.header_column);
    lcd_send_string(gas_sensor.header, gas_sensor.header.length());
    lcd_set_cursor(gas_sensor.val_row, gas_sensor.val_column);
    lcd_send_string(gas_sensor.val, gas_sensor.val.length());
    sleep_ms(3000);

}
#else
int main() {
	return 0;
}
#endif