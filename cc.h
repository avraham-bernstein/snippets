#pragma once
#define CC_H_ CC_VS(1,0,0)
#if 0 // begin:comment (must have balanced quotes and braces!)
================================================================================
FILE: cc.h
DESCRIP: Common unified base compiler *preprocessor* definitions that support
	the POSIX-2 binary API defined in <unistd.h> for the following
	programming environments:

	1. different languages: C, C++
	2. different versions of C: C99, C11
	3. differnet versions of C++: C++03, C++11, C++14
	4. different compilers: gcc, clang, msvc
	5. different O/S: linux, ios, windows

	The goals are:

	1. To minimize the need for inserting 'ifdefs' inside project source code.

	2. To minimize the need for creation of virtual layers/drivers for the O/S,
		language, or compiler.

	3. To provide a uniform syntax for generic "typeless" programming, using the
		macro CC_AUTO(varname,value), where the compiler can infer the variable
		type from its value, and therefore its type need not be specified. The
		challenge is that C99, C11, C++03, C++11, and C++14 all have a different
		syntax for specifying this same logical construct.
		For	example:

			int x = 42;
			CC_AUTO(y,x);
			// ==> gcc/clang: C99 & C++03:	__typeof(x) y = x
			// ==> gcc/clang: C11:			__auto_type y = x
			// ==> gcc/clang/msvc: C++11:	auto y = x
			// ==> gcc/clang/msvc: C++14:	decltype(auto) y = x

	4. To provide a macro "wrapper" for specifying compiler specific attributes
		and pragmas that the preprocessor ignores when the associated compiler
		is not present. And similarly to provide the infrastructure for
		specifying logically equivalent attributes and pragmas via a generic
		macro that the preprocessor resolves to	the compiler specific variant.

	The following macro wrappers have been defined for pragmas:

		#define CC_STR(_text)		__STRING(_text)

		#define CC_GNUC 			(defined(__GNUC__) ? ((10000*__GNUC__) + (100*__GUNC_MINOR__) + __GNUC_PATCHLEVEL__)) : 0)

		#define CC_CLANG 			(defined(__clang__) ? ((10000*__clang_major__) + (100*__clang_minor__) + __clang_patchlevel__)) : 0)

		#define CC_MSVC				(defined(_MSC_VER) ? _MSC_VER : 0)

		#if CC_GNUC
		#define CC_PRAG_BASE(_qstr)				_Pragma(_qstr) // builtin
		#else
		#define CC_PRAG_BASE(...)
		#endif

		#define CC_PRAG(_keyword, ...)			CC_PRAG_BASE(CC_STR(_keyword __VA_ARGS__))
		#define GCC_PRAG(_keyword, ...)			CC_PRAG(GCC _keyword, __VA_ARGS__)

		#if CC_CLANG
		#define CLANG_PRAG(_keyword, ...)		CC_PRAG(clang _keyword, __VA_ARGS__)
		#else
		#define CLANG_PRAG(...)
		#endif

		#if CC_MSVC
		#define MSVC_PRAG_BASE(_pragma_expr) 	__pragma(_pragma_expr) // builtin
		#else
		#define MSVC_PRAG_BASE(...)
		#endif

		#define MSVC_PRAG1(_keyword)			MSVC_PRAG_BASE(_keyword)
		#define MSVC_PRAG2(_keyword, ...)		MSVC_PRAG_BASE(_keyword(__VA_ARGS__))

	And the following is an implementation of a generic macro:

		#define CC_PRAG_MAC_POP(_qmacname)		CC_PRAGA(pop_macro(_qmacname))
		#define CC_PRAG_MAC_PUSH(_qmacname)		CC_PRAGA(push_macro(_qmacname))

		#define MSVC_PRAG_MAC_POP(_qmacname) 	MSVC_PRAG2(pop_macro,_qmacname)
		#define MSVC_PRAG_MAC_PUSH(_qmacname) 	MSVC_PRAG2(push_macro,_qmacname)

		#if CC_GNUC
		#define CC_GPRAG_MAC_POP				CC_PRAG_MAC_POP
		#define CC_GPRAG_MAC_PUSH				CC_PRAG_MAC_PUSH
		#elif CC_MSVC
		#define CC_GPRAG_MAC_POP				MSVC_PRAG_MAC_POP
		#define CC_GPRAG_MAC_PUSH				MSVC_PRAG_MAC_PUSH
		#endif

	Example usage:

		#define X 1
		int a = X; // ==> a = 1

		CC_GPRAG_MAC_PUSH("X")	==> gcc/clang: _Pragma("push_macro(\"X\")")
		#undef X
		#define X 2
		int b = X; // ==> b = 2

		CC_GPRAG_MAC_POP("X")	==> gcc/clang: _Pragma("pop_macro(\"X\")")
		int c = X; // ==> c = 1

