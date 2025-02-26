#include <stdio.h>
#include "bno08x_rpi.h"

#define BNO08X_I2C_ADDR 0x4B  // I2C Address of BNO08x
#define INT_PIN 4  // WiringPi pin for INT (optional)
#define RST_PIN 5  // WiringPi pin for RST (optional)

int main() {
    if (!BNO08x_begin(BNO08X_I2C_ADDR, INT_PIN, RST_PIN)) {
        printf("Failed to initialize BNO08x!\n");
        return -1;
    }

    while (1) {
        float x, y, z;
        if (BNO08x_getMagnetometer(&x, &y, &z)) {
            printf("Magnetometer: X=%.2f uT, Y=%.2f uT, Z=%.2f uT\n", x, y, z);
        } else {
            printf("Failed to read magnetometer data!\n");
        }

        usleep(500000);  // Delay 500ms
    }

    return 0;
}
