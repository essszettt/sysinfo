/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: dumpVars.c                                                         |
| project:  ZX Spectrum Next - SYSINFO                                         |
| author:   Stefan Zell                                                        |
| date:     09/15/2025                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| description:                                                                 |
|                                                                              |
| Read and show system information of the ZX Spectrum Next (dot command)       |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| Copyright (c) 09/15/2025 STZ Engineering                                     |
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
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#include <arch/zxn.h>
#include <arch/zxn/sysvar.h>

#include "sysinfo.h"
#include "dumpVars.h"

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/
/*!
Size of the buffer to render the value of a system variable
*/
#define VALUE_LEN_MAX LINE_LEN_MAX

/*============================================================================*/
/*                               Namespaces                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Konstanten                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Variablen                                    */
/*============================================================================*/
/*!
List of all system variables that will be dumped
@code
 FLAGS
=======

BIT7 = Variable is a pointer
@endcode
*/
static const varentry_t g_tVariables[] = 
{
  {0x5B00, 0x10, 0x00, "SWAP"     },
  {0x5B10, 0x11, 0x00, "STOO"     },
  {0x5B21, 0x09, 0x00, "YOUNGER"  },
  {0x5B2A, 0x10, 0x00, "REGNUOY"  },
  {0x5B3A, 0x18, 0x00, "ONERR"    },
  {0x5B52, 0x02, 0x00, "OLDHL"    },
  {0x5B54, 0x02, 0x00, "OLDBC"    },
  {0x5B56, 0x02, 0x00, "OLDAF"    },
  {0x5B58, 0x01, 0x00, "CACHEBNK" },
  {0x5B59, 0x01, 0x00, "RESERVED" },
  {0x5B5A, 0x02, 0x80, "RETADDR"  }, // ptr
  {0x5B5C, 0x01, 0x00, "BANKM"    },
  {0x5B5D, 0x01, 0x00, "RAMRST"   },
  {0x5B5E, 0x01, 0x00, "RAMERR"   },
  {0x5B5F, 0x01, 0x00, "INKL"     },
  {0x5B60, 0x01, 0x00, "INK2"     },
  {0x5B61, 0x01, 0x00, "ATTRULA"  },
  {0x5B62, 0x01, 0x00, "ATTRRHR"  },
  {0x5B63, 0x01, 0x00, "ATTRRHC"  },
  {0x5B64, 0x01, 0x00, "INKMASK"  },
  {0x5B65, 0x01, 0x00, "LSBANK"   },
  {0x5B66, 0x01, 0x00, "FLAGS3"   },
  {0x5B67, 0x01, 0x00, "BANK678"  },
  {0x5B68, 0x01, 0x00, "FLAGN"    },
  {0x5B69, 0x01, 0x00, "MAXBNK"   },
  {0x5B6A, 0x02, 0x00, "OLDSP"    },
  {0x5B6C, 0x02, 0x80, "SYNRET"   }, // ptr
  {0x5B6E, 0x05, 0x00, "LASTV"    },
  {0x5B73, 0x01, 0x00, "TILEBNKL" },
  {0x5B74, 0x01, 0x00, "TILEML"   },
  {0x5B75, 0x01, 0x00, "TILEBNK2" },
  {0x5B76, 0x01, 0x00, "TILEM2"   },
  {0x5B77, 0x01, 0x00, "NXTBNK"   },
  {0x5B78, 0x01, 0x00, "DATABNK"  },
  {0x5B79, 0x01, 0x00, "LODDRV"   },
  {0x5B7A, 0x01, 0x00, "SAVDRV"   },
  {0x5B7B, 0x01, 0x00, "L2SOFT"   },
  {0x5B7C, 0x02, 0x00, "TILEWL"   },
  {0x5B7E, 0x02, 0x00, "TILEWL"   },
  {0x5B80, 0x02, 0x00, "TILEOFFL" },
  {0x5B82, 0x02, 0x00, "TILEOFF2" },
  {0x5B84, 0x02, 0x00, "COORDSX"  },
  {0x5B86, 0x02, 0x00, "COORDSY"  },
  {0x5B88, 0x01, 0x00, "PAPERL"   },
  {0x5B89, 0x01, 0x00, "PAPER2"   },
  {0x5B8A, 0x75, 0x00, "TMPVARS"  },
  {0x5BFF, 0x01, 0x00, "TSTACK"   },
  {0x5C00, 0x08, 0x00, "KSTATE"   },
  {0x5C08, 0x01, 0x00, "LASTK"    },
  {0x5C09, 0x01, 0x00, "REPDEL"   },
  {0x5C0A, 0x01, 0x00, "REPPER"   },
  {0x5C0B, 0x02, 0x80, "RETVARS"  }, // ptr
  {0x5C0D, 0x01, 0x00, "K_DATA"   },
  {0x5C0E, 0x01, 0x00, "TVDATA"   },
  {0x5C10, 0x26, 0x00, "STRMS"    },
  {0x5C36, 0x02, 0x80, "CHARS"    }, // ptr
  {0x5C38, 0x01, 0x00, "RASP"     },
  {0x5C39, 0x01, 0x00, "PIP"      },
  {0x5C3A, 0x01, 0x00, "ERRNR"    },
  {0x5C3B, 0x01, 0x00, "FLAGS"    },
  {0x5C3C, 0x01, 0x00, "TVFLAG"   },
  {0x5C3D, 0x02, 0x80, "ERRSP"    }, // ptr
  {0x5C3F, 0x02, 0x00, "RESERVED" },
  {0x5C41, 0x01, 0x00, "MODE"     },
  {0x5C42, 0x02, 0x00, "NEWPPC"   },
  {0x5C44, 0x01, 0x00, "RESERVED" },
  {0x5C45, 0x02, 0x00, "PPC"      },
  {0x5C47, 0x01, 0x00, "SUBPPC"   },
  {0x5C48, 0x01, 0x00, "BORDCR"   },
  {0x5C49, 0x02, 0x00, "E_PPC"    },
  {0x5C4B, 0x02, 0x80, "VARS"     }, // ptr
  {0x5C4D, 0x02, 0x80, "DEST"     }, // ptr
  {0x5C4F, 0x02, 0x80, "CHANS"    }, // ptr
  {0x5C51, 0x02, 0x80, "CURCHL"   }, // ptr
  {0x5C53, 0x02, 0x80, "PROG"     }, // ptr
  {0x5C55, 0x02, 0x80, "NXTLIN"   }, // ptr
  {0x5C57, 0x02, 0x80, "DATADD"   }, // ptr
  {0x5C59, 0x02, 0x80, "E_LINE"   }, // ptr
  {0x5C5B, 0x02, 0x80, "CUR"      }, // ptr
  {0x5C5D, 0x02, 0x80, "CH_ADD"   }, // ptr
  {0x5C5F, 0x02, 0x80, "X_PTR"    }, // ptr
  {0x5C61, 0x02, 0x80, "WORKSP"   }, // ptr
  {0x5C63, 0x02, 0x80, "STKBOT"   }, // ptr
  {0x5C65, 0x02, 0x80, "STKEND"   }, // ptr
  {0x5C67, 0x02, 0x00, "BREG"     },
  {0x5C68, 0x02, 0x80, "MEM"      }, // ptr
  {0x5C6A, 0x01, 0x00, "FLAGS2"   },
  {0x5C6B, 0x01, 0x00, "DF_SZ"    },
  {0x5C6C, 0x02, 0x00, "RESERVED" },
  {0x5C6E, 0x02, 0x00, "OLDPPC"   },
  {0x5C70, 0x01, 0x00, "OSPPC"    },
  {0x5C71, 0x01, 0x00, "FLAGX"    },
  {0x5C72, 0x02, 0x00, "STRLEN"   },
  {0x5C74, 0x02, 0x80, "T_ADDR"   }, // ptr
  {0x5C76, 0x02, 0x00, "SEED"     },
  {0x5C78, 0x03, 0x00, "FRAMES"   }, // uint24_t
  {0x5C7B, 0x02, 0x80, "UDG"      }, // ptr
  {0x5C7D, 0x02, 0x00, "COORDS"   },
  {0x5C7F, 0x01, 0x00, "GMODE"    },
  {0x5C80, 0x01, 0x80, "PRCC"     }, // ptr
  {0x5C81, 0x01, 0x00, "STIMEOUT" },
  {0x5C82, 0x02, 0x00, "ECHO_E"   },
  {0x5C84, 0x02, 0x80, "DF_CC"    }, // ptr
  {0x5C86, 0x02, 0x80, "DF_CCL"   }, // ptr
  {0x5C88, 0x02, 0x00, "S_POSN"   },
  {0x5C8A, 0x02, 0x00, "SPOSNL"   },
  {0x5C8C, 0x01, 0x00, "SCR_CT"   },
  {0x5C8D, 0x01, 0x00, "ATTR_P"   },
  {0x5C8E, 0x01, 0x00, "MASK_P"   },
  {0x5C8F, 0x01, 0x00, "ATTR_T"   },
  {0x5C90, 0x01, 0x00, "MASK_T"   },
  {0x5C91, 0x01, 0x00, "P_FLAG"   },
  {0x5C92, 0x1E, 0x00, "MEMBOT"   },
  {0x5CB0, 0x02, 0x00, "RESERVED" },
  {0x5CB2, 0x02, 0x80, "RAMTOP"   }, // ptr
  {0x5CB4, 0x02, 0x80, "P_RAMT"   }, // ptr
  /* --------------------------- */
  {0xFFFF, 0x00, 0x00, 0          }
};

