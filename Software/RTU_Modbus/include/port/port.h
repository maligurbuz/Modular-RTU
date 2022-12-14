 /*
  * FreeModbus Libary: MCF5235 Port
  * Copyright (C) 2006 Christian Walter <wolti@sil.at>
  * Parts of crt0.S Copyright (c) 1995, 1996, 1998 Cygnus Support
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * File: $Id: port.h,v 1.2 2006/09/04 14:39:20 wolti Exp $
  */

#ifndef _PORT_H
#define _PORT_H

/* ----------------------- Platform includes --------------------------------*/

#include "stm32f4xx_conf.h"
#include "lwip/opt.h"
#include "lwip/sys.h"



/* ----------------------- Defines ------------------------------------------*/
#undef INLINE
#define INLINE                  inline

#define ENTER_CRITICAL_SECTION( )   ( __disable_irq() )
#define EXIT_CRITICAL_SECTION( )    ( __enable_irq() )

#define assert( x )             LWIP_ASSERT( #x, x );

#define PR_BEGIN_EXTERN_C       extern "C" {
#define PR_END_EXTERN_C         }

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
      /* Debug output in TCP module. */
/* ----------------------- Type definitions ---------------------------------*/
typedef char    BOOL;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef unsigned short USHORT;
typedef short   SHORT;

typedef unsigned long ULONG;
typedef long    LONG;



/* ----------------------- Function prototypes ------------------------------*/


#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
