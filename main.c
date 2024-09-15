#include "pico/stdlib.h"
#include "tm1637.h"

int main()
{
    stdio_init_all();

    // Initialize the TM1637 display
    tm1637_init(2, 3); // Use GPIO 2 for CLK and GPIO 3 for DIO

    // Set display brightness (0 to 7)
    tm1637_set_brightness(4);

    // Display a number
    tm1637_display_number(1234);

    // Main loop
    while (true)
    {
        // Increment and display numbers
        for (int i = 0; i <= 9999; i++)
        {
            tm1637_display_number(i);
            sleep_ms(500);
        }

        // Clear the display
        tm1637_clear_display();
        sleep_ms(1000);
    }

    return 0;
}