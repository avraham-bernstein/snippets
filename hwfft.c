/*
FILE: hwfft.c
DESCRIP: new hamming weight crypto primitive
================================================================================
DATE: 2017-11-26T00:09:00Z
AUTHOR: Avraham DOT Bernstein AT gmail
COPYRIGHT (c) 2017 Avraham Bernstein, Jerusalem ISRAEL. All rights reserved.
LICENSE: Apache License, Version 2.0: https://opensource.org/licenses/Apache-2.0
REVISION HISTORY:
2017-11-26: 1.0.0: AB: original
================================================================================
*/

// ==== cc.h

// ==== cc-stddef.h

#ifdef __cplusplus
    #include <cstddef>
#else
    #include <stddef.h>
#endif

#ifndef __BEGIN_DECLS                                       // <stddef.h>
    #ifdef __cplusplus
        #define __BEGIN_DECLS       extern "C" {
        #define __END_DECLS         }
    #else
        #define __BEGIN_DECLS
        #define __END_DECLS
    #endif
#endif

#define CC_BEGIN_DECLS              __BEGIN_DECLS           // alias
#define CC_END_DECLS                __END_DECLS             // alias

#ifndef __CONCAT                                            // <stddef.h>
#define __CONCAT(_x,_y)             _x ## _y                // raw
#endif

#define CC__CONCAT                  __CONCAT                // alias-raw
#define CC_CONCAT(_x,_y)            CC__CONCAT(_x,_y)       // deferred
#define CC_CAT                      CC_CONCAT               // alias-deferred
#define CC_CONCAT2                  CC_CONCAT               // alias-deferred
#define CC_CAT2                     CC_CONCAT               // alias-deferred

#define CC__CONCAT3(_x,_y,_z)       _x ## _y ## _z          // raw
#define CC_CONCAT3(_x,_y,_z)        CC__CONCAT3(_x,_y,_z)   // deferred
#define CC_CAT3                     CC_CONCAT3              // alias-deferred

#define CC__CONCAT4(_w,_x,_y,_z)    _w ## _x ## _y ## _z    // raw
#define CC_CONCAT4(_w,_x,_y,_z)     CC__CONCAT4(_w,_x,_y,_z) // deferred
#define CC_CAT4                     CC_CONCAT4              // alias-deferred

#ifndef __STRING                                            // <stddef.h>
#define __STRING(_x)                # _x                    // raw
#endif

#define CC__STRING                  __STRING                // alias-raw
#define CC_STRING(_x)               CC__STRING(_x)         	// deferred
#define CC_STR                      CC_STRING           	// alias-deferred

// ==== cc-compiler.h

#define CC_VS(_major,_minor,_patch) ((_major)*10000 + (_minor)*100 + (_patch))
#define CC_VS2(_major,_minor)       CC_VS(0,_major,_minor)

#define CC_CLANG                    (defined(__clang__) ? CC_VS(__clang_major__, __clang_minor__, __clang_patchlevel__) : 0)
#define CC_GNUC                     (defined(__GNUC__) ? CC_VS(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__) : 0)
#define CC_MSC                      (defined(_MSC_VER) ? _MSC_VER : 0)
#define CC_TINYC                    (defined(__TINYC__) ? __TINYC__ : 0)

#define CC_C                        defined(__STDC_VERSION__)
#define CC_C94                      (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L)
#define CC_C99                      (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define CC_C11                      (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)

#define CC_CPP                      defined(__cplusplus)
#define CC_CPP03                    (defined(__cplusplus) && __cplusplus >= 200301L)
#define CC_CPP11                    (defined(__cplusplus) && __cplusplus >= 201103L)
#define CC_CPP14                    (defined(__cplusplus) && __cplusplus >= 201402L)

#if CC_CPP
    #define CC_CPP_MINIMUM          CC_CPP11
    #if CC_CPP < CC_CPP_MINIMUM
        #error "Requires C++ version >= C++11"
    #endif
