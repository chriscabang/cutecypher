#ifndef CUTECYPHER_H
#define CUTECYPHER_H

#include <stdint.h>
#include <stddef.h>

#ifdef CUTECYPHER_CPP
namespace CUTECYPHER_CPP {
#elif defined(__cplusplus)
extern "C" {
#endif

// Hashing: SHA-512

typedef struct {
  uint64_t h[8];      // Hash state
  uint64_t Nl, Nh;    // Low and high parts of message length (in bits)
  uint8_t data[128];  // Data block to process
  size_t len;         // Length of data
} cute_sha512_ctx;

void cute_sha512_init(cute_sha512_ctx *c);

void cute_sha512_update(cute_sha512_ctx *c, const uint8_t *data, size_t len);

void cute_sha512_final(cute_sha512_ctx *c, uint8_t *hash);

#ifdef __cplusplus
}
#endif

#endif // CUTECYPHER_H
