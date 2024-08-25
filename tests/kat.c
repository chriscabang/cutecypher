#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cute_sha512.h"
#include "vectors.h"

// :TODO: 
// - Move utility functions somewhere
// - Utility functions should be static

int kat_sha512(const char *message, const char *expected_hash);

struct known_answer_tests {
  enum SHAfamily version;
  int (*operation)(const char *, const char *);
} kats[] = {
  { SHA512, kat_sha512 }
};

void printh(uint8_t *hash) {
  for (int i = 0; i < 64; i++)
    printf("%02x", hash[i]);
  printf("\n");
}

uint8_t hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0; // Invalid character
}

void hex_string_to_byte_array(const char *hex_string, uint8_t *byte_array, size_t byte_array_len) {
    for (size_t i = 0; i < byte_array_len; i++) {
        byte_array[i] = (hex_char_to_value(hex_string[2 * i]) << 4) | hex_char_to_value(hex_string[2 * i + 1]);
    }
}

int hashcmp(const uint8_t *hash1, const uint8_t *hash2, size_t hash_len) {
  int res = 1;
  for (size_t i = 0; i < hash_len; i++) {
    if (hash1[i] != hash2[i]) {
      res = 0; // Hashes are not equal
    }
  }
  return res; // Hashes are equal
}

int kat_sha512(const char *message, const char *expected_hash) {
  uint8_t hash[64];
  cute_sha512_ctx ctx;

  cute_sha512_init(&ctx);
  cute_sha512_update(&ctx, (const uint8_t *) message, strlen(message));
  cute_sha512_final(&ctx, hash);

  // Print the results
  printf("Input: %s\nExpected: %s\n", message, expected_hash);
  printf("Received: "); printh(hash);

  uint8_t expected[64];
  hex_string_to_byte_array(expected_hash, expected, 64);

  return hashcmp(expected, hash, 64);
}

static void kat_test() {
  size_t n = sizeof(kats) / sizeof(kats[0]);
  for (int i = 0; i < (int)n; i++) {
    struct Hash hash = find_hash(kats[i].version);
    for (int t = 0; t < TESTCOUNT; t++) {
      if (!kats[i].operation(hash.v[t].input, hash.v[t].expected)) {
        printf ("test failed\n");
      } else {
        printf("test passed\n");
      }
    }
  }
}

int main() {

  kat_test();

  return 0;
}
