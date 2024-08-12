#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cutecypher.h"

void print_hash(uint8_t *hash) {
  for (int i = 0; i < 64; i++)
    printf("%02x", hash[i]);
  printf("\n");
}

int main() {
  // Known test vectors for SHA-512
  struct {
    const char *input;
    const char *expected_hash;
  } tests[] = {
      {"abc",
       "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
       "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f"},
      {"", "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
           "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e"},
      {"The quick brown fox jumps over the lazy dog",
       "07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb64"
       "2e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6"},
      {"The richest man in Babylon",
       "bf69a68e0366275ab72573d900ab0fb9fa62ff5ee7e0b04542f8a5e7856d76dc"
       "04716ceced7135dccdf0060dacc25cc55adac2434604a735a12575dfd46067ed"}};

  uint8_t hash[64];
  cute_sha512_ctx ctx;

  for (int i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) {
    // Initialize the context
    cute_sha512_init(&ctx);

    // Update the context with the test input
    cute_sha512_update(&ctx, (const uint8_t *)tests[i].input,
                       strlen(tests[i].input));

    // Finalize the hash
    cute_sha512_final(&ctx, hash);

    // Print the results
    printf("Test %d\n", i + 1);
    printf("Input: %s\nExpected: %s\n", tests[i].input, tests[i].expected_hash);
    printf("Received: ");
    print_hash(hash);

    // Compare the generated hash with the expected hash
    char hash_string[129]; // Each byte is 2 hex digits + null terminator
    for (int j = 0; j < 64; j++) {
      sprintf(hash_string + j * 2, "%02x", hash[j]);
    }
    if (strcmp(hash_string, tests[i].expected_hash) == 0) {
      printf("PASS\n");
    } else {
      printf("FAIL\n");
    }
  }

  return 0;
}
