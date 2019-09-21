//
// Created by Ivan Kishchenko on 21/09/2019.
//

#include "MessageProcessor.h"
#include "Protocol.h"
#include <Arduino.h>

Buffer batch[BATCH_STACK_SIZE] = { };

void MessageProcessor::process(Stream &stream) {
    if (&stream == &Serial) {
        if (Serial.getWriteError() > 0) {
            Serial.clearWriteError();
            _status == MP_INIT;
            while (stream.available()) {
                stream.read();
            }
        }
    }

    if (_status == MP_INIT) {
        if (stream.available() >= HDR_SIZE) {
            _packet.magic = readDword(stream);
            _packet.code = readByte(stream);
            _packet.size = readByte(stream);

            if (_packet.magic != PROTO_MAGIC) {
                while (stream.available()) {
                    stream.read();
                }
                message(String("WRONG1 ") + String(_packet.magic, 16));
                writeByte(stream, 0xFE);
                return;
            }

            _status = MP_HEADER;
        }
    } else if (_status == MP_HEADER) {
        if (stream.available() >= _packet.size) {
            _status = MP_BODY;
            processBin(stream);
            _status = MP_INIT;
        }
    }
}

void MessageProcessor::processMode(protocol::Packet &packet, Stream &stream) {
    protocol::Mode body;
    body.pin = readByte(stream);
    body.mode = readByte(stream);

    pinMode(body.pin, body.mode);
    writeByte(stream, body.code);
}

void MessageProcessor::processDigitalWrite(protocol::Packet &packet, Stream &stream) {
    protocol::DigitalWrite body;
    body.pin = readByte(stream);
    body.value = readByte(stream);
    digitalWrite(body.pin, body.value);
    writeByte(stream, body.code);
}

void MessageProcessor::processDigitalRead(protocol::Packet &packet, Stream &stream) {
    protocol::DigitalRead body;
    body.pin = readByte(stream);
    writeByte(stream, body.code);
    writeByte(stream, digitalRead(body.pin));
}

void MessageProcessor::processAnalogWrite(protocol::Packet &packet, Stream &stream) {
    protocol::AnalogWrite body;
    body.pin = readByte(stream);
    body.value = readWord(stream);
    analogWrite(body.pin, body.value);
    writeByte(stream, body.code);
}

void MessageProcessor::processAnalogRead(protocol::Packet &packet, Stream &stream) {
    protocol::AnalogRead body;
    body.pin = readByte(stream);
    writeByte(stream, body.code);
    writeWord(stream, analogRead(body.pin));
}

void MessageProcessor::processDelay(protocol::Packet &packet, Stream &stream) {
    protocol::Delay body;
    body.delay = readWord(stream);
    delay(body.delay);
    writeByte(stream, body.code);
}

void MessageProcessor::processMessage(protocol::Packet &packet, Stream &stream) {
    protocol::Message body;
    char *msg = (char *) malloc(body.size);
    Serial.readBytes(msg, body.size);
    free(msg);
    writeByte(stream, body.code);
}

void MessageProcessor::processRegisterBatch(protocol::Packet &packet, Stream &stream) {
    protocol::RegisterBatch body;
    stream.readBytes((char *) &body.batch, sizeof(uint8_t));
    if (body.batch < BATCH_STACK_SIZE) {
        batch[body.batch].reset();
        size_t dataSize = packet.size-1;
        char *data = (char *) malloc(dataSize);
        stream.readBytes(data, dataSize);
        batch[body.batch] = Buffer(0, dataSize, data);
    }
    writeByte(stream, body.code);
}

void MessageProcessor::processExecBatch(protocol::Packet &packet, Stream &stream) {
    protocol::ExecBatch body;
    int id = readByte(stream);
    if (id >= 0 && id < BATCH_STACK_SIZE) {
        Buffer buffer = batch[id];
        MessageProcessor processor(nullptr);
        while (buffer.available()) {
            processor.process(buffer);
        }
    }
    writeByte(stream, body.code);
}

void MessageProcessor::processBin(Stream &stream) {
    switch (_packet.code) {
        case protocol::ProtoMode: {
            processMode(_packet, stream);
            break;
        }
        case protocol::ProtoDigitalWrite: {
            processDigitalWrite(_packet, stream);
            break;
        }
        case protocol::ProtoAnalogWrite: {
            processAnalogWrite(_packet, stream);
            break;
        }
        case protocol::ProtoDigitalRead: {
            processDigitalRead(_packet, stream);
            break;
        }
        case protocol::ProtoAnalogRead: {
            processAnalogRead(_packet, stream);
            break;
        }
        case protocol::ProtoDelay: {
            processDelay(_packet, stream);
            break;
        }
        case protocol::ProtoMessage: {
            processMessage(_packet, stream);
            break;
        }
        case protocol::ProtoBatch: {
            processRegisterBatch(_packet, stream);
            break;
        }
        case protocol::ProtoExecBatch: {
            processExecBatch(_packet, stream);
            break;
        }
        default:
            // drop data from serial
            message(String("WRONG2 ") + stream.available());
            while (stream.available()) {
                stream.read();
            }
            writeByte(stream, 0xFF);
            break;
    }
}

void MessageProcessor::message(const char* str) {
    if (_lcd != nullptr) {
        _lcd->clear();
        _lcd->setCursor(0, 0);
        _lcd->print(str);
    }
}


uint8_t readByte(Stream &stream) {
    return stream.read();
}

uint16_t readWord(Stream &stream) {
    uint16_t data =  (uint16_t)stream.read() & 0x00FF;
    data |=  (uint16_t)stream.read() << 8 & 0xFF00;

    return data;
}

uint32_t readDword(Stream &stream) {
    uint32_t data;
    stream.readBytes((uint8_t *)&data, sizeof(uint32_t));

    return data;
}

void writeByte(Stream &stream, uint8_t data) {
    stream.write(data);
}

void writeWord(Stream &stream, uint16_t data) {
    stream.write((char*)&data, sizeof(uint16_t));
}

void writeDword(Stream &stream, uint32_t data) {
    stream.write((char*)&data, sizeof(uint32_t));
}
