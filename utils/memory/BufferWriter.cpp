//
// Created by Kishchenko, Ivan on 1/10/20.
//

#include <string.h>
#include "BufferWriter.h"

size_t BufferWriter::writeBytes(const uint8_t* data, size_t size) {
    if (remaining() < size) {
        return 0;
    }

    memcpy((void*)(getData() + getOffset()), (void*)data, size);
    setOffset(getOffset() + size);

    return size;
}