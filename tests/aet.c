#include <stdio.h>
#include <string.h>

#include "cute_sha512.h"

int bit_diff_between(const uint8_t hash1[64], const uint8_t hash2[64]) {
  int c = 0;
  for (int i = 0; i < 64; i++) {
    uint8_t d = hash1[i] ^ hash2[i];
    for (int j = 0; j < 8; j++) {
      if (d & (1 << j)) c++;
    }
  }
  return c;
}

int avalanche_effect_test(const char *message) {
  uint8_t hash1[64], hash2[64];
  size_t len = strlen(message);

  cute_sha512_ctx ctx;

  cute_sha512_init(&ctx);
  cute_sha512_update(&ctx, (const uint8_t *)message, strlen(message));
  cute_sha512_final(&ctx, hash1);
  
  char __message[len + 1];
  strcpy(__message, message);

  __message[0] ^= 0x01;

  cute_sha512_init(&ctx);
  cute_sha512_update(&ctx, (const uint8_t *)message, strlen(message));
  cute_sha512_final(&ctx, hash2);

  return bit_diff_between(hash1, hash2);
}

int main(void) {
  avalanche_effect_test("abc");
  return 0;
}
