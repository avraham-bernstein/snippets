/*
FILE: weyl-prng.c
DESCRIP: experiments with weyl PRNG algo
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
#include <limits.h>
#include <stdio.h>
#include <string.h>

const int N = 0x7fffffff;

const uint16_t A = 9, C = 7;
inline static uint16_t lcg(uint16_t x) { return A * x + C; }

inline static uint16_t rotr16 (uint16_t value, uint16_t count) {
    const uint16_t mask = (CHAR_BIT*sizeof(value)-1);
    count &= mask;
    return (value>>count) | (value<<( (-count) & mask ));
}

static const uint16_t S1 = 0xabc1; // S is odd
static const uint16_t S2 = 0xff7;

inline static uint8_t msws1a(uint16_t shift) {
    static uint16_t x = 0, w = 0;

    x *= x; x += (w += S1);
    x = rotr16(x,shift); // rotate 4-15

    // x ^= x >> 1;
    // x = lcg(x);
    // if (__builtin_popcount(x) & 1) x = ~x;

    return x;
}

inline static uint8_t msws1b(uint16_t shift) {
    static uint16_t x = 0, w = 0;

    x *= x; x += (w += S1);
    x = rotr16(x,shift); // rotate 4-15
    if (__builtin_popcount(x) & 1) x = ~x;

    return x;
}

inline static uint8_t msws2a(uint16_t shift) {
    static uint16_t x = 0, w = 0;

    x *= x; x += (w += S2);
    x = rotr16(x,shift); // rotate 4-15

    // x ^= x >> 1;
    // x = lcg(x);
    // if (__builtin_popcount(x) & 1) x = ~x;

    return x;
}

inline static uint8_t msws2b(uint16_t shift) {
    static uint16_t x = 0, w = 0;

    x *= x; x += (w += S2);
    x = rotr16(x,shift); // rotate 4-15
    if (__builtin_popcount(x) & 1) x = ~x;

    return x;
}

int main()
{
    int freq_array[256];
    int min, max;
    const uint16_t SHIFT = 5;

    memset(freq_array,0,sizeof(freq_array));
    min=N, max = 0;

    for (int i = 0; i < N; ++i) {
        if ((i & 0xffffff) == 0) { printf("."); fflush(stdout); }
        uint8_t y = msws1a(SHIFT);
        ++freq_array[y];
    }

    printf("\n**** 1a: SHIFT=%2d, S=0x%04x\n",(int)SHIFT, (int)S1);

    for (int i = 0; i <= 255; ++i) {
        int f = freq_array[i];
        if (f < min) min = f;
        if (f > max) max = f;
        printf("%3d, %9d\n",i,f);
    }

    printf("min = %9d, max = %9d\n", min, max);

    memset(freq_array,0,sizeof(freq_array));
    min=N, max = 0;

    for (int i = 0; i < N; ++i) {
        if ((i & 0xffffff) == 0) { printf("."); fflush(stdout); }
        uint8_t y = msws1b(SHIFT);
        ++freq_array[y];
    }

    printf("\n**** 1b: SHIFT=%2d, S=0x%04x\n",(int)SHIFT, (int)S1);

    for (int i = 0; i <= 255; ++i) {
        int f = freq_array[i];
        if (f < min) min = f;
        if (f > max) max = f;
        printf("%3d, %9d\n",i,f);
    }

    printf("min = %9d, max = %9d\n", min, max);

    memset(freq_array,0,sizeof(freq_array));
    min=N, max = 0;

    for (int i = 0; i < N; ++i) {
        if ((i & 0xffffff) == 0) { printf("."); fflush(stdout); }
        uint8_t y = msws2a(SHIFT);
        ++freq_array[y];
    }

    printf("\n**** 2a: SHIFT=%2d, S=0x%04x\n",(int)SHIFT,(int)S2);

    for (int i = 0; i <= 255; ++i) {
        int f = freq_array[i];
        if (f < min) min = f;
        if (f > max) max = f;
        printf("%3d, %9d\n",i,f);
    }

    printf("min = %9d, max = %9d\n", min, max);

    memset(freq_array,0,sizeof(freq_array));
    min=N, max = 0;

    for (int i = 0; i < N; ++i) {
        if ((i & 0xffffff) == 0) { printf("."); fflush(stdout); }
        uint8_t y = msws2b(SHIFT);
        ++freq_array[y];
    }

    printf("\n**** 2b: SHIFT=%2d, S=0x%04x\n",(int)SHIFT, (int)S2);

    for (int i = 0; i <= 255; ++i) {
        int f = freq_array[i];
        if (f < min) min = f;
        if (f > max) max = f;
        printf("%3d, %9d\n",i,f);
    }

    printf("min = %9d, max = %9d\n", min, max);

    return 0;
}
