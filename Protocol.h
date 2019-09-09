//
// Created by Ivan Kishchenko on 08/09/2019.
//

#ifndef ROVER_ARDUINO_PROTOCOL_H
#define ROVER_ARDUINO_PROTOCOL_H

#pragma pack(push, 1)

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
        explicit Packet(uint16_t code) : magic(PROTO_MAGIC), code(code) {}
        explicit Packet() = default;

        uint32_t magic;
        uint16_t code;

        virtual size_t pack(uint8_t* buffer, size_t bufSize) {
            memcpy(buffer, &magic, sizeof(uint32_t));
            memcpy(buffer + sizeof(uint32_t), &code, sizeof(uint16_t));

            return sizeof(uint32_t) + sizeof(uint16_t);
        }
    };

    class DigitalWrite : public Packet {
    public:
        DigitalWrite(uint8_t pin, bool value) : Packet(ProtoDigitalWrite),  pin(pin), value(value) {}
        DigitalWrite() : DigitalWrite(0, false) {}
        uint8_t pin;
        uint8_t value;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;
            buffer[base++] = value;

            return base;
        }
    };

    class DigitalRead : public Packet {
    public:
        explicit DigitalRead(uint8_t pin) : Packet(ProtoDigitalRead),  pin(pin) {}
        DigitalRead() : DigitalRead(0) {}
        uint8_t pin;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;

            return base;
        }
    };

    class AnalogWrite : public Packet {
    public:
        AnalogWrite(uint8_t pin, int value) : Packet(ProtoAnalogWrite), pin(pin), value(value) {}
        AnalogWrite() : AnalogWrite(0, 0) {}

        uint8_t pin;
        int value;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;
            memcpy(buffer + base, &value, sizeof(int));

            return base + sizeof(int);
        }

    };

    class AnalogRead : public Packet {
    public:
        explicit AnalogRead(uint8_t pin) : Packet(ProtoAnalogRead),  pin(pin) {}
        AnalogRead() : AnalogRead(0) {}
        uint8_t pin;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;

            return base;
        }

    };

    class Mode : public Packet {
    public:
        Mode(uint8_t pin, PinMode mode) : Packet(ProtoMode),  pin(pin), mode(mode) {}
        Mode() : Mode(0, Input) {}
        uint8_t pin;
        uint8_t mode;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);
            buffer[base++] = pin;
            buffer[base++] = mode;

            return base;
        }
    };

    class Delay : public Packet {
    public:
        explicit Delay(int delay) : Packet(ProtoDelay), delay(delay) {}
        Delay() : Delay(0) {}

        int delay;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);

            memcpy(buffer + base, &delay, sizeof(int));

            return base + sizeof(int);
        }
    };

    class RegisterBatch : public Packet {
    public:
        RegisterBatch(uint8_t batch, uint8_t *context, uint8_t len) : Packet(ProtoBatch), batch(batch), len(len), context(context) {}
        RegisterBatch() : RegisterBatch(0, nullptr, 0) {}

        uint8_t batch;
        uint8_t len;
        uint8_t * context;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);

            buffer[base++] = batch;
            buffer[base++] = len;


            memcpy(buffer + base, context, len);

            return base + len;
        }
    };

    class ExecBatch : public Packet {
    public:
        explicit ExecBatch(uint8_t id) : Packet(ProtoExecBatch), id(id) {}
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
        Message(uint8_t len, const char *context) : Packet(ProtoMessage), len(len), context(context) {}
        Message() : Message(0, nullptr) {}

        uint8_t len;
        const char* context;

        size_t pack(uint8_t* buffer, size_t bufSize) override {
            size_t base = Packet::pack(buffer, bufSize);

            buffer[base++] = len;

            memcpy(buffer + base, context, len);

            return base;
        }
    };
}

#pragma pack(pop)

#endif //ROVER_ARDUINO_PROTOCOL_H
