//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_READER_H
#define ARDUINO_READER_H


#include <stddef.h>
#include <stdint.h>
#include "memory/Buffer.h"

class Reader {
public:
    template<typename T>
    size_t read(T& value) {
        return read((uint8_t*)&value, sizeof(T));
    }
    size_t read(Buffer& buffer, size_t size) {
        if (buffer.getSize() < size) {
            return 0;
        }
        return read(buffer.getData(), size);
    }

    template <typename T>
    size_t read(T* destination, size_t size)
    {
        return readBytes(destination, sizeof(T) * size);
    }

    virtual size_t available() = 0;
    virtual size_t readBytes(uint8_t* data, size_t size) = 0;
};


#endif //ARDUINO_READER_H
