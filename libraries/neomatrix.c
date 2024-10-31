#include "neomatrix.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "generated/neopixel.pio.h"
#include <stdlib.h>
#include <stdio.h>

#define GPIO10 10
#define WS2812_PIN 7

  // --------- //
 // neomatrix //
// --------- //

void set_pixel(NeoMatrix* neomatrix, uint8_t row, uint8_t col, uint32_t color){
    if (row < neomatrix->height && col < neomatrix->width)
    {
        neomatrix->buf[row + col * neomatrix->width] = color;
    }
}

void clear_pixels(NeoMatrix* neomatrix){
    for (int i = 0; i < (neomatrix->width * neomatrix->height); i++)
    {
        neomatrix->buf[i] = 0;
    }
    
}

void write(NeoMatrix* neomatrix){
    for (int i = 0; i < (neomatrix->width * neomatrix->height); i++)
    {
        pio_sm_put_blocking(neomatrix->pio, neomatrix->sm, neomatrix->buf[i] << 8u);
    }
}

bool neomatrix_init(NeoMatrix* neomatrix, uint8_t width, uint8_t height){
    // Set the width and height
    neomatrix->width = width;
    neomatrix->height = height;
    neomatrix->pio = pio0;
    neomatrix->sm = 0;

    // Allocate memory for the pixel buffer
    neomatrix->buf = (uint32_t *)malloc(width * height * sizeof(uint32_t));
    if (neomatrix->buf == NULL) {
        return false;
    }

    uint offset = pio_add_program(neomatrix->pio, &ws2812_program);
    ws2812_program_init(neomatrix->pio, neomatrix->sm, offset, WS2812_PIN, 800000, false);

    clear_pixels(neomatrix);

    gpio_init(GPIO10);       // Initialize the GPIO pin
    gpio_set_dir(GPIO10, GPIO_OUT); // Set pin as output
    gpio_put(GPIO10, true);

    return true;
}
