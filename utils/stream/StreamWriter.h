//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_STREAMREADER_H
#define ARDUINO_STREAMREADER_H


#include "utils/Writer.h"

#include <Stream.h>

class StreamWriter : public Writer {
private:
    Stream &_stream;
public:
    StreamWriter(const StreamWriter& writer) = delete;
    StreamWriter& operator = (const StreamWriter& writer) = delete;
    StreamWriter(Stream& stream) : _stream(stream) {}

    size_t writeBytes(const uint8_t *data, size_t size) override {
        if (_stream.availableForWrite() < size) {
            return 0;
        }
        return _stream.write(data, size);
    }
};


#endif //ARDUINO_STREAMREADER_H