LANGUAGE MINIMUM REQUIREMENTS:
	C99: full
	C11: optional
	C++03: full
	C++11: optional
	C++14: optional
	C++ Boost library must compile: yes

COMPILER MINIMUM REQUIREMENTS:
	gcc v4.6.4+ (Apr 2013)
	clang v3.4.2+ (June 2014)
	Ubuntu 14.04.1 LTS, Trusty Tahr (July 2014)
	MSVC 1900+ (2015)

O/S SUPPORT:
	linux
	IOS
	Win32

COMPILERS ACTUALLY TESTED:
	gcc/g++ v5.4.0 (June 2016)
	clang/clang++ v3.8.0 (March 2016)
	Ubuntu 16.04.2 LTS, Xenial (Feb 2017)

================================================================================
DATE: 2017-11-26T00:09:00Z
AUTHOR: Avraham DOT Bernstein AT gmail
COPYRIGHT (c) 2017 Avraham Bernstein, Jerusalem ISRAEL. All rights reserved.
LICENSE: Apache License, Version 2.0: https://opensource.org/licenses/Apache-2.0
REVISION HISTORY:
2017-11-26: 1.0.0: AB: original
================================================================================
#endif // end:comment

#ifdef __TINYC__

	#include <tcclib.h>
	#include <stdbool.h>

	// missing <sys/cdefs.h>
    #define __BEGIN_DECLS
    #define __END_DECLS
    #define __CONCAT(_x,_y)         _x ## _y
    #define __STRING(_x)            # _x

#elif !defined(__cplusplus)

	#include <features.h>
	#include <stddef.h>		// cstddef
	#include <sys/cdefs.h>
	#include <sys/types.h>

	#include <ctype.h> 		// cctype
	#include <stdint.h> 	// cstdint
	#include <inttypes.h>	// cinttypes
	#include <stdbool.h>
	#include <limits.h>		// climits
	#include <errno.h>		// cerrno

	#include <stdio.h>		// cstdio
	#include <malloc.h>
	#include <string.h>		// cstring
	#include <stdlib.h>		// cstdlib

	#include <assert.h> 	// cassert

	// #include <error.h>
	// #include <err.h>
	// #include <getopt.h>
	// #include <gettext-po.h>
	// #include <regex.h>
	// #include <signal.h>	// csignal
	// #include <time.h>	// ctime
	// #include <unistd.h> // Posix-2 API

#else

	#include <features.h>
	#include <cstddef>
	#include <sys/cdefs.h>
	#include <sys/types.h>

	#include <cctype>
	#include <cstdint>
	#include <cinttypes>
	#include <climits>
	#include <cerrno>

	#include <cstdio>
	#include <malloc.h>
	#include <cstring>
	#include <cstdlib>

	#include <cassert>

	#include <iostream>
	#include <iomanip>
	#include <stdexcept>

#endif

// =============================================================================
// Misc common macros

#define CC_VS(_major,_minor,_patch) ((_major)*10000 + (_minor)*100 + (_patch))
#define CC_VS2(_major,_minor)       CC_VS(0,_major,_minor)

#define CC_ARRAY_SIZE(_a)           (sizeof(_a)/sizeof((_a)[0]))

// =============================================================================
// <sys/cdefs.h> macro extensions

#define CC_BEGIN_DECLS              __BEGIN_DECLS
#define CC_END_DECLS                __END_DECLS

#define CC_CONCAT2(_x,_y)         	__CONCAT(_x,_y)			// deferred
#define CC_CAT2                     CC_CONCAT2          	// alias
#define CC_CAT                      CC_CONCAT2          	// alias

