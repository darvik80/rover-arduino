//
// Created by Kishchenko, Ivan on 1/10/20.
//

#ifndef ARDUINO_BUFFER_H
#define ARDUINO_BUFFER_H


#include <stdint.h>
#include <stddef.h>

class Buffer {
private:
    uint8_t* _data;
    const size_t _size;
    size_t _offset{0};
public:
    Buffer(const Buffer& rhs) = delete;
    Buffer& operator = (const Buffer& rhs) = delete;

    Buffer(uint8_t* data, size_t size) : _data(data), _size(size) {}

    size_t remaining() {
        return _size - _offset;
    }

    [[nodiscard]] const uint8_t* getData() const {
        return _data;
    }
    uint8_t* getData() {
        return _data;
    }
    size_t getSize() {
        return _size;
    }
    size_t getOffset() {
        return _offset;
    }

    void setOffset(size_t offset);
    void skip(size_t block);
    void back(size_t block);
};


#endif //ARDUINO_BUFFER_H
