#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define SDA_PIN 0
#define SCL_PIN 1
#define DEFAULT_LED 25

#define DELAY_US 600


// Modes for lcd_send_byte
#define LCD_CHARACTER  1    // sending a char
#define LCD_COMMAND    0    // sending a command for LCD

// defining the size of the screen
#define MAX_LINES 2
#define MAX_CHARS 16

/*-------------------------------------------------------------------*/

// commands for LCD
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08; // the code you send(as the lsb nibble) to keep the screen on 
const int LCD_ENABLE_BIT = 0x04;

// By default these LCD display drivers are on bus address 0x27
const static int LCD_ADDR = 0x27;

/*---------------------------------------------------------------------*/

// Single byte transfer function
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, LCD_ADDR, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    // mode: 1 or 0 (sending char or a command)
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;   // the actuall part of the byte gets sent in the first nibble; the other nibble is to keep the screen on
    uint8_t low = mode | ((val << 4) & 0xF0)| LCD_BACKLIGHT; // shifting the lsb nibble to the msb nibble and setting the lsb nibble to blink

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

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

static void inline lcd_send_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_send_string(const std::string str, size_t str_len) {
    for (size_t i = 0; i < str_len; i++){
		lcd_send_byte(str.at(i), LCD_CHARACTER);
	}
}

// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

int main() {
    stdio_init_all();
    i2c_init(i2c_default, 100 * 1000);

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));
    
    lcd_init();
    static size_t message_size = 4;
    std::string message[message_size] =
            {
                    "aaa", "bbb",
                    "ccc", "ddd"
            };

    while (1) {
        for (size_t word = 0; word < message_size; word++)
        {
                lcd_set_cursor(0, 0);
                lcd_send_string(message[word], message[word].length());
                sleep_ms(2000);
                lcd_clear();   
        }
    }
    return 0;
}