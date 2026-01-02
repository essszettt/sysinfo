/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: zxn_mem2hex.c                                                      |
| project:  ZX Spectrum Next - libzxn                                          |
| author:   S. Zell                                                            |
| date:     01/02/2026                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| description:                                                                 |
|                                                                              |
| Functions to trim whitespace from strings                                    |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| Copyright (c) 01/02/2026 STZ Engineering                                     |
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
#include "libzxn.h"

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Namespaces                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Konstanten                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Variablen                                    */
/*============================================================================*/

/*============================================================================*/
/*                               Strukturen                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Typ-Definitionen                             */
/*============================================================================*/

/*============================================================================*/
/*                               Prototypen                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Klassen                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Implementierung                              */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* zxn_mem2hex()                                                              */
/*----------------------------------------------------------------------------*/
size_t zxn_mem2hex(const void* const pData,
                   size_t uiSize,
                   char_t* acBuffer,
                   size_t uiBufferSize,
                   uint8_t uiGrouping)
{
  size_t uiReturn = 0;

  if ((0 != acBuffer) && (0 != uiBufferSize))
  {
    acBuffer[0] = '\0';

    if ((0 != pData) && (0 != uiSize))
    {
      char_t* pSrc     = (char_t*) pData;
      char_t* pSrcEnd  = pSrc + uiSize;
      char_t* pDest    = (char_t*) acBuffer;
      char_t* pDestEnd = pDest + uiBufferSize - 1;

      while (pSrc < pSrcEnd)
      {
        if (pDest < pDestEnd)
        {
          *pDest = zxn_nibble2hex(*pSrc >> 4);
          ++pDest;
        }

        if (pDest < pDestEnd)
        {
          *pDest = zxn_nibble2hex(*pSrc);
          ++pDest;
        }

        if (0 != uiGrouping)
        {
          if (0 == ((size_t) (pSrc - ((char_t*) pData) + 1) % uiGrouping))
          {
            if ((pDest < pDestEnd) && ((pSrc + 1) < pSrcEnd))
            {
              *pDest = ' ';
              ++pDest;
            }
          }
        }

        if (pDest >= pDestEnd)
        {
          break;
        }
        else
        {
          ++pSrc;
        }
      }

      *pDest = 0;

      uiReturn = (size_t) (pDest - acBuffer);
    }
  }

  return uiReturn;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
