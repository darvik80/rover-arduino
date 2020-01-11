//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_BUFFERWRITER_H
#define ARDUINO_BUFFERWRITER_H


#include <utils/Writer.h>
#include "Buffer.h"

class BufferWriter : public Buffer, public Writer {
public:
    BufferWriter(const BufferWriter& rhs) = delete;
    BufferWriter& operator = (const BufferWriter& rhs) = delete;
    BufferWriter(uint8_t* data, size_t size) : Buffer(data, size) {}

    size_t writeBytes(const uint8_t* data, size_t size) override;
};


#endif //ARDUINO_BUFFERWRITER_H
