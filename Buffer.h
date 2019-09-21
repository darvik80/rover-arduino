//
// Created by Ivan Kishchenko on 21/09/2019.
//

#ifndef ROVER_ARDUINO_BUFFER_H
#define ROVER_ARDUINO_BUFFER_H


#define BATCH_STACK_SIZE 8

class Buffer : public Stream {
private:
    char* _data;
    size_t _pos;
    size_t _size;
public:
    Buffer(size_t pos, size_t size, char *data) : _data(data), _pos(pos), _size(size) {}
    Buffer() : _data(nullptr), _pos(0), _size(0) {}

    size_t write(uint8_t uint8) override {
        return 0;
    }

    int available() override {
        return int16_t (_size - _pos);
    }

    int read() override {
        if (_pos < _size) {
            return _data[_pos++];
        }

        return -1;
    }

    int peek() override {
        if (_pos < _size) {
            return _data[_pos];
        }

        return -1;
    }

    void reset() {
        free(_data);
        _pos = 0;
        _size = 0;
    }
};

#endif //ROVER_ARDUINO_BUFFER_H