#else // STDC
    #define CC_C_MINIMUM            CC_C99
    #define CC_C_MODERN             CC_C11
    #if CC_C < CC_C_MINIMUM
        #error "Requires C version >= C99"
    #elif CC_C < CC_C_MODERN
        #warning "Using obsolete C version < C11"
    #endif
#endif

#define CC_CLANG_ATTRIB(...)
#define CC_GCC_ATTRIB(...)
#define CC_MSC_ATTRIB(_attrib)
#define CC_TCC_ATTRIB(...)

#define CC_C_GENERIC(...)

#if CC_CPP
    #define CC_INLINE               inline
    #define CC_CPP_USE_STD          using namespace std
#else
    #define CC_INLINE               static inline
    #define CC_CPP_USE_STD
    #if CC_C11
        #undef CC_C_GENERIC
        #define CC_C_GENERIC(...)   _Generic(__VA_ARGS__)
    #endif
#endif

#if !(CC_C11 || CC_CPP11)
    // _bool_expr arg must be resolvable at compile-time
    #define static_assert(_bool_expr, _q_emsg) \
	   static const char * CC_CAT(static_assert_,__COUNTER__)[(_bool_expr) ? 1 : 0] \
	      = { (_bool_expr_) ? "" : _q_emsg }
#endif

#if CC_GNUC

// ==== cc-gnuc.h

    #define CC_GNUC_MINIMUM         CC_VS(4,6,4) // 2013: supports C99 & C++03 +  better ARM, Android, and Golang support
    #define CC_GNUC_MODERN          CC_VS(4,9,0) // 2014: supports C11 & C++11
    #define CC_CLANG_MINIMUM        CC_VS(3,4,2) // 2013: supports C99 & C++03, compatability with gcc 4.7
    #define CC_CLANG_MODERN         CC_VS(3,6,0) // 2014: supports C11 & C++11, compatability with gcc 4.9

    #if CC_CLANG
        #if CC_CLANG < CC_CLANG_MINIMUM
            #error "Requires clang >= v3.4.2"
        #elif CC_CLANG < CC_CLANG_MODERN
            #warning "Using obsolete clang < v3.6.0"
        #endif
    #elif CC_GNUC < CC_GNUC_MINIMUM
        #error "Requires gcc >= v4.6.4"
    #elif CC_GNUC < GC_GNUC_MODERN
        #warning "Using obsolete gcc < v4.9.0"
    #endif

    #undef CC_GCC_ATTRIB
    #define CC_GCC_ATTRIB(...)      __attribute__((__VA_ARGS__))

    #if CC_CLANG
        #undef CC_CLANG_ATTRIB
        #define CC_CLANG_ATTRIB     CC_GCC_ATTRIB
    #endif

    #define CC_TYPEOF               __typeof

    #if CC_GNUC >= CC_GNUC_MODERN || CC_CLANG >= CC_CLANG_MODERN
        #define CC_AUTO_TYPE        __auto_type
    #endif

    #if CC_CLANG
        #pragma clang diagnostic ignored "-Wtautological-pointer-compare"
    #endif

    #include <features.h>
    #include <sys/cdefs.h>
    #include <sys/types.h>

    #if CC_C

        #include <assert.h>
        #include <ctype.h>
        #include <errno.h>
        #include <limits.h>
        #include <stdbool.h>
        #include <stdint.h>
        #include <stdlib.h>
        #include <string.h>

        #ifndef abort
        #define abort               __builtin_abort
        #endif

        #ifndef abs
        #define abs                 __builtin_abs
        #endif

        #ifndef calloc
        #define calloc              __builtin_calloc
        #endif

        #ifndef exit
        #define exit                __builtin_exit
        #endif

        #ifndef fprintf
        #define fprintf             __builtin_fprintf
        #endif

        #ifndef fscanf
        #define fscanf              __builtin_fscanf
        #endif

        #ifndef malloc
        #define malloc              __builtin_malloc
        #endif

        #ifndef memchr
        #define memchr              __builtin_memchr
        #endif

        #ifndef memcmp
        #define memcmp              __builtin_memcmp
        #endif

        #ifndef memcpy
        #define memcpy              __builtin_memcpy
        #endif

        #ifndef memset
        #define memset              __builtin_memset
        #endif

        #ifndef printf
        #define printf              __builtin_printf
        #endif

        #ifndef scanf
        #define scanf               __builtin_scanf
        #endif

        #ifndef sprintf
        #define sprintf             __builtin_sprintf
        #endif

        #ifndef sscanf
        #define sscanf              __builtin_sscanf
        #endif

        #ifndef strcat
        #define strcat              __builtin_strcat
        #endif

        #ifndef strchr
        #define strchr              __builtin_strchr
        #endif

        #ifndef strcmp
        #define strcmp              __builtin_strcmp
        #endif

        #ifndef strcpy
        #define strcpy              __builtin_strcpy
        #endif

        #ifndef strcspn
        #define strcspn             __builtin_strcspn
        #endif

        #ifndef strlen
        #define strlen              __builtin_strlen
        #endif

        #ifndef strncat
        #define strncat             __builtin_strncat
        #endif

        #ifndef strncmp
        #define strncmp             __builtin_strncmp
        #endif

        #ifndef strncpy
        #define strncpy             __builtin_strncpy
        #endif

        #ifndef strpbrk
        #define strpbrk             __builtin_strpbrk
        #endif

        #ifndef strrchr
        #define strrchr             __builtin_strrchr
        #endif

        #ifndef strspn
        #define strspn              __builtin_strspn
        #endif

        #ifndef strstr
        #define strstr              __builtin_strstr
        #endif

    #else

        #include <cassert>
        #include <cctype>
        #include <cerrno>
        #include <climits>
        #include <cstdint>
        #include <cstdlib>
        #include <cstring>

    #endif

    #include <unistd.h>             // Posix-2 API; both IOS and Windows support a Posix layer

    #ifndef popcount
        #define popcount            __builtin_popcount
    #endif

