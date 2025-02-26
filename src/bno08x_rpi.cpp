#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <unistd.h>  // For usleep()
#include <stdio.h>
#include "bno08x_rpi.h"

#define BNO08X_DEFAULT_ADDR 0x4B  // Default I2C Address for BNO08x

int i2c_fd;  // File descriptor for I2C communication

// Initialize BNO08x over I2C
bool BNO08x_begin(int address, int intPin, int rstPin) {
    i2c_fd = wiringPiI2CSetup(address);
    if (i2c_fd < 0) {
        printf("Failed to initialize I2C connection!\n");
        return false;
    }

    // Setup INT and RST pins if provided
    if (intPin >= 0) {
        pinMode(intPin, INPUT);
        pullUpDnControl(intPin, PUD_UP);  // Enable pull-up resistor
    }

    if (rstPin >= 0) {
        pinMode(rstPin, OUTPUT);
        digitalWrite(rstPin, HIGH);  // Keep BNO08x out of reset
    }

    printf("BNO08x initialized successfully!\n");
    return true;
}

// Read a register from BNO08x
uint8_t BNO08x_readRegister(uint8_t reg) {
    return wiringPiI2CReadReg8(i2c_fd, reg);
}

// Write a register to BNO08x
void BNO08x_writeRegister(uint8_t reg, uint8_t value) {
    wiringPiI2CWriteReg8(i2c_fd, reg, value);
}

// Read magnetometer data
bool BNO08x_getMagnetometer(float *x, float *y, float *z) {
    uint8_t rawData[6];

    for (int i = 0; i < 6; i++) {
        rawData[i] = BNO08x_readRegister(0x28 + i);  // Read magnetometer registers
    }

    int16_t rawX = (rawData[1] << 8) | rawData[0];
    int16_t rawY = (rawData[3] << 8) | rawData[2];
    int16_t rawZ = (rawData[5] << 8) | rawData[4];

    *x = rawX * 0.1f;  // Convert to microTesla
    *y = rawY * 0.1f;
    *z = rawZ * 0.1f;

    return true;
}
