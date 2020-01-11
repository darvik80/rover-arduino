//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_BUFFERREADER_H
#define ARDUINO_BUFFERREADER_H


#include <utils/Reader.h>
#include "Buffer.h"

class BufferReader : public Buffer, public Reader {
public:
    BufferReader(const BufferReader& rhs) = delete;
    BufferReader& operator = (const BufferReader& rhs) = delete;

    BufferReader(uint8_t* data, size_t size) : Buffer(data, size) {}

    virtual size_t available() override;
    size_t readBytes(uint8_t* data, size_t size) override;
};


#endif //ARDUINO_BUFFERREADER_H
