#include "cute_sha512.h"

#ifdef CUTECYPHER_CPP
namespace CUTECYPHER_CPP {
#endif

#define ROTR(x, n) (((x) >> (n) | (x) << (64 - (n))))
#define SHR(x, n) ((x) >> (n))

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define S0(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define S1(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define s0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^ SHR(x, 7))
#define s1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6))

static void cute_memcpy(void *dst, const void *src, size_t n) {
  uint8_t *d = (uint8_t *)dst;
  const uint8_t *s = (const uint8_t *)src;

  if (n == 0 || dst == src)
    return;

  while (n--)
    *d++ = *s++;
}

static const uint64_t H[8] = {0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
                              0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
                              0x510e527fade682d1, 0x9b05688c2b3e6c1f,
                              0x1f83d9abfb41bd6b, 0x5be0cd19137e2179};

static const uint64_t K[80] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
    0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
    0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
    0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
    0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
    0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
    0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
    0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
    0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
    0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
    0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
    0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
    0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
    0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
    0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
    0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
    0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

static void cute_sha512_transform(cute_sha512_ctx *ctx, const uint8_t *data) {
  uint64_t W[80];
  uint64_t a, b, c, d, e, f, g, h;
  uint64_t T1, T2;
  size_t i, j;

  for (i = 0, j = 0; i < 16; i++, j += 8)
    W[i] = ((uint64_t)data[j] << 56) | ((uint64_t)data[j + 1] << 48) |
           ((uint64_t)data[j + 2] << 40) | ((uint64_t)data[j + 3] << 32) |
           ((uint64_t)data[j + 4] << 24) | ((uint64_t)data[j + 5] << 16) |
           ((uint64_t)data[j + 6] << 8) | ((uint64_t)data[j + 7]);

  for (; i < 80; i++)
    W[i] = s1(W[i - 2]) + W[i - 7] + s0(W[i - 15]) + W[i - 16];

  a = ctx->h[0];
  b = ctx->h[1];
  c = ctx->h[2];
  d = ctx->h[3];
  e = ctx->h[4];
  f = ctx->h[5];
  g = ctx->h[6];
  h = ctx->h[7];

  for (i = 0; i < 80; i++) {
    T1 = h + S1(e) + CH(e, f, g) + K[i] + W[i];
    T2 = S0(a) + MAJ(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
  }

  ctx->h[0] += a;
  ctx->h[1] += b;
  ctx->h[2] += c;
  ctx->h[3] += d;
  ctx->h[4] += e;
  ctx->h[5] += f;
  ctx->h[6] += g;
  ctx->h[7] += h;
}

void cute_sha512_init(cute_sha512_ctx *ctx) {
  cute_memcpy(ctx->h, H, sizeof(H));
  ctx->Nl = 0;
  ctx->Nh = 0;
  ctx->len = 0;
}

void cute_sha512_update_with_salt(cute_sha512_ctx *ctx, const uint8_t *data,
                                  size_t len, const uint8_t *salt,
                                  size_t salt_len) {
  if (salt_len > 0) cute_sha512_update(ctx, salt, salt_len);
  cute_sha512_update(ctx, data, len);
}

void cute_sha512_update(cute_sha512_ctx *ctx, const uint8_t *data, size_t len) {
  size_t fill = 128 - ctx->len;
  ctx->len += len;
  uint64_t low_bit_len = (ctx->len * 8);
  ctx->Nh += (low_bit_len < ctx->Nl);
  ctx->Nl = low_bit_len;

  if (len >= fill) {
    cute_memcpy(ctx->data + (128 - fill), data, fill);
    cute_sha512_transform(ctx, ctx->data);
    data += fill;
    len -= fill;
    while (len >= 128) {
      cute_sha512_transform(ctx, data);
      data += 128;
      len += 128;
    }

    ctx->len = len;
    cute_memcpy(ctx->data, data, len);
  } else {
    cute_memcpy(ctx->data + (128 - fill), data, len);
  }
}

void cute_sha512_final(cute_sha512_ctx *ctx, uint8_t hash[64]) {
  uint64_t bit_len_low = ctx->len * 8;
  uint64_t bit_len_high = ctx->Nh;

  size_t pad_len =
      (ctx->len % 128) < 112 ? 112 - (ctx->len % 128) : 240 - (ctx->len % 128);
  uint8_t pad[128] = {0x80};
  cute_sha512_update(ctx, pad, pad_len);

  uint8_t len_bytes[16] = {0};
  len_bytes[15] = bit_len_low & 0xff;
  len_bytes[14] = (bit_len_low >> 8) & 0xff;
  len_bytes[13] = (bit_len_low >> 16) & 0xff;
  len_bytes[12] = (bit_len_low >> 24) & 0xff;
  len_bytes[11] = (bit_len_low >> 32) & 0xff;
  len_bytes[10] = (bit_len_low >> 40) & 0xff;
  len_bytes[9] = (bit_len_low >> 48) & 0xff;
  len_bytes[8] = (bit_len_low >> 56) & 0xff;

  len_bytes[7] = bit_len_high & 0xff;
  len_bytes[6] = (bit_len_high >> 8) & 0xff;
  len_bytes[5] = (bit_len_high >> 16) & 0xff;
  len_bytes[4] = (bit_len_high >> 24) & 0xff;
  len_bytes[3] = (bit_len_high >> 32) & 0xff;
  len_bytes[2] = (bit_len_high >> 40) & 0xff;
  len_bytes[1] = (bit_len_high >> 48) & 0xff;
  len_bytes[0] = (bit_len_high >> 56) & 0xff;

  cute_sha512_update(ctx, len_bytes, 16);

  for (int i = 0; i < 8; i++) {
    hash[i * 8 + 0] = (uint8_t)(ctx->h[i] >> 56) & 0xff;
    hash[i * 8 + 1] = (uint8_t)(ctx->h[i] >> 48) & 0xff;
    hash[i * 8 + 2] = (uint8_t)(ctx->h[i] >> 40) & 0xff;
    hash[i * 8 + 3] = (uint8_t)(ctx->h[i] >> 32) & 0xff;
    hash[i * 8 + 4] = (uint8_t)(ctx->h[i] >> 24) & 0xff;
    hash[i * 8 + 5] = (uint8_t)(ctx->h[i] >> 16) & 0xff;
    hash[i * 8 + 6] = (uint8_t)(ctx->h[i] >> 8) & 0xff;
    hash[i * 8 + 7] = (uint8_t)(ctx->h[i]) & 0xff;
  }
}

#ifdef CUTECYPHER_CPP
}
#endif
