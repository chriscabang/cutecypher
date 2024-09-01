#ifndef VECTORS_H
#define VECTORS_H

#include <stdint.h>

#define HASHCOUNT 2
#define TESTCOUNT 2

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
  const char *expected;
};

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
        "ddaf35a193617abacc417349ae204131"
        "12e6fa4e89a97ea20a9eeee64b55d39a"
        "2192992a274fc1a836ba3c23a3feebbd"
        "454d4423643ce80e2a9ac94fa54ca49f"
      },
      {
        "", 
        "cf83e1357eefb8bdf1542850d66d8007"
        "d620e4050b5715dc83f4a921d36ce9ce"
        "47d0d13c5d85f2b0ff8318d2877eec2f"
        "63b931bd47417a81a538327af927da3e"
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
