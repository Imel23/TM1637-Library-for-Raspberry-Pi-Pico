#ifndef TM1637_H
#define TM1637_H

#include "pico/stdlib.h"

// TM1637 Configuration
#define TM1637_CLK_PIN 2   // Change as per your wiring
#define TM1637_DIO_PIN 3   // Change as per your wiring
#define TM1637_DELAY_US 20 // Delay time in microseconds

// TM1637 Commands
#define TM1637_CMD_DATA_AUTO  0x40
#define TM1637_CMD_DATA_FIXED 0x44
#define TM1637_CMD_ADDR_START 0xC0
#define TM1637_CMD_DISPLAY    0x88

// Brightness levels (0 to 7)
typedef enum {
    TM1637_BRIGHTNESS_MIN = 0,
    TM1637_BRIGHTNESS_MAX = 7
} tm1637_brightness_t;

// Function Prototypes
void tm1637_init(uint gpio_clk, uint gpio_dio);
void tm1637_set_brightness(tm1637_brightness_t brightness);
void tm1637_display_digit(uint8_t position, uint8_t digit);
void tm1637_display_number(int number);
void tm1637_clear_display(void);

#endif // TM1637_H
