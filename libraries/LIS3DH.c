#include "LIS3DH.h"
#include <stdio.h>

#define LIS3DH_ADDRESS 0x18
#define I2C_SDA        8
#define I2C_SCL        9
#define CTRL_REG1      0x20
#define WHO_AM_I       0x0F
#define OUT_X_L        0x28
#define OUT_Y_L        0x2A
#define OUT_Z_L        0x2C

bool lis3dh_init(void) {
    set_reg(CTRL_REG1, 0x97);
    return read_reg(CTRL_REG1) == 0x97;
}

void set_reg(uint8_t reg, uint8_t val) {
    uint8_t data[] = {reg, val};
    i2c_write_blocking(i2c_default, LIS3DH_ADDRESS, data, 2, false);
}

uint8_t read_reg(uint8_t reg) {
    uint8_t val;
    i2c_write_blocking(i2c_default, LIS3DH_ADDRESS, &reg, 1, true);
    i2c_read_blocking(i2c_default, LIS3DH_ADDRESS, &val, 1, false);
    return val;
}

void lis3dh_read_data(uint8_t reg, float *final_value) {
    uint8_t lsb = read_reg(reg);
    reg |= 0x01;
    uint8_t msb = read_reg(reg);
    uint16_t raw_accel;

    raw_accel = (msb << 8) | lsb;

    float senstivity = 0.004f; // g per unit
    float scaling = 64 / senstivity;
    *final_value = (float) ((int16_t) raw_accel) / scaling;
}

void update(LIS3DH* accelerometer) {
    lis3dh_read_data(OUT_X_L, &accelerometer->x);
    lis3dh_read_data(OUT_Y_L, &accelerometer->y);
    lis3dh_read_data(OUT_Z_L, &accelerometer->z);
}
