//
// Created by Ivan Kishchenko on 08/09/2019.
//

#ifndef ROVER_ARDUINO_PROTOCOL_H
#define ROVER_ARDUINO_PROTOCOL_H

#pragma pack(push, 1)

#include <stdint.h>

namespace protocol {

#define PROTO_MAGIC 0x4452413f

    enum Code {
        ProtoInit,
        ProtoBatch,
        ProtoExecBatch,
        ProtoDigitalWrite,
        ProtoDigitalRead,
        ProtoAnalogWrite,
        ProtoAnalogRead,
        ProtoDelay,
        ProtoMode,
        ProtoMessage,
    };

    enum PinMode {
        Input,
        Output,
        InputPullUp
    };

    class Packet {
    public:
        explicit Packet(uint16_t code, uint8_t len) : magic(PROTO_MAGIC), code(code), size(len) {}
        explicit Packet()  : magic(PROTO_MAGIC), code(0), size(0) {};

        uint32_t magic;
        uint8_t code;
        uint8_t size;

        virtual size_t pack(uint8_t* buffer, size_t bufSize) {
            checkLimits(bufSize);
            memcpy(buffer, &magic, sizeof(uint32_t));
            memcpy(buffer + sizeof(uint32_t), &code, sizeof(uint8_t));
            memcpy(buffer + sizeof(uint32_t) + sizeof(uint8_t), &size, sizeof(uint8_t));

            return sizeof(uint32_t) + sizeof(uint16_t);
        }

        virtual uint8_t getSize() {
            return size + sizeof(uint32_t) + sizeof(uint16_t);
        }

    protected:
        void checkLimits(size_t bufSize) {
            if (bufSize < getSize()) {
                //throw std::invalid_argument("not enough buffer size: " +  std::to_string(bufSize) + "/" + std::to_string(getSize()));
            }
        }
    };

    class DigitalWrite : public Packet {
    public:
        DigitalWrite(uint8_t pin, bool value) : Packet(ProtoDigitalWrite, sizeof(uint16_t)),  pin(pin), value(value) {}
        DigitalWrite() : DigitalWrite(0, false) {}
        uint8_t pin;
        uint8_t value;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;
            buffer[base] = value;

            return getSize();
        }
    };

    class DigitalRead : public Packet {
    public:
        explicit DigitalRead(uint8_t pin) : Packet(ProtoDigitalRead, sizeof(uint8_t)),  pin(pin) {}
        DigitalRead() : DigitalRead(0) {}
        uint8_t pin;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base] = pin;

            return getSize();
        }
    };

    class AnalogWrite : public Packet {
    public:
        AnalogWrite(uint8_t pin, uint16_t value) : Packet(ProtoAnalogWrite, sizeof(uint16_t) + sizeof(uint8_t)), pin(pin), value(value) {}
        AnalogWrite() : AnalogWrite(0, 0) {}

        uint8_t pin;
        uint16_t value;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;
            memcpy(buffer + base, &value, sizeof(uint16_t));

            return getSize();
        }

    };

    class AnalogRead : public Packet {
    public:
        explicit AnalogRead(uint8_t pin) : Packet(ProtoAnalogRead, sizeof(uint8_t)),  pin(pin) {}
        AnalogRead() : AnalogRead(0) {}
        uint8_t pin;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base] = pin;

            return getSize();
        }

    };

    class Mode : public Packet {
    public:
        Mode(uint8_t pin, PinMode mode) : Packet(ProtoMode, sizeof(uint16_t)),  pin(pin), mode(mode) {}
        Mode() : Mode(0, Input) {}
        uint8_t pin;
        uint8_t mode;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;
            buffer[base] = mode;

            return getSize();

        }
    };

    class Delay : public Packet {
    public:
        explicit Delay(uint16_t delay) : Packet(ProtoDelay, sizeof(uint16_t)), delay(delay) {}
        Delay() : Delay(0) {}

        uint16_t delay;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);

            memcpy(buffer + base, &delay, sizeof(uint16_t));

            return getSize();
        }
    };

    class RegisterBatch : public Packet {
    public:
        RegisterBatch(uint8_t batch, uint8_t *context, uint8_t len) : Packet(ProtoBatch, len+sizeof(uint8_t)), batch(batch), context(context) {}
        RegisterBatch() : RegisterBatch(0, nullptr, 0) {}

        uint8_t batch;
        uint8_t * context;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);

            buffer[base++] = batch;
            memcpy(buffer + base, context, size);

            return getSize();
        }
    };

    class ExecBatch : public Packet {
    public:
        explicit ExecBatch(uint8_t id) : Packet(ProtoExecBatch, sizeof(uint8_t)), id(id) {}
        ExecBatch() : ExecBatch(0) {}

        uint8_t id;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);

            buffer[base++] = id;

            return base;
        }
    };

    class Message : public Packet {
    public:
        explicit Message(const char *context) : Packet(ProtoMessage, strlen(context)), context(context) {}
        Message() : Packet(ProtoMessage, 0), context(nullptr) {}

        const char* context;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);

            memcpy(buffer + base, context, size);

            return getSize();
        }
    };
}

#pragma pack(pop)

#endif //ROVER_ARDUINO_PROTOCOL_H
