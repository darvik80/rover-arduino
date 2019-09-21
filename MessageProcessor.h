//
// Created by Ivan Kishchenko on 21/09/2019.
//

#ifndef ROVER_ARDUINO_MESSAGEPROCESSOR_H
#define ROVER_ARDUINO_MESSAGEPROCESSOR_H

#include <WString.h>
#include <Stream.h>
#include "Buffer.h"
#include "Protocol.h"
#include "LiquidCrystal_I2C.h"

enum MessageProcessStatus {
    MP_INIT,
    MP_HEADER,
    MP_BODY,
};

#define HDR_SIZE  (sizeof(uint32_t) + sizeof(uint16_t))

uint8_t readByte(Stream& stream);

uint16_t readWord(Stream& stream);

uint32_t readDword(Stream& stream);

void writeByte(Stream& stream, uint8_t data);

void writeWord(Stream& stream, uint16_t data);

void writeDword(Stream& stream, uint32_t data);

class MessageProcessor {
private:
    MessageProcessStatus _status;
    protocol::Packet _packet;
    LiquidCrystal_I2C *_lcd;
public:
    explicit MessageProcessor(LiquidCrystal_I2C *lcd) : _status(MP_INIT), _lcd(lcd) {}

    void process(Stream &stream);

    static void processMode(protocol::Packet& packet, Stream &stream);

    static void processDigitalWrite(protocol::Packet& packet, Stream &stream);

    static void processDigitalRead(protocol::Packet& packet, Stream &stream);

    static void processAnalogWrite(protocol::Packet& packet, Stream &stream);

    static void processAnalogRead(protocol::Packet& packet, Stream &stream);

    static void processDelay(protocol::Packet& packet, Stream &stream);

    static void processMessage(protocol::Packet& packet, Stream &stream);

    static void processRegisterBatch(protocol::Packet& packet, Stream &stream);

    static void processExecBatch(protocol::Packet& packet, Stream &stream);

    void processBin(Stream &stream);

    void message(const char* str);
    void message(const String& str) {
        message(str.c_str());
    }
};

#endif //ROVER_ARDUINO_MESSAGEPROCESSOR_H
