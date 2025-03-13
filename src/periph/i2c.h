#ifndef I2C_H 
#define I2C_H
#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>

#define XSHUT_LOC1 4
#define LOC1_ADDR (0x29)
#define VL53L0X_ADDR 0x29

void i2c_init(void); // sets up
void i2c_set_slave_address(uint8_t address);

bool i2c_read_addr8_data8(uint8_t address, uint8_t reg, uint8_t *data);
bool i2c_write_addr8_data8(uint8_t address, uint8_t reg, uint8_t data);

bool data_init(uint8_t address);
bool load_default_settings(uint8_t address);
bool read_range(uint8_t address, uint16_t *range);

#endif