#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "cute_sha512.h"

#define NUM_TESTS 10000
#define BIT_SIZE  256

int bit_diff_between(const uint8_t hash[64], const uint8_t __hash[64] /*, size_t size */) {
  int c = 0;
  for (size_t i = 0; i < 64; i++) {
    uint8_t d = hash[i] ^ __hash[i];
    for (int j = 0; j < 8; j++) {
      if (d & (1 << j)) c++;
    }
  }
  return c;
}

void generate_256bits_random(uint8_t *r) {
  for (int i = 0; i < 32; i++) r[i] = rand() % 256;
}

void flip_bit(uint8_t *data, int bit_index) {
  int byte_index = bit_index / 8;
  int bit_in_byte = bit_index % 8;
  data[byte_index] ^= (1 << bit_in_byte);
}

int main(void) {
  uint8_t original[32];
  uint8_t modified[32];
  uint8_t hash[64], __hash[64];

  int histogram[513] = { 0 };

  srand(time(0));

  for (int i = 0; i < NUM_TESTS; i++) {
    generate_256bits_random(original);
    memcpy(modified, original, 32);
    int random_bit = rand() % BIT_SIZE;
    flip_bit(modified, random_bit);

    cute_sha512_ctx ctx;

    cute_sha512_init(&ctx);
    cute_sha512_update(&ctx, (const uint8_t *) original, 32);
    cute_sha512_final(&ctx, hash);

    cute_sha512_init(&ctx);
    cute_sha512_update(&ctx, (const uint8_t *) modified, 32);
    cute_sha512_final(&ctx, __hash);

    int diff = bit_diff_between(hash, __hash);

    histogram[diff]++;
  }

  for (int i = 0; i <= 512; i++) {
    if (histogram[i] > 0) printf ("%d bit difference: %d occurrences\n", i, histogram[i]);
  }

  return 0;
}
