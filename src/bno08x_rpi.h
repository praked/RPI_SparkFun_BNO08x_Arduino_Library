#ifndef BNO08X_RPI_H
#define BNO08X_RPI_H

#include <stdint.h>
#include <stdbool.h>

bool BNO08x_begin(int address, int intPin, int rstPin);
uint8_t BNO08x_readRegister(uint8_t reg);
void BNO08x_writeRegister(uint8_t reg, uint8_t value);
bool BNO08x_getMagnetometer(float *x, float *y, float *z);

#endif
