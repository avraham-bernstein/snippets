#pragma once
#define CC_BOOST_H_ CC_VS(1,0,0)
#if 0 // begin:comment (must have balanced quotes and braces!)
================================================================================
FILE: cc-boost.h
DESCRIP: This module defines a *MINIMAL* set of Boost-like macros that rely upon
    cpp only (i.e. the C preprocessor), but that do not rely upon any template
    features from C++, because this module is designed for a *heterogeneous*
    C/C++ project, whereas Boost, http://www.boost.org/ , is designed for a
    *homogeneous* C++ project.
    Note that cpp is an extremely complex, brittle, and inefficient tool for
    implementing complex macro preprocessing. If we require complex preprocessing
    then IMHO by far the best tool to use is Jinja2, http://jinja.pocoo.org/ .
    See how Google uses Jinja2 in their Chromium project,
    https://www.chromium.org/developers/jinja .
TESTED ON COMPILERS: gcc/g++ v5.4 & clang/clang++ v3.8
================================================================================
DATE: 2017-11-26T00:09:00Z
AUTHOR: Avraham DOT Bernstein AT gmail
COPYRIGHT (c) 2017 Avraham Bernstein, Jerusalem ISRAEL. All rights reserved.
LICENSE: Apache License, Version 2.0: https://opensource.org/licenses/Apache-2.0
REVISION HISTORY:
2017-11-26: 1.0.0: AB: original
================================================================================
#endif // end:comment

#include "cc.h"
#ifndef CC_H_
    #error "Missing inclusion of \"cc.h\"."
#endif

#define CCB_MAX_LIS_SIZE            64

#define CCB_NIL(...)
#define CCB_IDENT(...)              __VA_ARGS__

// usage: CC_EVAL(n)(...)
#define CCB_EVAL(n)                 CC_CAT2(CCB_EVAL_,n) // n=[0-8]
#define CCB_EVAL_0                  CCB_NIL
#define CCB_EVAL_1(...)             __VA_ARGS__
#define CCB_EVAL_2(...)             CCB_EVAL_1(__VA_ARGS__)
#define CCB_EVAL_3(...)             CCB_EVAL_2(__VA_ARGS__)
#define CCB_EVAL_4(...)             CCB_EVAL_3(__VA_ARGS__)
#define CCB_EVAL_5(...)             CCB_EVAL_4(__VA_ARGS__)
#define CCB_EVAL_6(...)             CCB_EVAL_5(__VA_ARGS__)
#define CCB_EVAL_7(...)             CCB_EVAL_6(__VA_ARGS__)
#define CCB_EVAL_8(...)             CCB_EVAL_7(__VA_ARGS__)

#define CCB_CALL(f,...)             f(__VA_ARGS__)

// usage: CC_DEFER(f)(...)
#define CCB_DEFER(f)                f CCB_NIL()

// usage: CCB_IF(b)(true_expr,false_expr)
#define CCB_IF(b)                   CC_CAT2(CCB_IF_,b) // b=[0,1]
#define CCB_IF__EVAL(x)             x
#define CCB_IF_0(true_expr,false_expr) CCB_IF__EVAL(false_expr)
#define CCB_IF_1(true_expr,false_expr) CCB_IF__EVAL(true_expr)

#define CCB_IOTA0(size)             CC_CAT2(CCB_IOTA0_,size) // size=[0-64]
#define CCB_IOTA0_0                 CCB_NIL
#define CCB_IOTA0_1                 0
#define CCB_IOTA0_2                 0,1
#define CCB_IOTA0_3                 0,1,2
#define CCB_IOTA0_4                 0,1,2,3

#define CCB_IOTA1(size)             CC_CAT2(CCB_IOTA1_,size) // size=[0-64]
#define CCB_IOTA1_0                 CCB_NIL
#define CCB_IOTA1_1                 1
#define CCB_IOTA1_2                 1,2
#define CCB_IOTA1_3                 1,2,3
#define CCB_IOTA1_4                 1,2,3,4

// #include "cc-boost-iota.h" // 5-64: iota0, iota1

