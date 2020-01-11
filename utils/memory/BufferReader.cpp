//
// Created by Kishchenko, Ivan on 1/10/20.
//

#include <string.h>
#include "BufferReader.h"

size_t BufferReader::available() {
   return remaining();
}

size_t BufferReader::readBytes(uint8_t* data, size_t size) {
    if (remaining() < size) {
        return 0;
    }

    memcpy(data, getData() + getOffset(), size);
    setOffset(getOffset() + size);

    return size;
}

