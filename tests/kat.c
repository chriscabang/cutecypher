#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cute_sha512.h"
#include "utils.h"
#include "vectors.h"

int kat_sha512(const char *message, const char *salt,
               const char *expected_hash);

struct known_answer_tests {
  enum SHAfamily version;
  int (*operation)(const char *, const char *, const char *);
} kats[] = {{SHA512, kat_sha512}};

int hashcmp(const uint8_t *hash1, const uint8_t *hash2, size_t hash_len) {
  int res = 1;
  for (size_t i = 0; i < hash_len; i++) {
    if (hash1[i] != hash2[i]) {
      res = 0; // Hashes are not equal
    }
  }
  return res; // Hashes are equal
}

int kat_sha512(const char *message, const char *salt,
               const char *expected_hash) {
  uint8_t hash[SHA512length];
  cute_sha512_ctx ctx;

  cute_sha512_init(&ctx);
  cute_sha512_update_with_salt(&ctx, (const uint8_t *)message, strlen(message),
                               (const uint8_t *)salt, salt != 0 ? strlen(salt) : 0);
  // cute_sha512_update(&ctx, (const uint8_t *) message, strlen(message));
  cute_sha512_final(&ctx, hash);

  // Print the results
  printf("Input: %s\nExpected: %s\n", message, expected_hash);
  printf("Received: ");
  printh(hash);

  uint8_t expected[64];
  hex_string_to_byte_array(expected_hash, expected, 64);

  return hashcmp(expected, hash, 64);
}

static int kat_test() {
  int res = 0;
  size_t n = sizeof(kats) / sizeof(kats[0]);
  for (int i = 0; i < (int)n; i++) {
    struct Hash hash = find_hash(kats[i].version);
    for (int t = 0; t < TESTCOUNT; t++) {
      if (!kats[i].operation(hash.v[t].input, hash.v[t].salt,
                             hash.v[t].expected)) {
        res = 1;
      }
    }
  }
  return res;
}

int main() {

  return kat_test();

}