#define CCB_RIOTA0(size)            CC_CAT2(CCB_RIOTA0_,size) // size=[0-64]
#define CCB_RIOTA0_0                CCB_NIL
#define CCB_RIOTA0_1                0
#define CCB_RIOTA0_2                1,0
#define CCB_RIOTA0_3                2,1,0
#define CCB_RIOTA0_4                3,2,1,0

#define CCB_RIOTA1(size)            CC_CAT2(CCB_RIOTA1_,size) // size=[0-64]
#define CCB_RIOTA1_0                CCB_NIL
#define CCB_RIOTA1_1                1
#define CCB_RIOTA1_2                2,1
#define CCB_RIOTA1_3                3,2,1
#define CCB_RIOTA1_4                4,3,2,1

// #include "cc-boost-riota.h" // 5-64: riota0, riota1

// usage: CCB_INDEX(i)(...)
#define CCB_INDEX(i)                CC_CAT2(CCB_INDEX_,i) // i=[0,64] + 67 for CCB_SIZE
#define CCB_INDEX_0                 CCB_NIL
#define CCB_INDEX_1(_1,...)         _1
#define CCB_INDEX_2(_1,_2,...)      _2
#define CCB_INDEX_3(_1,_2,_3,...)   _3
#define CCB_INDEX_4(_1,_2,_3,_4,...) _4
// #include "cc-boost-index.h" // 5-64

#define CCB_INDEXF(i,f,...)         f(CCB_INDEX(i)(__VA_ARGS__)) // i=[0,64]
#define CCB_INDEXF2(i,f2,...)       f2(i,CCB_INDEX(i)(__VA_ARGS__)) // i=[0,64]

// The CCB_SIZE function below will return the size of its list argument from a
// size of 0 to 64. It will return an error for a list of size 65. It will
// silently fail for larger lists, i.e. it will return the 66th element in the
// list.
// Note about the algorithm: The algorithm is non-recursive. It was originally
// discovered by Laurent Deniau, but his algorithm could not handle an empty
// list. My (AB) solution corrects this flaw, and is in fact able to handle an
// empty list because it relies upon the special behavior of the paste operator
// '0,##__VA_ARGS__', namely that it does not append the ',' when __VA_ARGS__ is
// empty. The reason for using CCB_INDEX_66 (which 3 more than would be dictated
// by CCB_MAX_LIS_SIZE-1) is because internally the implementation uses 3
// additional args, i.e. 2 as an inherenet part of the algorithm, and a 3rd for
// error handling.

#define CCB_SIZE(...)               CCB_INDEX_67(-1,##__VA_ARGS__,CCB_E_SIZE,\
                                    64,63,62,61,60,59,58,57,\
                                    56,55,54,53,52,51,50,49,\
                                    48,47,46,45,44,43,42,41,\
                                    40,39,38,37,36,35,34,33,\
                                    32,31,30,29,28,27,26,25,\
                                    24,23,22,21,20,19,18,17,\
                                    16,15,14,13,12,11,10, 9,\
                                     8, 7, 6, 5, 4, 3, 2, 1, 0)

#define CCB_SELECT(size,indices,...) CC_CAT2(CCB_SELECT_,size)(indices,__VA_ARGS__) // size=[0,64]
#define CCB_SELECT_0                CCB_NIL
#define CCB_SELECT_1(indices,...)   CC_CAT2(CCB_INDEX_,CCB_INDEX_1 indices)(__VA_ARGS__)
#define CCB_SELECT_2(indices,...)   CC_CAT2(CCB_INDEX_,CCB_INDEX_1 indices)(__VA_ARGS__),CC_CAT2(CCB_INDEX_,CCB_INDEX_2 indices)(__VA_ARGS__)
#define CCB_SELECT_3(indices,...)   CC_CAT2(CCB_INDEX_,CCB_INDEX_1 indices)(__VA_ARGS__),CC_CAT2(CCB_INDEX_,CCB_INDEX_2 indices)(__VA_ARGS__),\
CC_CAT2(CCB_INDEX_,CCB_INDEX_3 indices)(__VA_ARGS__)
#define CCB_SELECT_4(indices,...)   CC_CAT2(CCB_INDEX_,CCB_INDEX_1 indices)(__VA_ARGS__),CC_CAT2(CCB_INDEX_,CCB_INDEX_2 indices)(__VA_ARGS__),\
CC_CAT2(CCB_INDEX_,CCB_INDEX_3 indices)(__VA_ARGS__),CC_CAT2(CCB_INDEX_,CCB_INDEX_4 indices)(__VA_ARGS__)
// #include "cc-boost-select.h" // 5-64

