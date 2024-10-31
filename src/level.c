#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "libraries/LIS3DH.h"
#include "libraries/neomatrix.h"
#include "math.h"

#include "blink.pio.h"

#define I2C_SDA     8
#define I2C_SCL     9

#define GREEN_NB    

#ifdef i2c0

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    blink_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (125000000 / (2 * freq)) - 3;
}

#endif

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}


int main() {

    stdio_init_all();
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/lis3dh_i2c example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else
    printf("Hello, LIS3DH! Reading raw data from registers...\n");

    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // PIO Blinking example
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &blink_program);
    printf("Loaded program at %d\n", offset);
    
    #ifdef PICO_DEFAULT_LED_PIN
    blink_pin_forever(pio, 0, offset, PICO_DEFAULT_LED_PIN, 3);
    #else
    blink_pin_forever(pio, 0, offset, 6, 3);
    #endif

    LIS3DH accelerometer;    
    if (!lis3dh_init())
    {
        while(1){
            printf("failed accel init");
        }
    }

    NeoMatrix neomatrix;
    if (!neomatrix_init(&neomatrix, 8, 8))
    {
        while(1){
            printf("failed neomatrix init");
        }
    }

    while (1) {
        update(&accelerometer);
        printf("x: %3fg, y: %3fg, z: %3fg\n", accelerometer.x, accelerometer.y, accelerometer.z);
        clear_pixels(&neomatrix);
        if (accelerometer.x < 0.1 && accelerometer.x > -0.1 && accelerometer.y < 0.1 && accelerometer.y > -0.1)
        {
            set_pixel(&neomatrix, 4, 4, urgb_u32(0, 20, 0));
            set_pixel(&neomatrix, 4, 3, urgb_u32(0, 20, 0));
            set_pixel(&neomatrix, 3, 4, urgb_u32(0, 20, 0));
            set_pixel(&neomatrix, 3, 3, urgb_u32(0, 20, 0));
        }
        else
        {
            int x_index = (int)round((accelerometer.x + 1) * 3.5);
            int y_index = (int)round((accelerometer.y + 1) * 3.5);
            if (x_index < 0) x_index = 0;
            if (x_index > 7) x_index = 7;
            if (y_index < 0) y_index = 0;
            if (y_index > 7) y_index = 7;
            set_pixel(&neomatrix, x_index, y_index, urgb_u32(20, 0, 0));
        }
        write(&neomatrix);
        sleep_ms(100);
        clear_pixels(&neomatrix);
        write(&neomatrix);
    }
    
    return 0;

#endif
}