#define CC__CONCAT3(_x,_y,_z)    	_x ## _y ## _z
#define CC_CONCAT3(_x,_y,_z)     	CC__CONCAT3(_x,_y,_z) 	// deferred
#define CC_CAT3                     CC_CONCAT3          	// alias

#define CC__CONCAT4(_w,_x,_y,_z)   	_w ## _x ## _y ## _z
#define CC_CONCAT4(_w,_x,_y,_z)     CC__CONCAT4(_w,_x,_y,_z) // deferred
#define CC_CAT4                     CC_CONCAT4           	// alias

#define CC_STRING(_x)               __STRING(_x)         	// deferred
#define CC_STR						CC_STRING           	// alias
#define CC_QUOTE					CC_STRING				// alias

// =============================================================================
// Compiler macros
// https://sourceforge.net/p/predef/wiki/Compilers/

#define CC_GNUC                  	(defined(__GNUC__) ? CC_VS(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__) : 0)
#define CC_CLANG                 	(defined(__clang__) ? CC_VS(__clang_major__, __clang_minor__, __clang_patchlevel__) : 0)
#define CC_MSC                  	(defined(_MSC_VER) ? _MSC_VER : 0)
#define CC_TINYC                    (defined(__TINYC__) ? __TINYC__ : 0)

#if !(CC_GNUC || CC_CLANG || CC_MSC || CC_TINYC)
	#error "Unsupported compiler."
#endif

#if CC_GNUC > 0 && CC_GNUC < CC_VS(4,6,4)
	#if !CC_CLANG
		#error "GCC compiler is too old. Minimum supported version is 4.6.4."
	#endif
#endif

#if CC_CLANG > 0 && CC_CLANG < CC_VS(3,4,2)
	#error "Clang compiler is too old. Minimum supported version is 3.4.2."
#endif

#if CC_MSC > 0 && CC_MSC < CC_VS2(19,0)
	#error  "MSC compiler is too old. Minimum supported version is 1900, i.e. Visual Studio 14.0 2015."
#endif

#if CC_TINYC > 0 && CC_TINYC < CC_VS2(9,26)
	#error "TCC compiler is too old. Minimum supported version is 0926."
#endif

// =============================================================================
// Language standards macros
// https://sourceforge.net/p/predef/wiki/Standards/

#define CC_STDC                  	defined(__STDC_VERSION__)
#define CC_STDC_94                  (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L)
#define CC_STDC_99                  (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define CC_STDC_11                  (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)

#define CC_PLUS                  	defined(__cplusplus)
#define CC_PLUS_03                  (defined(__cplusplus) && __cplusplus >= 200301L)
#define CC_PLUS_11                  (defined(__cplusplus) && __cplusplus >= 201103L)
#define CC_PLUS_14                  (defined(__cplusplus) && __cplusplus >= 201402L)

#if CC_STDC == 0 && CC_PLUS == 0
	#error "C/C++ language level is undefined."
#endif

#if CC_STDC > 0 && CC_STDC < CC_STDC_99
	#error "C language compatibility is too low. Minimum supported version is 199901L, i.e C99."
#endif

#if CC_PLUS > 0 && CC_PLUS < CC_PLUS_03
	#error "C++ language compatibility is too low. Minimum supported version is 200301L, i.e C++03."
#endif

// =============================================================================
// H/W architecture macros
// https://sourceforge.net/p/predef/wiki/Architectures/

#if CC_GNUC
	#define CC_ARCH_AMD64			defined(__amd64__)
	#define CC_ARCH_X64				(defined(__x86_64__) && !defined(__amd64__))
#elif CC_MSC
	#define CC_ARCH_AMD64			defined(_M_AMD64)
	#define CC_ARCH_X64				(defined(_M_X64) && !defined(_M_AMD64))
#endif

#if CC_GNUC
	#ifdef __arm__
		#define CC_ARCH_ARM 		1
		#define CC_ARCH_ARMT		defined(__thumb__)

		#define CC_ARCH_ARM5		(defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__)|| \
									defined(__ARM_ARCH_5T__)|| defined(__ARM_ARCH_5TE__)|| defined(__ARM_ARCH_5TEJ__))

		#define CC_ARCH_ARM6		(defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || \
									defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__))

		#define CC_ARCH_ARM7		(defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || \
									defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__))

		#define CC_ARCH_ARM64		defined(__aarch64__)
	#endif