// usage: CCB_HEAD(size)(...)
#define CCB_HEAD(size)              CC_CAT2(CCB_HEAD_,size) // size=[0-64]
#define CCB_HEAD_0                  CCB_NIL
#define CCB_HEAD_1(_1,...)          _1
#define CCB_HEAD_2(_1,_2,...)       _1,_2
#define CCB_HEAD_3(_1,_2,_3,...)    _1,_2,_3
#define CCB_HEAD_4(_1,_2,_3,_4,...) _1,_2,_3,_4

#define CCB_HEADF(size,f,...)       CC_CAT2(CCB_HEADF_,size)(f,__VA_ARGS__) // size=[0-64]
#define CCB_HEADF_0                 CCB_NIL
#define CCB_HEADF_1(f,_1,...)       f(_1)
#define CCB_HEADF_2(f,_1,_2,...)    f(_1),f(_2)
#define CCB_HEADF_3(f,_1,_2,_3,...) f(_1),f(_2),f(_3)
#define CCB_HEADF_4(f,_1,_2,_3,_4,...) f(_1),f(_2),f(_3),f(_4)

// usage: CCB_HEADF2(size,f2,...)
#define CCB_HEADF2(size,f2,...)     CC_CAT2(CCB_HEADF2_,size)(f2,__VA_ARGS__) // size=[0-64]
#define CCB_HEADF2_0                CCB_NIL
#define CCB_HEADF2_1(f,_1,...)      f(1,_1)
#define CCB_HEADF2_2(f,_1,_2,...)   f(1,_1),f(2,_2)
#define CCB_HEADF2_3(f,_1,_2,_3,...) f(1,_1),f(2,_2),f(3,_3)
#define CCB_HEADF2_4(f,_1,_2,_3,_4,...) f(1,_1),f(2,_2),f(3,_3),f(4,_4)

// #include "cc-boost-head.h" // 5-64: head, headf, headf2

#define CCB_MAP                     CCB_HEADF // alias
#define CCB_MAP2                    CCB_HEADF2 // alias

// usage: CCB_REVERSE(size)(...)
#define CCB_REVERSE(size)           CC_CAT2(CCB_REVERSE_,size) // size=[0,64]
#define CCB_REVERSE_0               CCB_NIL
#define CCB_REVERSE_1(_1,...)       _1
#define CCB_REVERSE_2(_1,_2,...)    _2,_1
#define CCB_REVERSE_3(_1,_2,_3,...) _3,_2,_1
#define CCB_REVERSE_4(_1,_2,_3,_4,...) _4,_3,_2,_1
// #include "cc-boost-reverse.h" // 5-64

// usage: CCB_REP(size)(...)
// note: CCB_REP(size)() returns nil
#define CCB_REP(size)               CC_CAT2(CCB_REP_,size) // size=[0-64]
#define CCB_REP_0                   CCB_NIL
#define CCB_REP_1(...)              __VA_ARGS__
#define CCB_REP_2(...)              __VA_ARGS__,##__VA_ARGS__
#define CCB_REP_3(...)              __VA_ARGS__,##__VA_ARGS__,##__VA_ARGS__
#define CCB_REP_4(...)              __VA_ARGS__,##__VA_ARGS__,##__VA_ARGS__,##__VA_ARGS__
// #include "cc-boost-rep.h" // 5-64

#define CCB_REPF(size,f,...)        CC_CAT2(CCB_REP_,size)(f(__VA_ARGS__))

// useful delimiter defs for CCB_JOINF

#define CCB_COMMA(...)              ,
#define CCB_DOT(...)                .
#define CCB_PTR(...)                ->
#define CCB_SLASH(...)              /
#define CCB_STAR(...)               *
#define CCB_LANGLE(...)             < // balanced!
#define CCB_RANGLE(...)             >
#define CCB_LBRACE(...)             { // balanced!
#define CCB_RBRACE(...)             }
#define CCB_LBRACKET(...)           [ // balanced!
#define CCB_RBRACKET(...)           ]
#define CCB_LPAREN(...)             ( // balanced!
#define CCB_RPAREN(...)             )