#elif CC_MSC

// ==== cc-msc.h

    #define CC_MSC_MINIMUM          CC_VS2(19,0) // 2015: supports C99, C++11, Android/ARM
    #if CC_MSC < CC_MSC_MINIMUM
        #error "Requires msc >= v1900"
    #endif

    #undef CC_MSC_ATTRIB
    #define CC_MSC_ATTRIB(_attrib)  declspec(_attrib)

#elif CC_TINYC

// ==== cc-tinyc.h

    #define CC_TINYC_MINIMUM        CC_VS2(9,26) // 2013: supports C99, __TINYC__ version id, limited gcc attributes and extensions
    #if CC_TINYC < CC_TINYC_MINIMUM
        #error "Requires tcc >= v0.9.26"
    #endif

    #undef CC_TCC_ATTRIB
    #define CC_TCC_ATTRIB(...)      __attribute__((__VA_ARGS__))

    #define CC_TYPEOF               typeof

    #include <tcclib.h>

#else
    #warning "Unrecognized compiler"
#endif

// ==== cc-auto.h

#if CC_C
    #if CC_C11 && ( CC_CLANG >= CC_CLANG_MODERN || CC_GNUC >= CC_GNUC_MODERN )
        #define CC_AUTO(_varname,_val)          CC_AUTO_TYPE _varname = _val
    #else
        // potential danger of side effects because "_val" is used twice
        #define CC_AUTO(_varname,_val)          CC_TYPEOF(_val) _varname = _val
    #endif
#elif CC_CPP
    #if CC_CPP14
        #define CC_AUTO(_varname,_val)          decltype(auto) _varname = _val
    #elif CC_CPP11
        #define CC_AUTO(_varname,_val)          auto _varname = _val
    #endif
#endif

// ==== APP START ====

#if CC_C
    // Note MSC does *not* support C11
    #if !(CC_GNUC && CC_C11)
        #error "C version of app requires GNUC & C11, because using generics, auto_type, and statement expressions."
    #endif
