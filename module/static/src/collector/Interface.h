#ifndef CollectorInterface_H
#define CollectorInterface_H

#include "DataTypes.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef __GNUC__
	#define AC_GCC
#else
	#error "No supported compiler found."
#endif

#if AC_GCC
	#define AC_EXPORT __attribute__((visibility("default")))
	#pragma GCC visibility push(hidden)
#endif

typedef struct {
	
} AC_Interface;

AC_Interface *AC_EXPORT AC_GetInterface();

#if AC_GCC
	#pragma GCC visibility pop
#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif
