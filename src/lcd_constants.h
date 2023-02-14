#ifndef LCD_CONSTANTS_H
#define LCD_CONSTANTS_H

namespace LCDConstants {
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
}

#endif 