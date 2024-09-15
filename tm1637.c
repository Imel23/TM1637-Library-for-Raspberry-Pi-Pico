#include "tm1637.h"
#include <stdio.h>

// Segment patterns for digits 0-9
static const uint8_t segment_patterns[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// GPIO pins
static uint CLK_PIN;
static uint DIO_PIN;

// Internal functions
static void tm1637_start(void);
static void tm1637_stop(void);
static void tm1637_write_bit(bool bit);
static bool tm1637_write_byte(uint8_t byte);
static void tm1637_write_command(uint8_t command);

void tm1637_init(uint gpio_clk, uint gpio_dio)
{
    CLK_PIN = gpio_clk;
    DIO_PIN = gpio_dio;

    gpio_init(CLK_PIN);
    gpio_init(DIO_PIN);

    gpio_set_dir(CLK_PIN, GPIO_OUT);
    gpio_set_dir(DIO_PIN, GPIO_OUT);

    tm1637_clear_display();
    tm1637_set_brightness(TM1637_BRIGHTNESS_MAX);
}

void tm1637_set_brightness(tm1637_brightness_t brightness)
{
    if (brightness > TM1637_BRIGHTNESS_MAX)
        brightness = TM1637_BRIGHTNESS_MAX;
    tm1637_write_command(TM1637_CMD_DISPLAY | brightness);
}

void tm1637_display_digit(uint8_t position, uint8_t digit)
{
    if (position > 3 || digit > 9)
        return;

    tm1637_start();
    tm1637_write_byte(TM1637_CMD_DATA_FIXED);
    tm1637_stop();

    tm1637_start();
    tm1637_write_byte(TM1637_CMD_ADDR_START + position);
    tm1637_write_byte(segment_patterns[digit]);
    tm1637_stop();
}

void tm1637_display_number(int number)
{
    if (number < 0 || number > 9999)
        return;

    uint8_t digits[4] = {0};

    // Extract digits and store them in the array from most significant digit to least significant digit
    for (int i = 0; i < 4; i++)
    {
        digits[3 - i] = number % 10;
        number /= 10;
    }

    tm1637_start();
    tm1637_write_byte(TM1637_CMD_DATA_FIXED);
    tm1637_stop();

    for (int i = 0; i < 4; i++)
    {
        tm1637_start();
        tm1637_write_byte(TM1637_CMD_ADDR_START + i);
        tm1637_write_byte(segment_patterns[digits[i]]);
        tm1637_stop();
    }
}

void tm1637_clear_display(void)
{
    tm1637_start();
    tm1637_write_byte(TM1637_CMD_DATA_FIXED);
    tm1637_stop();

    for (int i = 0; i < 4; i++)
    {
        tm1637_start();
        tm1637_write_byte(TM1637_CMD_ADDR_START + i);
        tm1637_write_byte(0x00);
        tm1637_stop();
    }
}

// Internal function implementations
static void tm1637_start(void)
{
    gpio_set_dir(DIO_PIN, GPIO_OUT);
    gpio_put(DIO_PIN, 1);
    gpio_put(CLK_PIN, 1);
    sleep_us(TM1637_DELAY_US);
    gpio_put(DIO_PIN, 0);
    sleep_us(TM1637_DELAY_US);
    gpio_put(CLK_PIN, 0);
}

static void tm1637_stop(void)
{
    gpio_set_dir(DIO_PIN, GPIO_OUT);
    gpio_put(CLK_PIN, 0);
    gpio_put(DIO_PIN, 0);
    sleep_us(TM1637_DELAY_US);
    gpio_put(CLK_PIN, 1);
    sleep_us(TM1637_DELAY_US);
    gpio_put(DIO_PIN, 1);
    sleep_us(TM1637_DELAY_US);
}

static void tm1637_write_bit(bool bit)
{
    gpio_put(CLK_PIN, 0);
    if (bit)
    {
        gpio_set_dir(DIO_PIN, GPIO_IN);
    }
    else
    {
        gpio_set_dir(DIO_PIN, GPIO_OUT);
        gpio_put(DIO_PIN, 0);
    }
    sleep_us(TM1637_DELAY_US);
    gpio_put(CLK_PIN, 1);
    sleep_us(TM1637_DELAY_US);
    gpio_put(CLK_PIN, 0);
    gpio_set_dir(DIO_PIN, GPIO_OUT);
}

static bool tm1637_write_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        tm1637_write_bit(byte & 0x01);
        byte >>= 1;
    }
    // Wait for ACK
    gpio_set_dir(DIO_PIN, GPIO_IN);
    sleep_us(TM1637_DELAY_US);
    gpio_put(CLK_PIN, 0);
    sleep_us(TM1637_DELAY_US);
    gpio_put(CLK_PIN, 1);
    sleep_us(TM1637_DELAY_US);
    bool ack = !gpio_get(DIO_PIN);
    gpio_put(CLK_PIN, 0);
    gpio_set_dir(DIO_PIN, GPIO_OUT);
    return ack;
}

static void tm1637_write_command(uint8_t command)
{
    tm1637_start();
    tm1637_write_byte(command);
    tm1637_stop();
}
