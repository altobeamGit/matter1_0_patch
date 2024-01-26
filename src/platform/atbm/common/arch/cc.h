/*
 * cc.h
 *
 *  Created on: 2019-8-10
 *      Author: NANXIAOFENG
 */

#ifndef CC_H_
#define CC_H_

#include <stddef.h>
#include <inttypes.h>
#include "lwip/lwipopts.h"

//typedef unsigned int uintptr_t;
typedef int sys_prot_t;

#define U16_F		"u"
#define U32_F		"u"

#define X8_F        "02x"
#define X16_F		"x"
#define X32_F		"x"

#define S16_F		"d"
#define S32_F		"d"

#ifndef SZT_F
#define SZT_F U32_F
#endif /* SZT_F */

#ifndef NULL
#define NULL		((void*)0)
#endif

#define PACK_STRUCT_FIELD(x)	x
#define PACK_STRUCT_STRUCT	__attribute__ ((__packed__))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

/* Platform specific diagnostic output */

#define DEBUG2( ...)						\
	do{							\
		iot_printf(__VA_ARGS__);			\
	} while( 0)

#ifdef LWIP_DEBUG
#define LWIP_PLATFORM_DIAG(x)		DEBUG2 x
#define LWIP_PLATFORM_ASSERT(x)		DEBUG(1,1, x "\n")
#else
#define LWIP_PLATFORM_DIAG(x)		//DEBUG2 x
#define LWIP_PLATFORM_ASSERT(x)		//DEBUG(1,1, x "\n")
#endif

/* "lightweight" synchronization mechanisms */
//#define SYS_ARCH_DECL_PROTECT(x)	/* - declare a protection state variable. */
//#define SYS_ARCH_PROTECT(x)		/* - enter protection mode. */
//#define SYS_ARCH_UNPROTECT(x)		/* - leave protection mode. */

#define LWIP_RAND() ((u32_t)hal_random_get())

#endif /* CC_H_ */
