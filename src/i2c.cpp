#include <stdio.h>
#include <string>

#ifdef COMPILING_FOR_RASPI_PICO

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "lcd_constants.h"

using namespace LCDConstants;
#endif

//-----------SPECIFING PINS-----------------
#define SDA_PIN 0
#define SCL_PIN 1
#define DEFAULT_LED 25

#define GAS_SENSOR_PIN 26
#define HALLO_SENSOR_PIN 27

//-----------OTHER MACROS-------------------
#define DELAY_US 600

// Single byte transfer function
#ifdef COMPILING_FOR_RASPI_PICO
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, LCD_ADDR, &val, 1, false);
#endif
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
// We send a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    // mode: 1 or 0 (sending char or a command)
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;   // the actuall part of the byte gets sent in the first nibble; the other nibble is to keep the screen on
    uint8_t low = mode | ((val << 4) & 0xF0)| LCD_BACKLIGHT; // shifting the lsb nibble to the msb nibble and setting the lsb nibble to blink

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
static void inline lcd_send_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
void lcd_send_string(const std::string str, size_t str_len) {
    for (size_t i = 0; i < str_len; i++){
		lcd_send_byte(str.at(i), LCD_CHARACTER);
	}
}
#endif

#ifdef COMPILING_FOR_RASPI_PICO
// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}
#endif

//------------STRUCT FOR DISPLAY OUTPUTS-----------------
struct DisplayValue
{
    std::string header = "";
    int header_row = 0;
    int header_column = 0;
    std::string val = 0;
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
    hall_sensor.val = std::to_string(4095);

    gas_sensor.header = "Gas: ";
    gas_sensor.header_row = 1;
    gas_sensor.val_row = 1;
    gas_sensor.val_column = 10;
    gas_sensor.val = std::to_string(4095);

    lcd_set_cursor(hall_sensor.header_row, hall_sensor.header_column);
    lcd_send_string(hall_sensor.header, hall_sensor.header.length());
    lcd_set_cursor(hall_sensor.val_row, hall_sensor.val_column);
    lcd_send_string(hall_sensor.val, hall_sensor.val.length());

    lcd_set_cursor(gas_sensor.header_row, gas_sensor.header_column);
    lcd_send_string(gas_sensor.header, gas_sensor.header.length());
    lcd_set_cursor(gas_sensor.val_row, gas_sensor.val_column);
    lcd_send_string(gas_sensor.val, gas_sensor.val.length());

}
#else
int main() {
	return 0;
}
#endif