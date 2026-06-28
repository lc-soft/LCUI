/*
   SipHash reference C implementation

   Copyright (c) 2012-2016 Jean-Philippe Aumasson
   <jeanphilippe.aumasson@gmail.com>
   Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
   Copyright (c) 2017 Salvatore Sanfilippo <antirez@gmail.com>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along
   with this software. If not, see
   <http://creativecommons.org/publicdomain/zero/1.0/>.

   ----------------------------------------------------------------------------

   This version was modified by Salvatore Sanfilippo <antirez@gmail.com>
   in the following ways:

   1. We use SipHash 1-2. This is not believed to be as strong as the
      suggested 2-4 variant, but AFAIK there are not trivial attacks
      against this reduced-rounds version, and it runs at the same speed
      as Murmurhash2 that we used previously, while the 2-4 variant slowed
      down Redis by a 4% figure more or less.
   2. Hard-code rounds in the hope the compiler can optimize it more
      in this raw from. Anyway we always want the standard 2-4 variant.
   3. Modify the prototype and implementation so that the function directly
      returns an uint64_t value, the hash itself, instead of receiving an
      output buffer. This also means that the output size is set to 8 bytes
      and the 16 bytes output code handling was removed.
   4. Provide a case insensitive variant to be used when hashing strings that
      must be considered identical by the hash table regardless of the case.
      If we don't have directly a case insensitive hash function, we need to
      perform a text transformation in some temporary buffer, which is costly.
   5. Remove debugging code.
   6. Modified the original test.c file to be a stand-alone function testing
      the function in the new form (returning an uint64_t) using just the
      relevant test vector.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Fast tolower() alike function that does not care about locale
 * but just returns a-z instead of A-Z. */
int siptlw(int c)
{
	if (c >= 'A' && c <= 'Z') {
		return c + ('a' - 'A');
	} else {
		return c;
	}
}

/* Test of the CPU is Little Endian and supports not aligned accesses.
 * Two interesting conditions to speedup the function that happen to be
 * in most of x86 servers. */
#if defined(__X86_64__) || defined(__x86_64__) || defined(__i386__) || \
    defined(__aarch64__) || defined(__arm64__)
#define UNALIGNED_LE_CPU
#endif

#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

#define U32TO8_LE(p, v)                \
	(p)[0] = (uint8_t)((v));       \
	(p)[1] = (uint8_t)((v) >> 8);  \
	(p)[2] = (uint8_t)((v) >> 16); \
	(p)[3] = (uint8_t)((v) >> 24);

#define U64TO8_LE(p, v)                  \
	U32TO8_LE((p), (uint32_t)((v))); \
	U32TO8_LE((p) + 4, (uint32_t)((v) >> 32));