#else // c++
    // But MSC does support C++11
    #if !CC_CPP11
        #error "C++ version of app requires C++11, because using lambdas."
    #endif
#endif

#include <stdio.h>

CC_CPP_USE_STD;

CC_GCC_ATTRIB(nonnull,nothrow,unused)
static void perr5(const char * emsg, const char * fname, int lineno, const char * func, int abort_flag)
{
    static const char * app = "MY_APP";

    fflush(stdout);
    fprintf(stderr,"\n%c: %s: %s(%d,%s): %s.\n%s", abort_flag ? 'F' : 'E', app, fname, lineno, func, emsg, abort_flag ? "Abort!\n" : "");
    fflush(stderr);

    if (abort_flag) {
        abort();
    }
}

#define perr(_q_emsg) perr5(_q_emsg,__FILE__,__LINE__,__PRETTY_FUNCTION__,0)
#define panic(_q_emsg) perr5(_q_emsg,__FILE__,__LINE__,__PRETTY_FUNCTION__,1)

#ifdef CC_GNUC
    #define popcount_32 popcount
    #define popcount_64 popcount
#else
    #define popcount_32 bswar_32
    #define popcount_64 bswar_64
    // ref: https://www.playingwithpointers.com/swar.html
    CC_INLINE unsigned bswar_32(uint32_t x)
    {
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        return (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
    }
    CC_INLINE unsigned bswar_64(uint64_t x) {
        x = x - ((x >> 1) & 0x5555555555555555);
        x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
        return (((x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101) >> 56;
    }
#endif

typedef struct { uint64_t x, w, S; } rnd32_t;

CC_GCC_ATTRIB(nonnull,nothrow)
CC_INLINE uint32_t rnd32(rnd32_t * ctx)
{
    // ref: https://en.wikipedia.org/wiki/Middle-square_method
    // Middle Square Weyl Sequence PRNG: Non-linear ! Period: 2^127 bits
    // As of 2017-10-25: fastest & smallest PRNG to pass BigCrush TestU01

    // assert(ctx != 0 && (ctx->S & 1) == 1);

    CC_AUTO(x,ctx->x);
    x *= x; x += (ctx->w += ctx->S);
    x = (x >> 32) | (x << 32); // swap hi-lo
    ctx->x = x;

    return x;
}

// lambda version must prefix last statement with 'return'
// captures (x,shift)
#define _brotl_body(...)                        \
    CC_AUTO(_x,x);                              \
    unsigned _shift = shift;                    \
    const unsigned _bsize = sizeof(_x) << 3;    \
    _shift &= _bsize - 1;                       \
    __VA_ARGS__ (_x << _shift) | (_x >> (_bsize - _shift))

#if CC_C
    #define brotl_g(x,shift) CC_C_GENERIC((x),  \
            uint8_t:brotl_8,                    \
            uint16_t:brotl_16,                  \
            uint32_t:brotl_32,                  \
            uint64_t:brotl_64                   \
        )(x,shift)

    // gcc statement expr, captures (x,shift)
    #define _brotl() ({ _brotl_body(); })
#else // c++
    #define brotl_8                 brotl_g
    #define brotl_16                brotl_g
    #define brotl_32                brotl_g
    #define brotl_64                brotl_g

    // c++ lambda, captures (x,shift)
    // must use capture because we do not know the type of 'x'
    #define _brotl_lambda_def() auto _brotl = [x,shift] () { _brotl_body(return); }
#endif

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint8_t brotl_8(uint8_t x, unsigned shift)
{
    #if CC_CPP
        _brotl_lambda_def();
    #endif
    return _brotl();
}

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint16_t brotl_16(uint16_t x, unsigned shift)
{
    #if CC_CPP
        _brotl_lambda_def();
    #endif
    return _brotl();
}

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint32_t brotl_32(uint32_t x, unsigned shift)
{
    #if CC_CPP
        _brotl_lambda_def();
    #endif
    return _brotl();
}

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint64_t brotl_64(uint64_t x, unsigned shift)
{
    #if CC_CPP
        _brotl_lambda_def();
    #endif
    return _brotl();
}

#if CC_CPP
    #define brotr_8                 brotr_g
    #define brotr_16                brotr_g
    #define brotr_32                brotr_g
    #define brotr_64                brotr_g
#else
    #define brotr_g(x,shift) CC_C_GENERIC((x),  \
            uint8_t:brotr_8,                    \
            uint16_t:brotr_16,                  \
            uint32_t:brotr_32,                  \
            uint64_t:brotr_64                   \
        )(x,shift)
#endif

#define _brotr(x,shift) ({                      \
    CC_AUTO(_x,x);                              \
    unsigned _shift = shift;                    \
    const unsigned _bsize = sizeof(_x) << 3;    \
    _shift &= _bsize - 1;                       \
    (_x >> _shift) | (_x << (_bsize - _shift)); \
})

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint8_t brotr_8(uint8_t x, unsigned shift)
{
    return _brotr(x,shift);
}

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint16_t brotr_16(uint16_t x, unsigned shift)
{
    return _brotr(x,shift);
}

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint32_t brotr_32(uint32_t x, unsigned shift)
{
    return _brotr(x,shift);
}

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE uint64_t brotr_64(uint64_t x, unsigned shift)
{
    return _brotr(x,shift);
}

