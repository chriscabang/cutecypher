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
  const char *expected;
};

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
        "5P0n9b0b5qu@r3P@nt5abc",
        "24339ecda3d58ce27c7f073293ca13a4"
        "b51e99b6edd2afcb94b551af0d670539"
        "83bda56a0afdc0a9a0f9386514593488"
        "d2a2be6ae7bef99338da16ab234adcba"
      },
      {
        "",
        "cf83e1357eefb8bdf1542850d66d8007"
        "d620e4050b5715dc83f4a921d36ce9ce"
        "47d0d13c5d85f2b0ff8318d2877eec2f"
        "63b931bd47417a81a538327af927da3e"
      },
      {
        "5P0n9b0b5qu@r3P@nt5A fox jumps over the lazy dog", 
        "3185346b571dad5d3b3a3a2d7daaa344"
        "dcef217372d9c8cb2304e523e9f3b257"
        "35a80a8f00da7b4e434d34f48304e23b"
        "b0981d67893c16d171f2eeb76bc694e7"
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