/*!
Diese Tabelle dient zur effizienten Umrechnung eines numerischen Wertes in eine
hexadezimale Zifffer (ASCII).
*/
static const unsigned char g_acHexDigits[] = "0123456789ABCDEF";

/*!
Buffer to render the value of system variable
*/
static unsigned char g_acValue[VALUE_LEN_MAX];

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
/* dumpVariables()                                                            */
/*----------------------------------------------------------------------------*/
int dumpVariables(void)
{
  int iReturn = EOK;

  union 
  {
    uint8_t  uiRaw8[4];
    uint16_t uiRaw16[2];
    uint32_t uiRaw32;
  } value;
  
  zprintf("--- SYSTEM VARIABLES -----------\n");

  const varentry_t* pVar = &g_tVariables[0];
  while (0 != pVar->acName)
  {
    value.uiRaw32 = 0;  

    switch (pVar->uiSize)
    {
      case 0:
        snprintf(g_acValue, sizeof(g_acValue), "%s", "NIL");
        break;

      case 1:
        value.uiRaw8[0] = ((uint8_t*) pVar->uiAddress)[0];
        snprintf(g_acValue, sizeof(g_acValue), "0x%02X", value.uiRaw8[0]);
        break;

      case 2:
        value.uiRaw8[0] = ((uint8_t*) pVar->uiAddress)[0]; 
        value.uiRaw8[1] = ((uint8_t*) pVar->uiAddress)[1]; 

        if (0 != (pVar->uiFlags & (1 << 7))) /* ptr ? */
        {
          if (0 != value.uiRaw16[0]) /* NULL ? */
          {
            snprintf(g_acValue, sizeof(g_acValue), "(%04X)", value.uiRaw16[0]);
          }
          else
          {
            snprintf(g_acValue, sizeof(g_acValue), "(%s)", "nil");
          }
        }
        else
        {
          snprintf(g_acValue, sizeof(g_acValue), "0x%04X", value.uiRaw16[0]);
        }
        break;

      case 3:
        value.uiRaw8[0] = ((uint8_t*) pVar->uiAddress)[0]; 
        value.uiRaw8[1] = ((uint8_t*) pVar->uiAddress)[1]; 
        value.uiRaw8[2] = ((uint8_t*) pVar->uiAddress)[2]; 
        snprintf(g_acValue, sizeof(g_acValue), "0x%06lX", value.uiRaw32);
        break;

      default:
        mem2hex(((uint8_t*) pVar->uiAddress), pVar->uiSize, g_acValue, sizeof(g_acValue), 1);
    }

    zprintf(DUMP_VARNUM "-" DUMP_VARNAME " = %s\n", pVar->uiAddress, pVar->acName, g_acValue);

    switch (pVar->uiAddress)
    {
      case 0x5C41: /* MODE */
        {
          const unsigned char* acModes[] = {"C|K|L", "E", "G", ""};
          zprintf("   + " DUMP_VARSUB " = %s\n", "CURSOR", acModes[value.uiRaw8[0] & 0x03]);
        }
        break;

      case 0x5C48: /* BORDCR */
        zprintf("   + " DUMP_VARSUB " = %u\n", "BORDER", (value.uiRaw8[0] >> 3) & 0x07);
        break;

      case 0x5C6A: /* FLAGS2 */
        zprintf("   + " DUMP_VARSUB " = %s\n", "CAPS", value.uiRaw8[0] & (1 << 3) ? "on" : "off");
        break;

      case 0x5C8D: /* ATTR_P */
      case 0x5C8F: /* ATTR_T */
        zprintf("   + " DUMP_VARSUB " = %u\n", "FLASH",   value.uiRaw8[0] & (1 << 7) ? 1 : 0);
        zprintf("   + " DUMP_VARSUB " = %u\n", "BRIGHT",  value.uiRaw8[0] & (1 << 6) ? 1 : 0);
        zprintf("   + " DUMP_VARSUB " = %u\n", "PAPER",  (value.uiRaw8[0] >> 3) & 0x07);
        zprintf("   + " DUMP_VARSUB " = %u\n", "INK",     value.uiRaw8[0]       & 0x07);
        break;

      case 0x5C82: /* ECHO_E  */
      case 0x5C88: /* S_POSN  */
      case 0x5C8A: /* S_POSNL */
        zprintf("   + " DUMP_VARSUB " = %u\n", "COL", value.uiRaw8[0]);
        zprintf("   + " DUMP_VARSUB " = %u\n", "ROW", value.uiRaw8[1]);
    }

    ++pVar;
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/* nibble2hex()                                                               */
/*----------------------------------------------------------------------------*/
char nibble2hex(unsigned char uiValue)
{
  return g_acHexDigits[uiValue & 0x0F];
}


/*----------------------------------------------------------------------------*/
/* mem2hex()                                                                  */
/*----------------------------------------------------------------------------*/
unsigned long mem2hex(const void* const pData,
                      unsigned long uiSize,
                      char* acBuffer,
                      unsigned long uiBufferSize,
                      unsigned long uiGrouping)
{
  unsigned long uiReturn = 0;

  if ((0 != acBuffer) && (0 != uiBufferSize))
  {
    acBuffer[0] = '\0';

    if ((0 != pData) && (0 != uiSize))
    {
      char* pSrc     = (char*) pData;
      char* pSrcEnd  = pSrc + uiSize;
      char* pDest    = (char*) acBuffer;
      char* pDestEnd = pDest + uiBufferSize - 1;

      while (pSrc < pSrcEnd)
      {
        if (pDest < pDestEnd)
        {
          *pDest = nibble2hex(*pSrc >> 4);
          ++pDest;
        }

        if (pDest < pDestEnd)
        {
          *pDest = nibble2hex(*pSrc);
          ++pDest;
        }

        if (0 != uiGrouping)
        {
          if (0 == ((unsigned long)(pSrc - ((char*) pData) + 1) % uiGrouping))
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

      uiReturn = (unsigned long)(pDest - acBuffer);
    }
  }

  return uiReturn;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