#ifdef UNALIGNED_LE_CPU
#define U8TO64_LE(p) (*((uint64_t *)(p)))
#else
#define U8TO64_LE(p)                                               \
	(((uint64_t)((p)[0])) | ((uint64_t)((p)[1]) << 8) |        \
	 ((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) | \
	 ((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) | \
	 ((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56))
#endif

#define U8TO64_LE_NOCASE(p)                                                 \
	(((uint64_t)(siptlw((p)[0]))) | ((uint64_t)(siptlw((p)[1])) << 8) | \
	 ((uint64_t)(siptlw((p)[2])) << 16) |                               \
	 ((uint64_t)(siptlw((p)[3])) << 24) |                               \
	 ((uint64_t)(siptlw((p)[4])) << 32) |                               \
	 ((uint64_t)(siptlw((p)[5])) << 40) |                               \
	 ((uint64_t)(siptlw((p)[6])) << 48) |                               \
	 ((uint64_t)(siptlw((p)[7])) << 56))

#define SIPROUND                   \
	do {                       \
		v0 += v1;          \
		v1 = ROTL(v1, 13); \
		v1 ^= v0;          \
		v0 = ROTL(v0, 32); \
		v2 += v3;          \
		v3 = ROTL(v3, 16); \
		v3 ^= v2;          \
		v0 += v3;          \
		v3 = ROTL(v3, 21); \
		v3 ^= v0;          \
		v2 += v1;          \
		v1 = ROTL(v1, 17); \
		v1 ^= v2;          \
		v2 = ROTL(v2, 32); \
	} while (0)

uint64_t siphash(const uint8_t *in, const size_t inlen, const uint8_t *k)
{
#ifndef UNALIGNED_LE_CPU
	uint64_t hash;
	uint8_t *out = (uint8_t *)&hash;
#endif
	uint64_t v0 = 0x736f6d6570736575ULL;
	uint64_t v1 = 0x646f72616e646f6dULL;
	uint64_t v2 = 0x6c7967656e657261ULL;
	uint64_t v3 = 0x7465646279746573ULL;
	uint64_t k0 = U8TO64_LE(k);
	uint64_t k1 = U8TO64_LE(k + 8);
	uint64_t m;
	const uint8_t *end = in + inlen - (inlen % sizeof(uint64_t));
	const int left = inlen & 7;
	uint64_t b = ((uint64_t)inlen) << 56;
	v3 ^= k1;
	v2 ^= k0;
	v1 ^= k1;
	v0 ^= k0;

	for (; in != end; in += 8) {
		m = U8TO64_LE(in);
		v3 ^= m;

		SIPROUND;

		v0 ^= m;
	}

	switch (left) {
	case 7:
		b |= ((uint64_t)in[6]) << 48; /* fall-thru */
	case 6:
		b |= ((uint64_t)in[5]) << 40; /* fall-thru */
	case 5:
		b |= ((uint64_t)in[4]) << 32; /* fall-thru */
	case 4:
		b |= ((uint64_t)in[3]) << 24; /* fall-thru */
	case 3:
		b |= ((uint64_t)in[2]) << 16; /* fall-thru */
	case 2:
		b |= ((uint64_t)in[1]) << 8; /* fall-thru */
	case 1:
		b |= ((uint64_t)in[0]);
		break;
	case 0:
		break;
	}

	v3 ^= b;

	SIPROUND;

	v0 ^= b;
	v2 ^= 0xff;

	SIPROUND;
	SIPROUND;

	b = v0 ^ v1 ^ v2 ^ v3;
#ifndef UNALIGNED_LE_CPU
	U64TO8_LE(out, b);
	return hash;
#else
	return b;
#endif
}

uint64_t siphash_nocase(const uint8_t *in, const size_t inlen, const uint8_t *k)
{
#ifndef UNALIGNED_LE_CPU
	uint64_t hash;
	uint8_t *out = (uint8_t *)&hash;
#endif
	uint64_t v0 = 0x736f6d6570736575ULL;
	uint64_t v1 = 0x646f72616e646f6dULL;
	uint64_t v2 = 0x6c7967656e657261ULL;
	uint64_t v3 = 0x7465646279746573ULL;
	uint64_t k0 = U8TO64_LE(k);
	uint64_t k1 = U8TO64_LE(k + 8);
	uint64_t m;
	const uint8_t *end = in + inlen - (inlen % sizeof(uint64_t));
	const int left = inlen & 7;
	uint64_t b = ((uint64_t)inlen) << 56;
	v3 ^= k1;
	v2 ^= k0;
	v1 ^= k1;
	v0 ^= k0;

	for (; in != end; in += 8) {
		m = U8TO64_LE_NOCASE(in);
		v3 ^= m;

		SIPROUND;

		v0 ^= m;
	}

	switch (left) {
	case 7:
		b |= ((uint64_t)siptlw(in[6])) << 48; /* fall-thru */
	case 6:
		b |= ((uint64_t)siptlw(in[5])) << 40; /* fall-thru */
	case 5:
		b |= ((uint64_t)siptlw(in[4])) << 32; /* fall-thru */
	case 4:
		b |= ((uint64_t)siptlw(in[3])) << 24; /* fall-thru */
	case 3:
		b |= ((uint64_t)siptlw(in[2])) << 16; /* fall-thru */
	case 2:
		b |= ((uint64_t)siptlw(in[1])) << 8; /* fall-thru */
	case 1:
		b |= ((uint64_t)siptlw(in[0]));
		break;
	case 0:
		break;
	}

	v3 ^= b;

	SIPROUND;

	v0 ^= b;
	v2 ^= 0xff;

	SIPROUND;
	SIPROUND;

	b = v0 ^ v1 ^ v2 ^ v3;
#ifndef UNALIGNED_LE_CPU
	U64TO8_LE(out, b);
	return hash;
#else
	return b;
#endif
}
