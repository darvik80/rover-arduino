//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_CRC16_H
#define ARDUINO_CRC16_H

#include <stdint-gcc.h>
#include <utils/memory/Buffer.h>

uint16_t crc16(uint8_t* data, size_t size);
uint16_t crc16(Buffer& buffer);

#endif //ARDUINO_CRC16_H
