//
// Created by Kishchenko, Ivan on 1/10/20.
//

#include <string.h>
#include "Buffer.h"

void Buffer::setOffset(size_t offset) {
    if (offset < getSize()) {
        _offset = offset;
    }
}

void Buffer::skip(size_t block) {
    if (getOffset() + block < getSize()) {
        _offset += block;
    }
}

void Buffer::back(size_t block) {
    if (getOffset() > block) {
        _offset -= block;
    }
}