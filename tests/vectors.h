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
  const char *salt;
  const char *expected;
};

// A fox jumps over the lazy dog + 5P0n9b0b5qu@r3P@nt5
// B45E1A776384D53368DD4869F32466C38E605E6995AAD25A6D221973BB07BF2A17F055BF0D87471D9E069D2EF01E3E51AACB8F1A135C384A92E7D4EF0520E8AB
// abc + 5P0n9b0b5qu@r3P@nt5
// 2E16C237EB058414BC6859C6E2E2143C63DB406F20349F3DAED749FEAC699A217913F1D41A0CD8DF1FE557EA8C8B48C878FF6124DDAF907FE72AD1E1FEE67049
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
        "abc", 0,
        "ddaf35a193617abacc417349ae204131"
        "12e6fa4e89a97ea20a9eeee64b55d39a"
        "2192992a274fc1a836ba3c23a3feebbd"
        "454d4423643ce80e2a9ac94fa54ca49f"
      },
      {
        "", 0,
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
