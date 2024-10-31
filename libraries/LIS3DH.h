#ifndef LIS3DH_H
#define LIS3DH_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

typedef struct {
    float x, y, z;
} LIS3DH;

bool lis3dh_init(void);
void set_reg(uint8_t reg, uint8_t val);
uint8_t read_reg(uint8_t reg);
void update(LIS3DH* accelerometer);

#endif
