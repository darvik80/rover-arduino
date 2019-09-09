#include <Arduino.h>
#include "User_Setup.h"

#include "Protocol.h"

#include <inttypes.h>
// { Commands
#define PROTO_INIT      0x494e493f  //  "?INI"
#define PROTO_MSG       0x47534d3f  //  "?MSG"
#define PROTO_DIGITAL_WRITE         0x5257443f  //  "?DWR
#define PROTO_DIGITAL_READ          0x4452443f  //  "?DRD
#define PROTO_ANALOG_WRITE          0x5257413f  //  "?AWR
#define PROTO_ANALOG_READ           0x4452413f  //  "?ARD
#define PROTO_DELAY                 0x5947443f  //  "?DLY
#define PROTO_MODE                  0x444f4d3f  //  "?MOD"
#define PROTO_REGISTER_BATCH        0x4745523f  //  "?REG"
#define PROTO_EXEC_BATCH            0x4558453f  //  "?EXE"
// } Commands

bool sync = false;

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

Buffer batch[BATCH_STACK_SIZE] = { };

void processBin(Stream& stream) {
    protocol::Packet packet;
    stream.readBytes((char*)&packet.magic, sizeof(uint32_t));
    stream.readBytes((char*)&packet.code, sizeof(uint16_t));
    if (packet.magic != PROTO_MAGIC) {
        while (stream.available()) {
            stream.read();
        }
        uint16_t code = 0xF0F1;
        stream.write((const uint8_t*)&code, sizeof(uint16_t));
        stream.write((const uint8_t*)&packet.magic, sizeof(uint32_t));
        return;
    }

    switch (packet.code) {
        case protocol::ProtoMode: {
            protocol::Mode body;
            stream.readBytes((char*)&body.pin, sizeof(uint8_t));
            stream.readBytes((char*)&body.mode, sizeof(uint8_t));
            pinMode(body.pin, body.mode);
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            break;
        }
        case protocol::ProtoDigitalWrite: {
            protocol::DigitalWrite body;
            stream.readBytes((char*)&body.pin, sizeof(uint8_t));
            stream.readBytes((char*)&body.value, sizeof(uint8_t));
            digitalWrite(body.pin, body.value);
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            break;
        }
        case protocol::ProtoAnalogWrite: {
            protocol::AnalogWrite body;
            stream.readBytes((char*)&body.pin, sizeof(uint8_t));
            stream.readBytes((char*)&body.value, sizeof(int));
            analogWrite(body.pin, body.value);
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            break;
        }
        case protocol::ProtoDigitalRead: {
            protocol::DigitalRead body;
            stream.readBytes((char*)&body.pin, sizeof(uint8_t));
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            stream.write(digitalRead(body.pin));
            break;
        }
        case protocol::ProtoAnalogRead: {
            protocol::AnalogWrite body;
            stream.readBytes((char*)&body.pin, sizeof(uint8_t));
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            stream.write(analogRead(body.pin));
            break;
        }
        case protocol::ProtoDelay: {
            protocol::Delay body;
            stream.readBytes((char*)&body.delay, sizeof(int));
            delay(body.delay);
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            break;
        }
        case protocol::ProtoMessage: {
            protocol::Message body;
            stream.readBytes((char*)&body.len, sizeof(uint8_t));
            char* msg = (char*)malloc(body.len);
            stream.readBytes((char*)&body.len, sizeof(uint8_t));
            Serial.readBytes(msg, body.len);
            // TODO: send message to display
            free(msg);
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            break;
        }
        case protocol::ProtoBatch: {
            protocol::RegisterBatch body;
            stream.readBytes((char*)&body.batch, sizeof(uint8_t));
            stream.readBytes((char*)&body.len, sizeof(uint8_t));
            if (body.batch < BATCH_STACK_SIZE) {
                batch[body.batch].reset();

                char* data = (char*)malloc(body.len);
                stream.readBytes(data, body.len);
                batch[body.batch] = Buffer(0, body.len, data);
            }
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            break;
        }
        case protocol::ProtoExecBatch: {
            protocol::ExecBatch body;
            int id = stream.read();
            if (id >= 0 && id < BATCH_STACK_SIZE) {
                Buffer buffer = batch[id];
                while (buffer.available()) {
                    processBin(buffer);
                }
            }
            stream.write((const uint8_t*)&body.code, sizeof(uint16_t));
            break;
        }
        default:
            // drop data from serial
            while (stream.available()) {
                stream.read();
            }
            stream.write(0xF0F0);
            break;
    }
}

void processStr(Stream& stream) {
    String packet = stream.readString();

    uint32_t code = *(uint32_t*)packet.begin();
    if (!sync && code != PROTO_INIT) {
        Serial.println("?HSE");
        return;
    }
    const char* data = packet.begin() + 4;
    while (*data == ' ') ++data;

    switch (code) {
        case PROTO_INIT:
            stream.println(packet);
            sync = true;
            break;
        case PROTO_ANALOG_WRITE:
        case PROTO_DIGITAL_WRITE:
        case PROTO_MODE:
        {
            int pin = 0, value = 0;
            int res = sscanf(data, "%d:%d", &pin, &value);
            if (res == 2) {
                if (code == PROTO_DIGITAL_WRITE) {
                    digitalWrite(pin, value > 0);
                } else if (code == PROTO_ANALOG_WRITE) {
                    analogWrite(pin, value);
                } else {
                    pinMode(pin, value);
                }

                stream.print(packet.substring(0, 4));
                stream.println(" OK");
            }
        }
            break;
        case PROTO_ANALOG_READ:
        case PROTO_DIGITAL_READ:
        {
            int pin = 0;
            int res = sscanf(data, "%d\r\n", &pin);
            if (res == 1) {
                if (code == PROTO_ANALOG_READ) {
                    res = analogRead(pin);
                } else {
                    res = digitalRead(pin);
                }
                stream.print(packet.substring(0, 4));
                stream.print(" OK ");
                stream.println(res);
            }
        }
            break;
        case PROTO_DELAY: {
            uint32_t duration;
            int res = sscanf(data, "%d\r\n", &duration);
            delay(duration);
            break;
        }
        case PROTO_MSG: {
            // TODO: send message to display
            break;
        }
        case PROTO_EXEC_BATCH: {
            int cmd = 0;
            int res = sscanf(data, "%d\r\n", &cmd);
            if (res == 1) {
                if (cmd > 0 && cmd < BATCH_STACK_SIZE) {
                    Buffer buffer = batch[cmd];
                    while (buffer.available()) {
                        processBin(buffer);
                    }
                }
            }
            break;
        }
        default:
            stream.println("?UNK");
            break;
    }
}


void setup() {
    Serial.begin(57600);
}

void loop() {
    if (Serial.getWriteError() > 0) {
        sync = false;
        Serial.clearWriteError();
    }
    if (Serial.available()) {
        processBin(Serial);
    }
}