#elif CC_MSC
	#ifdef _M_ARM
		#define CC_ARCH_ARM			1
		#define CC_ARCH_ARMT		defined(_M_ARMT)
		#define CC_ARCH_ARM5		(_M_ARM == 5)
		#define CC_ARCH_ARM6		(_M_ARM == 6)
		#define CC_ARCH_ARM7		(_M_ARM == 7)
		// what about ARM64 ???
	#endif
#endif

#ifndef CC_ARCH_X64
	#define CC_ARCH_X64				0
	#define CC_ARCH_AMD64			0
#endif

#ifndef CC_ARCH_ARM
	#define CC_ARCH_ARM 			0
#endif

#ifndef CC_ARCH_ARM64
	#define CC_ARCH_ARM64 			0
#endif

// =============================================================================
// O/S macros
// https://sourceforge.net/p/predef/wiki/OperatingSystems/
// http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system#OSXiOSandDarwin

#ifdef								__ANDROID__
	#include <android/api-level.h>
	#define CC_OS_ANDROID			__ANDROID_API__
	#if __ANDROID_API__ < 19
		#error "ANDROID version is too old. Minimum supported API level is 19, i.e. Android 4.4 KitKat."
	#endif
#else
	#define CC_OS_ANDROID			0
#endif

#define CC_OS_APPLE					defined(__APPLE__)
#ifdef __APPLE__
	#include <TargetConditionals.h>
	#define CC_OS_APPLE_IOS			(TARGET_OS_IPHONE == 1)
	#define CC_OS_APPLE_OSX			(TARGET_OS_IPHONE == 0)
#else
	#define CC_OS_APPLE_IOS			0
	#define CC_OS_APPLE_OSX			0
#endif

#define CC_OS_BSD					defined(BSD)
#define CC_OS_BSD_FREE				defined(__FreeBSD__)
#define CC_OS_BSD_NET				defined(__NetBSD__)
#define CC_OS_BSD_OPEN				defined(__OpenBSD__)

#define CC_OS_CYGWIN				defined(__CYGWIN__)
#define CC_OS_ECOS					defined(__ECOS)
#define CC_OS_LINUX					defined(__linux__)
#define CC_OS_MSWIN					defined(_WIN64)
#define CC_OS_MINGW64				defined(__MINGW64__)
#define CC_OS_PLAN9					defined(pyr)

#define CC_OS_POSIX					(defined(_POSIX_VERSION) ? _POSIX_VERSION : 0)
#define CC_OS_POSIX_2001			200112L
#define CC_OS_POSIX_2008			200809L
#if CC_OS_POSIX != 0 && CC_OS_POSIX < CC_OS_POSIX_2001
	#error "POSIX version is too old. Minimum supported version is ISO POSIX.1-2001"
#endif

#define CC_OS_QNX					defined(__QNXNTO__)
#define CC_OS_UNIX					defined(__unix__)

// =============================================================================
// Link format macros

#define CC_LINK_DWARF				CC_OS_APPLE
#define CC_LINK_ELF					CC_OS_LINUX
#define CC_LINK_WIN					CC_OS_MSWIN

// =============================================================================
// C/C++ unification macros

#if CC_PLUS
    #define CC_INLINE               inline
#else
    #define CC_INLINE               static __inline__
#endif

// Implementation of "static_assert" for legacy compilers, i.e. pre C11 & C++11.

#if !(CC_STDC_11  || CC_PLUS_11)
	// _bool_expr arg must be resolvable at compile-time
	#define static_assert(_bool_expr, _q_emsg) \
		static const char * CC_CAT(static_assert_,__COUNTER__)[(_bool_expr) ? 1 : 0] \
		= { (_bool_expr_) ? "" : _q_emsg }
#endif

// The CC_AUTO macro greatly simplifies programming, because it enables type
// safe generic programming. We can implement the macro by using one of the
// following keywords:
//      decltype(auto): C++14
//      auto: C++11
//      __auto_type: GNUC: C11
//      __typeof: GNUC: C99 & C++, but has potential side effects!
// MSC C compilers do not support this important feature.

