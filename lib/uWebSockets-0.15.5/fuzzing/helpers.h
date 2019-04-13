#ifndef HELPERS_H
#define HELPERS_H

/* Common helpers for fuzzing */

#include <functional>
#include <string_view>
#include <cstring>

/* We use this to pad the fuzz */
static inline const uint8_t *makePadded(const uint8_t *data, size_t size) {
    static char *padded = new char[1024  * 500];

    memcpy(padded, data, size);

    return (uint8_t *) padded;
}

/* Splits the fuzz data in one or many chunks */
static inline void makeChunked(const uint8_t *data, size_t size, std::function<void(const uint8_t *data, size_t size)> cb) {
    /* First byte determines chunk size; 0 is all that remains, 1-255 is small chunk */
    for (int i = 0; i < size; ) {
        unsigned int chunkSize = data[++i];
        if (!chunkSize) {
            chunkSize = size - i;
        } else {
            chunkSize = std::min<int>(chunkSize, size - i);
        }

        cb(data + i, chunkSize);
        i += chunkSize;
    }
}

/* Reads all bytes to trigger invalid reads */
static inline void readBytes(std::string_view s) {
    volatile int sum = 0;
    for (int i = 0; i < s.size(); i++) {
        sum += s[i];
    }
}

#endif
