/*
FILE: xxtea.c
DESCRIP: experiments with xxtea.c crypto algo
================================================================================
DATE: 2017-11-26T00:09:00Z
AUTHOR: Avraham DOT Bernstein AT gmail
COPYRIGHT (c) 2017 Avraham Bernstein, Jerusalem ISRAEL. All rights reserved.
LICENSE: Apache License, Version 2.0: https://opensource.org/licenses/Apache-2.0
REVISION HISTORY:
2017-11-26: 1.0.0: AB: original
================================================================================
*/
#include <stdint.h>
#include <stdlib.h> // for abort()
#include <assert.h>
#include <stdio.h>

#ifndef __cplusplus
#define INLINE static inline
#include <malloc.h>
#else
#define INLINE inline
#endif

#ifdef __GNUC__
#define GCC_ATTRIB(...) __attribute__((__VA_ARGS__))
#define memcpy __builtin_memcpy
#else
#define GCC_ATTRIB(...)
#include <string.h> // for memcpy()
#endif

GCC_ATTRIB(nonnull,nothrow)
static void perr(const char * emsg, const char * fname, int lineno, const char * func, int abort_flag)
{
    fflush(stdout);
    fprintf(stderr,"\nE: %s(%d,%s): %s.\n%s", fname, lineno, func, emsg, abort_flag ? "Abort!\n" : "");
    fflush(stderr);

    if (abort_flag) {
        abort();
    }
}

#define PERR(emsg) perr(emsg,__FILE__,__LINE__,__PRETTY_FUNCTION__,0)
#define PANIC(emsg) perr(emsg,__FILE__,__LINE__,__PRETTY_FUNCTION__,1)

typedef struct { uint64_t x, w, S; } rnd32_t;

GCC_ATTRIB(nonnull,nothrow)
INLINE uint32_t rnd32(rnd32_t * ctx)
{
    // ref: https://en.wikipedia.org/wiki/Middle-square_method
    // Middle Square Weyl Sequence PRNG: Non-linear ! Period: 2^127 bits
    // As of 2017-10-25: fastest & smallest PRNG to pass BigCrush TestU01

    assert(ctx != 0 && (ctx->S & 1) == 1);

    uint64_t x = ctx->x;
    x *= x; x += (ctx->w += ctx->S);
    x = (x >> 32) | (x << 32); // swap hi-lo
    ctx->x = x;

    return x;
}

#define DELTA   (uint32_t)0x9e3779b9
#define MX      (uint32_t)( ((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)) )

GCC_ATTRIB(nonnull,nothrow)
static void xxtea(uint32_t * v, int n, const uint32_t key[4]) {
    // ref: https://en.wikipedia.org/wiki/XXTEA
    // clarified version: btea()

    uint32_t y, z, sum;
    unsigned p, rounds, e;

    if (n > 1) { // encrypt
        rounds = 6 + 52/n;
        sum = 0;
        z = v[n-1];
        do {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p=0; p<n-1; p++) {
                y = v[p+1];
                z = v[p] += MX;
            }
            y = v[0];
            z = v[n-1] += MX;
        } while (--rounds);
    } else if (n < -1) { // decrypt
        n = -n;
        rounds = 6 + 52/n;
        sum = rounds*DELTA;
        y = v[0];
        do {
            e = (sum >> 2) & 3;
            for (p=n-1; p>0; p--) {
                z = v[p-1];
                y = v[p] -= MX;
            }
            z = v[n-1];
            y = v[0] -= MX;
            sum -= DELTA;
        } while (--rounds);
    }
}