#if CC_STDC && CC_GNUC

    #if CC_STDC_11 && ( CC_CLANG >= CC_VS(3,6,0) || CC_GNUC >= CC_VS(4,9,0) )
        #define CC_AUTO(_varname,_value)        __auto_type _varname = _value
    #else
        #define CC_AUTO(_varname,_value)        __typeof(_value) _varname = _value
    #endif

#elif CC_PLUS

    #if CC_PLUS_14
        #define CC_AUTO(_varname,_value)        decltype(auto) _varname = _value
    #elif CC_PLUS_11
        #define CC_AUTO(_varname,_value)        auto _varname = _value
    #else
        #define CC_AUTO(_varname,_value)        __typeof(_value) _varname = _value
    #endif

#endif

#ifndef CC_AUTO
    #define CC_AUTO(...)            CC_AUTO__UNSUPPORTED(__VA_ARGS__)
#endif

// =============================================================================
// Attribute macros

// WARNING: Attribute & pragma keywords
// Aside from the standard C/C++ language keywords, there are many other keywords
// used for attributes and pragma. They are effectively reserved too.

// The following is a nearly complete list for GCC:

//  clang
//  default
//  dependency
//  diagnostic
//  error
//  GCC
//  hidden
//  ignored
//  internal
//  ivdep
//  message
//  optimize
//  poison
//  pop
//  pop_macro
//  pop_options
//  protected
//  push
//  push_macro
//  push_options
//  system_header
//	target
//  visibility
//  warning
//  weak

// TBD: list clang attribute & pragma keywords

// TBD: list MSVC attribute & pragma keywords

#if CC_GNUC
#define GCC_ATTRIB(...)             __attribute__((__VA_ARGS__))
//  https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#Common-Function-Attributes
//  https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html#ARM-Function-Attributes
//  https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#x86-Function-Attributes
//  https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#Common-Variable-Attributes
#else
#define GCC_ATTRIB(...)
#endif

#if CC_CLANG
#define CLANG_ATTRIB(...)           __attribute__((__VA_ARGS__))
// https://clang.llvm.org/docs/LanguageExtensions.html
#else
#define CLANG_ATTRIB(...)
#endif

#if CC_MSVC
#define MSVC_ATTRIB(_attrib)		__declspec(_attrib)
// https://msdn.microsoft.com/en-us/library/dabb5z75.aspx
#else
#define MSVC_ATTRIB(...)
#endif

// =============================================================================
// Pragma macros

// "_Pragma" is the C preprocessor builtin alternative to the "#pragma" syntax.
// The _Pragma(qstr) syntax is less cumbersome, and allows us to define first
// class pragma macros.
//
// SYNTAX GOTCHYAS!
//  1. Like "#pragma", "_Pragma" must be on a line by itself.
//  2. The _Pragma(qstr) arg does *NOT* accept string concatenation,
//      i.e. _Pragma("abcdef") != _Pragma("abc" "def")
//
// Example of a pragma with a conditional guard:
//
// Instead of writing the following cumbersome 3 lines of code per pragma invocation:
//
//      #if MYPRAGMA_ENABLED
//      #pragma keyword value
//      #endif
//
// we need to define a macro MYPRAGMA using the _Pragma syntax:
//
//      #if MYPRAGMA_ENABLED
//      #define MYPRAGMA	_Pragma("keyword value")
//      #else
//      #define MYPRAGMA
//      #endif
//
// Now each invocation can use the following 1 liner:
//
//      MYPRAGMA

#if CC_GNUC
#define CC_PRAG_BASE(_q_str)					_Pragma(_q_str) // builtin macro
#else
#define CC_PRAG_BASE(...)
#endif

// https://gcc.gnu.org/onlinedocs/gcc/Pragmas.html#Pragmas

#define CC_PRAG(_keyword, ...)     				CC_PRAG_BASE(CC_STR(_keyword __VA_ARGS__))
#define GCC_PRAG(_keyword, ...)					CC_PRAG(GCC _keyword, __VA_ARGS__)

// https://clang.llvm.org/docs/LanguageExtensions.html

#if CC_CLANG
#define CLANG_PRAG(_keyword, ...)     			CC_PRAG(clang _keyword, __VA_ARGS__)
#else
#define CLANG_PRAG(_keyword, ...)
#endif

// https://msdn.microsoft.com/en-us/library/d9xs805.aspx

