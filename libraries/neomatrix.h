#ifndef NeoMatrix_H
#define NeoMatrix_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

typedef struct {
    uint8_t width, height;
    uint sm;
    PIO pio;
    uint32_t* buf;
} NeoMatrix;


bool neomatrix_init(NeoMatrix* neomatrix, uint8_t width, uint8_t height);
void set_pixel(NeoMatrix* neomatrix, uint8_t row, uint8_t col, uint32_t color);
void clear_pixels(NeoMatrix* neomatrix);
void write(NeoMatrix* neomatrix);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);

#endif