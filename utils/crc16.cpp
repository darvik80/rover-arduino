//
// Created by Kishchenko, Ivan on 1/10/20.
//

#include "crc16.h"
#include <util/crc16.h>

uint16_t crc16(uint8_t* data, size_t size) {
    uint16_t result = 0xffff;
    for (size_t idx = 0; idx < size; ++idx) {
        result = _crc16_update(result, data[idx]);
    }

    return result;
}

uint16_t crc16(Buffer& buffer) {
    return crc16(buffer.getData(), buffer.getSize());
}