#if CC_MSVC
#define MSVC_PRAG_BASE(_pragma_expr)			__pragma(_pragma_expr) // builtin macro
#else
#define MSVC_PRAG_BASE(...)
#endif

#define MSVC_PRAG1(_keyword)					MSVC_PRAG_BASE(_keyword)
#define MSVC_PRAG2(_keyword, ...)				MSVC_PRAG_BASE(_keyword(__VA_ARGS__))

// GCC Detailed Pragmas

// STDC pragmas (except MSVC)

#define CC_PRAG_MAC_POP(_q_macname)             CC_PRAG(pop_macro(_q_macname), )
#define CC_PRAG_MAC_PUSH(_q_macname)            CC_PRAG(push_macro(_q_macname), )
#define CC_PRAG_MSG(_q_msg)                     CC_PRAG(message, _q_msg)
#define CC_PRAG_PACK(...)                       CC_PRAG(pack(__VA_ARGS__),)
#define CC_PRAG_PACK_PUSH(...)                  CC_PRAG(pack(push,##__VA_ARGS__),)
#define CC_PRAG_PACK_POP()                      CC_PRAG(pack(pop),)
#define CC_PRAG_WEAK(_sym)                      CC_PRAG(weak, _sym)
#define CC_PRAG_WEAK_ALIAS(_alias,_sym)         CC_PRAG(weak, _alias = _sym)

// GCC msg pragmas

#define GCC_PRAG_ERR(_q_msg)                    GCC_PRAG(error, _q_msg)
#define GCC_PRAG_MSG                            CC_PRAG_MSG // convenience
#define GCC_PRAG_WARN(_q_msg)                   GCC_PRAG(warning, _q_msg)

// GCC diagnostic pragmas

#define GCC_PRAG_DIAG(_kind,_q_opt)				GCC_PRAG(diagnostic _kind, _q_opt)
#define GCC_PRAG_DIAG_ERR(_q_opt)          		GCC_PRAG_DIAG(error, _q_opt)
#define GCC_PRAG_DIAG_IGN(_q_opt)          		GCC_PRAG_DIAG(ignored, _q_opt)
#define GCC_PRAG_DIAG_POP()                     GCC_PRAG_DIAG(pop, )
#define GCC_PRAG_DIAG_PUSH()                    GCC_PRAG_DIAG(push, )
#define GCC_PRAG_DIAG_WARN(_q_opt)         		GCC_PRAG_DIAG(warning, _q_opt)

// GCC optimization pragmas

#define GCC_PRAG_TARG(...)						GCC_PRAG(target, (__VA_ARGS__)) // comma separated list of quoted opts
#define GCC_PRAG_OPT(...)           			GCC_PRAG(optimize, (__VA_ARGS__)) // comma separated list of quoted opts
#define GCC_PRAG_OPT_POP()                      GCC_PRAG(pop_options, )
#define GCC_PRAG_OPT_PUSH()                     GCC_PRAG(push_options, )

// GCC visibility pragmas

#define GCC_PRAG_VIS_POP()                      GCC_PRAG(visibility, pop)
#define GCC_PRAG_VIS_PUSH(_q_vis_type)          GCC_PRAG(visibility, push(_q_vis_type))
#define GCC_PRAG_VIS_PUSH_DEFAULT()             GCC_PRAG_VIS_PUSH("default")
#define GCC_PRAG_VIS_PUSH_HIDDEN()              GCC_PRAG_VIS_PUSH("hidden")
#define GCC_PRAG_VIS_PUSH_INTERNAL()            GCC_PRAG_VIS_PUSH("internal")
#define GCC_PRAG_VIS_PUSH_PROTECTED()           GCC_PRAG_VIS_PUSH("protected")

// GCC misc pragmas

// "dependency" is equivalent to the make file time dependency concept.
// This pragma is useful for files that are automatically generated.
// The pragma issues a warning when the dependent file is more recent than the
// file that invokes it.
// An alternative angle bracketted fname can be used instead of a quoted one,
// just like include files - with the identical search order ramifications.

#define GCC_PRAG_DEP(_q_fname)                  GCC_PRAG(dependency, _q_fname)