GCC_ATTRIB(nonnull,nothrow)
static void ayb_xxtea(uint32_t * v, int n, const uint32_t key[4]) {

// AYB: BEGIN

    enum { OP_ENCRYPT, OP_DECRYPT, OP_INVALID };
    int op;

    if (n > 1) {
        op = OP_ENCRYPT;
    } else if (n < -1) {
        op = OP_DECRYPT;
    } else {
        op = OP_INVALID;
    }

    assert( DELTA != 0 && v != 0 && key != 0 && op != OP_INVALID );
    assert( ((key[0] != 0) + (key[1] != 0) + (key[2] != 0) + (key[3] != 0)) >= 2 ); // at least 2 keys != 0

    if (n < 0) { n = -n; }

// AYB: END

    // unsigned rounds = 6 + 52/n; // original
    // AYB: double the number of rounds to protect against the yaarkov differential attack
    //  https://eprint.iacr.org/2010/254
    unsigned rounds = 12 + 128/n; // AYB

    uint32_t y, z, sum;
    unsigned p, e;

// AYB: BEGIN

    // initialize PRNG

    rnd32_t r_ctx;
    r_ctx.x = DELTA * UINT32_C(613); // where 613 is prime, and DELTA is uint32_t
    r_ctx.w = 0;
    r_ctx.S = ((uint64_t)(key[0]^key[1])) | ((uint64_t)(key[2]^key[3]) << 32);
    if (r_ctx.S == 0) {
        r_ctx.S = ((uint64_t)(key[0]^key[2])) | ((uint64_t)(key[1]^key[3]) << 32);
        if (r_ctx.S == 0) {
            r_ctx.S = DELTA;
        }
    }
    if ((r_ctx.S & 1) == 0) {
        ++r_ctx.S;
    }

// AYB: END

    if (op == OP_ENCRYPT) {

        sum = 0;
        z = v[n-1];
        do {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p=0; p<n-1; p++) {
                y = v[p+1];
                z = v[p] += rnd32(&r_ctx) ^ MX; // AYB
            }
            y = v[0];
            z = v[n-1] += rnd32(&r_ctx) ^ MX; // AYB
        } while (--rounds);

    } else if (op == OP_DECRYPT) {

// AYB: BEGIN

        // allocate and initialize array of random numbers because we will be
        // using them in reverse order

        const int r_num_ops = n*rounds;
        int r_i = r_num_ops - 1;

        #pragma pack(push)
        #pragma pack(4)

        struct Uint32 { uint32_t x; };

        #ifndef __cplusplus
            struct Uint32 * r_arr = malloc(sizeof(struct Uint32) * r_num_ops);
        #else
            Uint32 * r_arr = new Uint32[r_num_ops];
        #endif

        #pragma pack(pop)

        if (r_arr == 0) {
            PANIC("out of memory");
        }

        // !!! load the random numbers in reverse order !!!
        for (; r_i >= 0; --r_i) r_arr[r_i].x = rnd32(&r_ctx);

// AYB: END

        sum = rounds*DELTA;
        y = v[0];
        do {
            e = (sum >> 2) & 3;
            for (p=n-1; p>0; p--) {
                z = v[p-1];
                y = v[p] -= r_arr[++r_i].x ^ MX; // AYB
            }
            z = v[n-1];
            y = v[0] -= r_arr[++r_i].x ^ MX; // AYB
            sum -= DELTA;
        } while (--rounds);

// AYB: BEGIN
        #ifndef __cplusplus
            free(r_arr);
        #else
            delete[] r_arr;
        #endif
// AYB: END
    }
}

int main(int argc, const char * argv[])
{
    const uint32_t key[4] = { 0xaabbccdd, 0x1eeff001, 0x22334455, 0x96677889 };

    #define N8 32
    #define N32 (N8/4)
    const unsigned char plaintext[N8] = {
        'a','b','c','d','e','f','g','h',
        'i','j','k','l','m','n','o','p',
        'q','r','s','t','u','v','w','x',
        'y','z','0','1','2','3','4',0
    };

    unsigned char buf[N8];
    int i;

    assert(N8 >= 32 && N8%4 == 0);

    printf("xxtea-original:\n");
    memcpy(buf,plaintext,N8);
    xxtea((uint32_t *)buf,N32,key);
    xxtea((uint32_t *)buf,-N32,key);
    for (i = 0; i < N8; ++i) {
        printf("%2d: %c\n",i,buf[i]);
    }
    printf("\n");

    printf("xxtea-ayb:\n");
    memcpy(buf,plaintext,N8);
    ayb_xxtea((uint32_t *)buf,N32,key);
    ayb_xxtea((uint32_t *)buf,-N32,key);
    for (i = 0; i < N8; ++i) {
        printf("%2d: %c\n",i,buf[i]);
    }
    printf("\n");

    return 0;
}