#if CC_CPP
    #define bshuffle_8              bshuffle_g
    #define bshuffle_16             bshuffle_g
    #define bshuffle_32             bshuffle_g
    #define bshuffle_64             bshuffle_g
#else
    #define bshuffle_g(x,p) CC_C_GENERIC((x),   \
            uint8_t:bshuffle_8,                 \
            uint16_t:bshuffle_16,               \
            uint32_t:bshuffle_32,               \
            uint64_t:bshuffle_64                \
        )(x,p)
#endif

#define _bshuffle(x,p) ({                       \
    CC_AUTO(_x,x);                              \
    CC_AUTO(_p,p);                              \
    CC_AUTO(_y,_x); _y = 0;                     \
    unsigned _i = 0;                            \
    for (; _x; _x >>=1, ++_i) {                 \
        if (_x & 1) _y |= 1 << _p[_i];          \
    }                                           \
    _y;                                         \
})

CC_GCC_ATTRIB(pure,nonnull,nothrow)
CC_INLINE uint8_t bshuffle_8(uint8_t x, const uint8_t p[8])
{
    return _bshuffle(x,p);
}

CC_GCC_ATTRIB(pure,nonnull,nothrow)
CC_INLINE uint16_t bshuffle_16(uint16_t x, const uint8_t p[16])
{
    return _bshuffle(x,p);
}

CC_GCC_ATTRIB(pure,nonnull,nothrow)
CC_INLINE uint32_t bshuffle_32(uint32_t x, const uint8_t p[32])
{
    return _bshuffle(x,p);
}

CC_GCC_ATTRIB(pure,nonnull,nothrow)
CC_INLINE uint64_t bshuffle_64(uint64_t x, const uint8_t p[64])
{
    return _bshuffle(x,p);
}

#if CC_CPP
    #define hwdft_8                 hwdft_g
    #define hwdft_16                hwdft_g
    #define hwdft_32                hwdft_g
    #define hwdft_64                hwdft_g
#else
    #define hwdft_g(x) CC_C_GENERIC((x),        \
            uint8_t:hwdft_8,                    \
            uint16_t:hwdft_16,                  \
            uint32_t:hwdft_32,                  \
            uint64_t:hwdft_64                   \
        )(x)
#endif

#define _hwdft(x,width) ({                      \
    CC_AUTO(_x,x);                              \
    unsigned _width = width;                    \
    unsigned _mask = (1 << _width) - 1;         \
    unsigned _threshhold = _mask >> 1;          \
    unsigned _sum = 0;                          \
    for (; _x; _x >>= _width) {                 \
        if ((_x & _mask) > _threshhold) ++_sum; \
    }                                           \
    _sum;                                       \
})

