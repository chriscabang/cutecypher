#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>

static inline void printh(uint8_t *hash) {
  for (int i = 0; i < 64; i++)
    printf("%02x", hash[i]);
  printf("\n");
}

static uint8_t hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0; // Invalid character
}

static void hex_string_to_byte_array(const char *hex_string, uint8_t *byte_array, size_t byte_array_len) {
    for (size_t i = 0; i < byte_array_len; i++) {
        byte_array[i] = (hex_char_to_value(hex_string[2 * i]) << 4) | hex_char_to_value(hex_string[2 * i + 1]);
    }
}


#endif//UTILS_H