#define CCB_JOINF(size,f1,fdelim0,...) CC_CAT2(CCB_JOINF_,size)(f1,fdelim0,__VA_ARGS__) // size=[0-64]
#define CCB_JOINF_0                 CCB_NIL
#define CCB_JOINF_1(f,d,_1,...)\
    f(_1)
#define CCB_JOINF_2(f,d,_1,_2,...)\
    f(_1)d()f(_2)
#define CCB_JOINF_3(f,d,_1,_2,_3,...)\
    f(_1)d()f(_2)d()f(_3)
#define CCB_JOINF_4(f,d,_1,_2,_3,_4,...)\
    f(_1)d()f(_2)d()f(_3)d()f(_4)

#define CCB_JOINF2(size,f2,fdelim1,...) CC_CAT2(CCB_JOINF2_,size)(f2,fdelim1,__VA_ARGS__) // size=[0-64]
#define CCB_JOINF2_0(f,d,...)       CCB_NIL
#define CCB_JOINF2_1(f,d,_1,...)\
    f(1,_1)
#define CCB_JOINF2_2(f,d,_1,_2,...)\
    f(1,_1)d(1)f(2,_2)
#define CCB_JOINF2_3(f,d,_1,_2,_3,...)\
    f(1,_1)d(1)f(2,_2)d(2)f(3,_3)
#define CCB_JOINF2_4(f,d,_1,_2,_3,_4,...)\
    f(1,_1)d(1)f(2,_2)d(2)f(3,_3)d(3)f(4,_4)

// #include "cc-boost-join.h" // 5-64: joinf, joinf2

 #define CCB_BNOT(b)                 CC_CAT2(CCB_BNOT_,b) // b=[0,1]
 #define CCB_BNOT_0                  1
 #define CCB_BNOT_1                  0

 #define CCB_BEQ(b0,b1)              CC_CAT3(CCB_BEQ_,b0,b1) // b0=[0,1], b1=[0,1]
 #define CCB_BEQ_00                  1
 #define CCB_BEQ_01                  0
 #define CCB_BEQ_10                  0
 #define CCB_BEQ_11                  1

 #define CCB_BNE(b0,b1)              CC_CAT3(CCB_BNE_,b0,b1) // b0=[0,1], b1=[0,1]
 #define CCB_BNE_00                  0
 #define CCB_BNE_01                  1
 #define CCB_BNE_10                  1
 #define CCB_BNE_11                  0

 #define CCB_BAND(b0,b1)             CC_CAT3(CCB_BAND_,b0,b1) // b0=[0,1], b1=[0,1]
 #define CCB_BAND_00                 0
 #define CCB_BAND_01                 0
 #define CCB_BAND_10                 0
 #define CCB_BAND_11                 1

 #define CCB_VBAND(size)             CC_CAT(CCB_VBAND_,size) // size=[0-64], bN=[0,1]
 #define CCB_VBAND_0(...)            1
 #define CCB_VBAND_1(b0,...)         b0
 #define CCB_VBAND_2(b0,b1,...)      CCB_BAND(b0,b1)
 #define CCB_VBAND_3(b0,b1,b2,...)   CCB_BAND(CCB_BAND(b0,b1),b2)
 #define CCB_VBAND_4(b0,b1,b2,b3,...) CCB_BAND(CCB_BAND(CCB_BAND(b0,b1),b2),b3)

 #define CCB_BOR(b0,b1)              CC_CAT3(CCB_BOR_,b0,b1)
 #define CCB_BOR_00                  0
 #define CCB_BOR_01                  1
 #define CCB_BOR_10                  1
 #define CCB_BOR_11                  1

 #define CCB_VBOR(size)              CC_CAT(CCB_VBOR_,size) // size=[0-64], bN=[0,1]
 #define CCB_VBOR_0(...)             0
 #define CCB_VBOR_1(b0,...)          b0
 #define CCB_VBOR_2(b0,b1,...)       CCB_BOR(b0,b1)
 #define CCB_VBOR_3(b0,b1,b2,...)    CCB_BOR(CCB_BOR(b0,b1),b2)
 #define CCB_VBOR_4(b0,b1,b2,b3,...) CCB_BOR(CCB_BOR(CCB_BOR(b0,b1),b2),b3)

 #define CCB_BXOR(b0,b1)             CC_CAT3(CCB_BXOR_,b0,b1) // b0=[0,1], b1=[0,1]
 #define CCB_BXOR_00                 0
 #define CCB_BXOR_01                 1
 #define CCB_BXOR_10                 1
 #define CCB_BXOR_11                 0

 #define CCB_VBXOR(size)             CC_CAT(CCB_VBXOR_,size) // size=[0-64], bN=[0,1]
 #define CCB_VBXOR_0(...)            0
 #define CCB_VBXOR_1(b0,...)         b0
 #define CCB_VBXOR_2(b0,b1,...)      CCB_BXOR(b0,b1)
 #define CCB_VBXOR_3(b0,b1,b2,...)   CCB_BXOR(CCB_BXOR(b0,b1),b2)
 #define CCB_VBXOR_4(b0,b1,b2,b3,...) CCB_BXOR(CCB_BXOR(CCB_BXOR(b0,b1),b2),b3)