CC_GCC_ATTRIB(const,nothrow)
CC_INLINE unsigned hwdft_8(uint8_t x)
{
    unsigned sum = _hwdft(x,1);
    sum += _hwdft(x,2) << 1;
    return sum;
}

CC_GCC_ATTRIB(const,nothrow,unused)
static unsigned hwdft_16(uint16_t x)
{
    unsigned sum = _hwdft(x,1);
    sum += _hwdft(x,2) << 1;
    sum += _hwdft(x,4) << 2;
    return sum;
}

CC_GCC_ATTRIB(const,nothrow,unused)
static unsigned hwdft_32(uint32_t x)
{
    unsigned sum = _hwdft(x,1);
    sum += _hwdft(x,2) << 1;
    sum += _hwdft(x,4) << 2;
    sum += _hwdft(x,8) << 3;
    return sum;
}

CC_GCC_ATTRIB(const,nothrow,unused)
static unsigned hwdft_64(uint64_t x)
{
    unsigned sum = _hwdft(x,1);
    sum += _hwdft(x,2) << 1;
    sum += _hwdft(x,4) << 2;
    sum += _hwdft(x,8) << 3;
    sum += _hwdft(x,16) << 4;
    return sum;
}

#if 1

#define BWIDTH 16
#define N (1 << BWIDTH)
uint8_t arr[N];
uint8_t perm[BWIDTH][BWIDTH];

