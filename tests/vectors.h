#ifndef VECTORS_H
#define VECTORS_H

#include <stdint.h>

#define HASHCOUNT 2
#define TESTCOUNT 4

enum SHAfamily {
  SHA224 = 1, SHA256, SHA384, SHA512, SHA1, SHA2, SHA3
};

enum SHAlength {
  SHA224length = 28,
  SHA256length = 32,
  SHA384length = 48,
  SHA512length = 64,
  SHA1length = 20
};

struct vector {
  const char *input;
  const char *salt;
  const char *expected;
};

// A fox jumps over the lazy dog + 5P0n9b0b5qu@r3P@nt5
// b45e1a776384d53368dd4869f32466c38e605e6995aad25a6d221973bb07bf2a17f055bf0d87471d9e069d2ef01e3e51aacb8f1a135c384a92e7d4ef0520e8ab
// abc + 5P0n9b0b5qu@r3P@nt5
// 2e16c237eb058414bc6859c6e2e2143c63db406f20349f3daed749feac699a217913f1d41a0cd8df1fe557ea8c8b48c878ff6124ddaf907fe72ad1e1fee67049
// Test arrays for hashes
struct Hash {
  enum SHAfamily name;
  int size;
  struct vector v[TESTCOUNT];
} hashes[HASHCOUNT] = {
  {
    0
  },
  {
    SHA512, SHA512length,
    { 
      {
        "abc",
        0,
        "ddaf35a193617abacc417349ae204131"
        "12e6fa4e89a97ea20a9eeee64b55d39a"
        "2192992a274fc1a836ba3c23a3feebbd"
        "454d4423643ce80e2a9ac94fa54ca49f"
      },
      {
        "abc",
        "5P0n9b0b5qu@r3P@nt5",
        "2e16c237eb058414bc6859c6e2e2143c"
        "63db406f20349f3daed749feac699a21"
        "7913f1d41a0cd8df1fe557ea8c8b48c8"
        "78ff6124ddaf907fe72ad1e1fee67049"
      },
      {
        "",
        0,
        "cf83e1357eefb8bdf1542850d66d8007"
        "d620e4050b5715dc83f4a921d36ce9ce"
        "47d0d13c5d85f2b0ff8318d2877eec2f"
        "63b931bd47417a81a538327af927da3e"
      },
      {
        "A fox jumps over the lazy dog",
        "5P0n9b0b5qu@r3P@nt5", 
        "b45e1a776384d53368dd4869f32466c3"
        "8e605e6995aad25a6d221973bb07bf2a"
        "17f055bf0d87471d9e069d2ef01e3e51"
        "aacb8f1a135c384a92e7d4ef0520e8ab"
      }
    }
  }
};

struct Hash find_hash(enum SHAfamily name) {
  struct Hash h = hashes[0];
  for (int i = 0; i < HASHCOUNT; i++) {
    if (hashes[i].name == name) {
      h = hashes[i];
      break;
    }
  }
  return h;
}

#endif//VECTORS_H
