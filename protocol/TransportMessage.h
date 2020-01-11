//
// Created by Kishchenko, Ivan on 1/9/20.
//

#ifndef ARDUINO_TRANSPORT_PROTOCOL_H
#define ARDUINO_TRANSPORT_PROTOCOL_H

#include "utils/memory/Buffer.h"
#include "utils/Writer.h"
#include "utils/Reader.h"
#include <utils/crc16.h>

namespace protocol {

#define PROTO_MAGIC 0x4441

    class TransportMessage {
    private:
        Buffer& _buffer;
    public:
        TransportMessage() = delete;
        TransportMessage(const TransportMessage& rhs) = delete;
        TransportMessage& operator = (const TransportMessage& rhs) = delete;

        TransportMessage(Buffer& buffer) : _buffer(buffer) {};

        bool encode(Writer& writer) {
            if (writer.write(PROTO_MAGIC) == 0) {
                return false;
            }

            uint16_t length = _buffer.getSize();
            if (writer.write(length) == 0) {
                return false;
            }
            if (writer.write(crc16(_buffer)) == 0) {
                return false;
            }

            return writer.write(_buffer);
        }

        bool decode(Reader& reader) {
            if (reader.available() < sizeof(uint16_t)*3) {
                return false;
            }

            uint16_t header;
            if (reader.read(header) == 0) {
                return false;
            }
            if (header != PROTO_MAGIC) {
                return false;
            }

            size_t length{0};
            if (reader.read(length) == 0) {
                return false;
            }
            uint16_t dataCrc16{0};
            if (reader.read(dataCrc16) == 0) {
                return false;
            }

            if (reader.read(_buffer, length) == 0) {
                return false;
            }

            return dataCrc16 == crc16(_buffer);
        }

        [[nodiscard]] Buffer &getBuffer() {
            return _buffer;
        }
    };

}

#endif //ARDUINO_TRANSPORT_PROTOCOL_H
