/*
 * FreeModbus Libary: lwIP Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
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
 * File: $Id: portother.c,v 1.2 2006/09/04 14:39:20 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "port/port.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_FRAME_LOG_BUFSIZE    512

/* ----------------------- Start implementation -----------------------------*/



