/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: zxn_ink.c                                                          |
| project:  ZX Spectrum Next - libzxn                                          |
| author:   S. Zell                                                            |
| date:     01/13/2026                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| description:                                                                 |
|                                                                              |
| Function to set the ink color                                                |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| Copyright (c) 01/13/2026 STZ Engineering                                     |
|                                                                              |
| This software is provided  "as is",  without warranty of any kind, express   |
| or implied. In no event shall STZ or its contributors be held liable for any |
| direct, indirect, incidental, special or consequential damages arising out   |
| of the use of or inability to use this software.                             |
|                                                                              |
| Permission is granted to anyone  to use this  software for any purpose,      |
| including commercial applications,  and to alter it and redistribute it      |
| freely, subject to the following restrictions:                               |
|                                                                              |
| 1. Redistributions of source code must retain the above copyright            |
|    notice, definition, disclaimer, and this list of conditions.              |
|                                                                              |
| 2. Redistributions in binary form must reproduce the above copyright         |
|    notice, definition, disclaimer, and this list of conditions in            |
|    documentation and/or other materials provided with the distribution.      |
|                                                                          ;-) |
+-----------------------------------------------------------------------------*/

/*============================================================================*/
/*                               Includes                                     */
/*============================================================================*/
#include <stdint.h>
#include <stdio.h>
#include "libzxn.h"

/*============================================================================*/
/*                               Macros                                       */
/*============================================================================*/

/*============================================================================*/
/*                               Constants                                    */
/*============================================================================*/

/*============================================================================*/
/*                               Variables                                    */
/*============================================================================*/

/*============================================================================*/
/*                               Structures                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Type-Definitions                             */
/*============================================================================*/

/*============================================================================*/
/*                               Prototypes                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Implementation                               */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* zxn_ink_fastcall()                                                         */
/*----------------------------------------------------------------------------*/
void zxn_ink_fastcall(uint8_t uiColor) __z88dk_fastcall
{
  fputc((int) 0x10, stdout);
  fputc((int) ((uiColor & 0x38 ? uiColor >> 3 : uiColor) & 0x07), stdout);
  fflush(stdout);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
