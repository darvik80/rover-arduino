//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_STREAMREADER_H
#define ARDUINO_STREAMREADER_H


#include "utils/Reader.h"

#include <Stream.h>

class StreamReader : public Reader {
private:
    Stream& _stream;
public:
    StreamReader(const StreamReader& reader) = delete;
    StreamReader& operator = (const StreamReader& reader) = delete;
    StreamReader(Stream& stream) : _stream(stream) {}

    size_t available() override {
        return _stream.available();
    }
    size_t readBytes(uint8_t *data, size_t size) override {
        if (available() < size) {
            return 0;
        }
        return _stream.readBytes(data, size);
    }
};


#endif //ARDUINO_STREAMREADER_H