CC_GCC_ATTRIB(nothrow)
static void init()
{
    // init global permutation vectors

    static const uint8_t iota[BWIDTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint8_t p[BWIDTH],swap;
    unsigned i,j,k,r;

    rnd32_t r_ctx;
    r_ctx.S = 0x12345678fUL; // must be odd; 1 param: effectively 63-bit
    r_ctx.x = 0;
    r_ctx.w = 0;

    for (i = 0; i < BWIDTH; ++i) {
        memcpy(p,iota,BWIDTH);

        // Fisher-Yates permutation algorithm tweaked to disallow fixed points
        for (j = 0; j < (BWIDTH-2); ++j) {
            r = rnd32(&r_ctx) % (BWIDTH-1-j);
            swap = p[j];
            p[j] = p[k = 1 + j + r];
            p[k] = swap;
        }

        // last round: j = BWIDTH-1 and r = 0
        swap = p[j];
        p[j] = p[1+j];
        p[j+1] = swap;

        for (j = 0; j < BWIDTH; ++j) {
            assert(p[j] != j);
        }

        memcpy(&perm[i][0],p,BWIDTH);
    }
}

int main()
{
    uint8_t hi_8, lo_8;
    uint16_t hi, lo, x;
    unsigned hi_shift, lo_shift, x_shift, hw;
    uint32_t i;

    init(); // init global permutation vectors

    for (i = 0; i < N; ++i) {
        lo_8 = i;
        hi_8 = i >> 8;

        // cross 8x8 HWDFT rotation + conditional complement: "extremely" bent

        lo_shift = hwdft_g(hi_8); // hwdft_g has a uniform distribution
        hi_shift = hwdft_g(lo_8); // hwdft_g has a uniform distribution

        lo = (lo_shift & 1) ? brotr_g((uint8_t)~lo_8, lo_shift + 1) : brotl_g(lo_8, lo_shift + 3); // 2 params: 3-bit
        hi = (hi_shift & 1) ? brotl_g((uint8_t)~hi_8, hi_shift + 5) : brotr_g(hi_8, hi_shift + 7); // 2 params: 3-bit
        x = hi | (lo << 8); // swap hi-lo

        // self 16 HWDFT rotation + conditional complement: bent

        x ^= x >> 1; // linear grey xform changes HW
        x_shift = hwdft_g(x);
        x = (x_shift & 1) ? brotr_g((uint16_t)~x, x_shift + 11) : brotl_g(x, x_shift + 13); // 2 params: 4-bit

        // non-linear bit shuffle

        hw = popcount_32(x) & (BWIDTH-1); // recall that the HW has a binomial distribution
        x = bshuffle_g(x,&perm[hw][0]); // every HW has its own different permutation vector

        if (++arr[x] != 1) {
            printf("failure:16 = 0x%04x\n",(uint32_t)x);
            break;
        }
    }

    return 0;
}

/*

LO_PRIME and HI_PRIME are known, and are non-zero and non-0xff
GOAL is to determine original lo and hi

hi_hw0 = hwdft_8(LO_PRIME) & 7
hi_hw1 = hwdft_8(rotr(LO_PRIME,1)) & 7
hi_hw0 != hi_hw1

lo_hw0 = hwdft_8(HI_PRIME) & 7
lo_hw1 = hwdft_8(rotr(HI_PRIME,1)) & 7
lo_hw0 != lo_hw1

lo_0 = rotl(LO_PRIME,hi_hw0)
lo_1 = rotl(LO_PRIME,hi_hw1)
lo_0 != lo_1

hi_0 = rotl(HI_PRIME,lo_hw0)
hi_1 = rotl(HI_PRIME,lo_hw1)
hi_0 != hi_1

TEST 4 possibilities ?

1. LO_PRIME = rotr(lo_0,hi_hw0) = rotr(rotl(LO_PRIME,hi_hw0),hi_hw0) ==> LO_PRIME
2. LO_PRIME = rotr(lo_0,hi_hw1) = rotr(rotl(LO_PRIME,hi_hw0),hi_hw1) = rotr(LO_PRIME,hi_hw1 - hi_hw0) != LO_PRIME since hi_hw0 != hi_hw1

3. LO_PRIME = rotr(lo_1,hi_hw0) = rotr(rotl(LO_PRIME,hi_hw1),hi_hw0) = rotr(LO_PRIME,hi_hw0 - hi_hw1) != LO_PRIME since hi_hw0 != hi_hw1
4. LO_PRIME = rotr(lo_1,hi_hw1) = rotr(rotl(LO_PRIME,hi_hw1),hi_hw1) ==> LO_PRIME

Obviously 1&2 are mutually exclusive, as are 3&4
Prove 1: 1 != 3 and 1 != 4
Prove 2: 2 != 3 and 2 != 4

*/

#else

inline static uint16_t brotr16(uint16_t x, unsigned shift)
{
    shift &= 15;
    return (x >> shift) | (x << (16-shift));
}

static unsigned hwdft_16(uint16_t x)
{
    uint16_t y;
    unsigned sum[3] = {0,0,0};

    y = x;
    while (y) {
        if (y & 1) ++sum[0];
        y >>= 1;
    }

    y = x;
    while (y) {
        if ((y & 3) > 1) ++sum[1];
        y >>= 2;
    }

    y = x;
    while (y) {
        if ((y & 15) > 7) ++sum[2];
        y >>= 4;
    }

    return sum[0] + 2*sum[1] + 4*sum[2];
}

#define N 0x100000000UL
uint8_t arr[N];

int main()
{
    uint16_t hi0, lo0;
    uint32_t hi1, lo1, x;
    unsigned hi_shift, lo_shift;
    uint64_t x;
    uint32_t j;

    for (x = 0; x < N; ++x) {
        if ((x & 0xffffff) == 0) {
            // progress counter every 16M
            j = x >> 24;
            printf("%02x\n",j);
            fflush(stdout);
        }

        lo0 = x;
        hi0 = x >> 16;

        lo_shift = hwdft_16(hi0);
        hi_shift = hwdft_16(lo0);

        lo1 = brotr16(lo0,lo_shift);
        hi1 = brotr16(hi0,hi_shift);

        x = hi1 | (lo1 << 16); // swap hi-lo

        // non-linear bit avalanche
        x ^= x >> 1; // grey xform (linear + invertible)
        if ((lo_shift + hi_shift) & 1) x = ~x; // complement (non-linear + invertible)

        if (++arr[x] != 1) {
            j = x;
            printf("failure:32 = 0x%08x\n",j);
            break;
        }
    }

    return 0;
}

#endif
