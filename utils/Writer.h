//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_WRITER_H
#define ARDUINO_WRITER_H


#include <stddef.h>
#include "memory/Buffer.h"

class Writer {
public:
    template<typename T>
    size_t write(T value) {
        return writeBytes((uint8_t*)&value, sizeof(T));
    }

    size_t write(Buffer& buffer) {
        return writeBytes(buffer.getData(), buffer.getSize());
    }

    template <typename T>
    size_t write(T* destination, size_t size) const
    {
        return wrtieBytes(destination, sizeof(T) * size);
    }

    virtual size_t writeBytes(const uint8_t* data, size_t size) = 0;
};


#endif //ARDUINO_WRITER_H
