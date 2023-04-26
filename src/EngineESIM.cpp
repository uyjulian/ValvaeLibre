#include "pico/stdlib.h"
#include "LiquidCrystal_I2C.h"
#include "hardware/adc.h"
#include "pico/time.h"
//#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>

#define ADC_NUM 1
#define ADC_PIN (26 + ADC_NUM)
#define ADC_RANGE (1 << 12)

uint RPM = 60;
int64_t WAIT_PER_DEGREE = 0;
uint16_t ROTATION_DEGREE = 0;
u_int8_t toneWheelTBL[] = { 1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };



int main() {

    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3f,16,2);


    lcd.init();
    lcd.backlight();
    lcd.printf("Initializing...");
    sleep_ms(1000);

    stdio_init_all();
    
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(ADC_NUM);

    lcd.clear();
    lcd.printf("RPM: ");
    lcd.setCursor(5,0);

    uint adc_raw;
    //absolute_time_t start, finish;
    int64_t gap;


    bool intake = true, exhaust = false;
    gpio_init(0);
    gpio_init(16);
    gpio_set_dir(0, true);
    gpio_set_dir(16, true);
    gpio_put(0, intake );
    gpio_put(16, exhaust);

    while (true){
        adc_raw = adc_read();
        RPM = (60 + (adc_raw * (3540.0 / (ADC_RANGE - 1))));
        //start = get_absolute_time();
        WAIT_PER_DEGREE = 1000000 / (((RPM / 60.0) / 2) * 720);
        //finish = get_absolute_time();
        lcd.print(RPM);
        lcd.printf("  ");
        lcd.setCursor(0,1);
        //gap = absolute_time_diff_us(start, finish);
        //lcd.print(gap);
        lcd.print(WAIT_PER_DEGREE);
        lcd.printf(" ms ");
        lcd.printf("   ");
        sleep_ms(WAIT_PER_DEGREE);
        intake = !intake;
        exhaust = !exhaust;
        gpio_put(0, intake );
        gpio_put(16, exhaust);
        lcd.setCursor(5,0);

    }

    

    return 0;
}