#define CCB_Z(i)                    CC_CAT2(CCB_Z_,i) // i=[0-64]
#define CCB_Z_0                     1
#define CCB_Z_1                     0
#define CCB_Z_2                     0
#define CCB_Z_3                     0
#define CCB_Z_4                     0
// #include "cc-boost-z.h" // 5-64

#define CCB_NZ(i)                   CCB_BEQ(CCB_Z(i),0)

#define CCB_INCR2(i,b)              CC_CAT3(CCB_INCR_,b,i)
#define CCB_INCR_00                 0
#define CCB_INCR_10                 1
#define CCB_INCR_01                 1
#define CCB_INCR_11                 2
#define CCB_INCR_02                 2
#define CCB_INCR_12                 3
#define CCB_INCR_03                 3
#define CCB_INCR_13                 4
#define CCB_INCR_04                 4
#define CCB_INCR_14                 5
// #include "cc-boost-incr.h" // 5-63
#define CCB_INCR_064                64
#define CCB_INCR_164                CCB_E_OVERFLOW

#define CCB_INCR(i)                 CCB_INCR2(i,1)

#define CCB_DECR2(i,b)              CC_CAT3(CCB_DECR_,b,i)
#define CCB_DECR_00                 0
#define CCB_DECR_10                 CCB_E_UNDERFLOW
#define CCB_DECR_01                 1
#define CCB_DECR_11                 0
#define CCB_DECR_02                 2
#define CCB_DECR_12                 1
#define CCB_DECR_03                 3
#define CCB_DECR_13                 2
#define CCB_DECR_04                 4
#define CCB_DECR_14                 3
// #include "cc-boot-decr.h" // 5-64

#define CCB_DECR(i)                 CCB_DECR2(i,1)

// usage: CCB_VBSUM(size)(...)
#define CCB_VBSUM(size)             CC_CAT(CCB_VBSUM_,size) // size=[0-64]
#define CCB_VBSUM_0(...)            0
#define CCB_VBSUM_1(b0,...)         b0
#define CCB_VBSUM_2(b0,b1,...)      CCB_INCR2(b0,b1)
#define CCB_VBSUM_3(b0,b1,b2,...)   CCB_INCR2(CCB_INCR2(b0,b1),b2)
#define CCB_VBSUM_4(b0,b1,b2,b3,...) CCB_INCR2(CCB_INCR2(CCB_INCR2(b0,b1),b2),b3)

// #include "cc-boost-vb.h" // 5-64: vband, vbor, vbsum, vbxor

CCB_NZ(0)
CCB_NZ(3)
CCB_INDEX(1)(a,b,c)
CCB_INDEX(2)(a,b,c)
CCB_SELECT(1,(3,2,1),a,b,c)
CCB_SELECT(2,(3,2,1),a,b,c)
CCB_SELECT(3,(3,2,1),a,b,c)
CCB_REP(3)(a,b,c)
CCB_VBSUM(3)(1,1,1)