// "ivdep" to be placed immediately before a loop, signifies that the loop is
// order independent, and therefore the compiler could implement it with SIMD
// instructions - assuming that the processor supports them
// TBD: clang has much richer loop optimization pragmas.

#if !CC_CLANG
#define GCC_PRAG_IVDEP()                        GCC_PRAG(ivdep, )
#else
#define GCC_PRAG_IVDEP()
#endif

// "poison" will generate a hard error when any of the specified functions are
// used in the compilation unit *following* the directive

#define GCC_PRAG_POISON(_func_list)             GCC_PRAG(poison, _func_list) // list is SPACE separated

// When "system_header" is placed in an include file, all subsequent code in the
// include file will no longer report any compilation warnings

#define GCC_PRAG_SYSHDR()                       GCC_PRAG(system_header, )

// TBD: clang pragmas

// TBD: MSVC pragmas

#define MSVC_PRAG_MAC_POP(_q_macname)			MSVC_PRAG2(pop_macro, _q_macname)
#define MSVC_PRAG_MAC_PUSH(_q_macname)			MSVC_PRAG2(push_macro, _q_macname)

// TBD: generic pragmas

#if CC_GNUC
#define CC_GPRAG_MAC_POP(_q_macname)			CC_PRAG_MAC_POP(_q_macname)
#define CC_GPRAG_MAC_PUSH(_q_macname)			CC_PRAG_MAC_PUSH(_q_macname)
#elif CC_MSVC
#define CC_GPRAG_MAC_POP(_q_macname)			MSVC_PRAG_MAC_POP(_q_macname)
#define CC_GPRAG_MAC_PUSH(_q_macname)			MSVC_PRAG_MAC_PUSH(_q_macname)
#else
#define CC_GPRAG_MAC_POP(...)					CC_GPRAG_MAC_POP__UNDEFINED
#define CC_GPRAG_MAC_PUSH(...)					CC_GPRAG_MAC_PUSH__UNDEFINED
#endif

// Visibility, see: https://gcc.gnu.org/wiki/Visibility

#define CC_MODULE_BUILD				0 	// default
// CC_MODULE_BUILD is used for:
// 1. link-time non-zero initialization of global vars
// 2. Windows linker only: linker visibility of global vars and funcs

#if CC_LINK_WIN

	#if CC_MSVC

  		#define CC_PUBLIC_EXPORT	MSVC_ATTRIB(dllexport)
  		#define CC_PUBLIC_IMPORT	MSVC_ATTRIB(dllimport)

  	#else

  		#define CC_PUBLIC_EXPORT	GCC_ATTRIB(dllexport)
  		#define CC_PUBLIC_IMPORT	GCC_ATTRIB(dllimport)

	#endif

	#define CC_PUBLIC				CC_PUBLIC_IMPORT	// default
	#define CC_LOCAL

#else

	#define CC_PUBLIC				GCC_ATTRIB(visibility("default"))
    #define CC_LOCAL  				GCC_ATTRIB(visibility("hidden"))

#endif

// "modules.h" database has the following type of entries (which must be cpp definitions):
//
//		#define MODULE__MAX			256		// used for dynamic debugging & tracing bit arrays
//		#define MODULE_ABC			1
//		#define MODULE_XYZ			99
//
//  	...
//
//		#if !defined(CC_MODULE_ID) || CC_MODULE_ID <= 0 || CC_MODULE_ID >= MODULE__MAX
//		#error "CC_MODULE_ID is not defined correctly."
//		#endif

// Visibility: Example
#if 0 // begin:comment

// at the beginning of the .h file

CC_GPRAG_MAC_PUSH("CC_MODULE_BUILD")
#undef CC_MODULE_BUILD
#define CC_MODULE_BUILD				(CC_MODULE_ID == MODULE_XYZ)

#if CC_LINK_WIN && CC_MODULE_BUILD
CC_GPRAG_MAC_PUSH("CC_PUBLIC")
#undef CC_PUBLIC
#define CC_PUBLIC					CC_PUBLIC_EXPORT
#endif

// ... body of .h file ...

// at the end of the .h file
#if  CC_LINK_WIN && CC_MODULE_BUILD
CC_GPRAG_MAC_POP("CC_PUBLIC")
#endif

CC_GPRAG_MAC_POP("CC_MODULE_BUILD")

#endif // end:comment
