/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: dumpRegs.c                                                         |
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
#include <errno.h>
#include <arch/zxn.h>

#include "sysinfo.h"
#include "dumpRegs.h"

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

/*!
List of all NEXT registers that will be dumped
*/
const regentry_t g_tRegisters[] = 
{
  {0x00, true,  "MACHINEID"  },
  {0x01, true,  "COREVERSION"},
  {0x02, true,  "RESET"      },
  {0x03, true,  "MACHINETYPE"},
  {0x04, true,  "CONFMAPPING"},
  {0x05, true,  "PERIPHERAL1"},
  {0x06, true,  "PERIPHERAL2"},
  {0x07, true,  "CPUSPEED"   },
  {0x08, true,  "PERIPHERAL3"},
  {0x09, true,  "PERIPHERAL4"},
  {0x0A, true,  "PERIPHERAL5"},
  {0x0B, true,  "JOYPORTMODE"},
  {0x0E, true,  "COREVERSUB" },
  {0x0F, true,  "BOARDID"    },
  {0x10, true,  "COREBOOT"   },
  {0x11, true,  "VIDEOTIMING"},
  {0x12, true,  "L2ACTRAMBNK"},
  {0x13, true,  "L2SHARAMBNK"},
  {0x14, true,  "GLBTRNSCLR" },
  {0x15, true,  "SPRLYSYSSTP"},
  {0x16, true,  "L2HSCRLCTRL"},
  {0x17, true,  "L2VSCRLCTRL"},
  {0x18, true,  "L2CLPWINDEF"},
  {0x19, true,  "SPCLPWINDEF"},
  {0x1A, true,  "L0CLPWINDEF"},
  {0x1B, true,  "L3CLPWINDEF"},
  {0x1C, true,  "CLPWINCTRL" },
  {0x1E, true,  "ACTVIDLNMSB"},
  {0x1F, true,  "ACTVIDLNLSB"},
  {0x20, true,  "MASKINTGNRT"},
  {0x22, true,  "LINEINTCTRL"},
  {0x26, true,  "ULAHSCRCTRL"},
  {0x27, true,  "ULAVSCRCTRL"},
  {0x28, true,  "PS2KYMPADMB"},
  {0x29, true,  "PS2KYMPADLB"},
  {0x2A, true,  "PS2KYMPDTMB"},
  {0x2B, true,  "PS2KYMPDTLB"},
  {0x2C, true,  "DACBMIRROR" },
  {0x2D, true,  "DACADMIRROR"},
  {0x2E, true,  "DACCMIRROR" },
  {0x2F, true,  "L3HSCRCTRLM"},
  {0x30, true,  "L3HSCRCTRLL"},
  {0x31, true,  "L3VSCRCTRL" },
  {0x32, true,  "L10HSCRCTRL"},
  {0x33, true,  "L10VSCRCTRL"},
  {0x34, true,  "SPRITENUMBR"},
  {0x35, true,  "SPRITEARR0" },
  {0x36, true,  "SPRITEARR1" },
  {0x37, true,  "SPRITEARR2" },
  {0x38, true,  "SPRITEARR3" },
  {0x39, true,  "SPRITEARR4" },
  {0x40, true,  "PALINDEXSEL"},
  {0x41, true,  "8BITPALDATA"},
  {0x42, true,  "EULAATTRFMT"},
  {0x43, true,  "PALETTECTRL"},
  {0x44, true,  "9BITPALDATA"},
  {0x4A, true,  "FBACKCOLVAL"},
  {0x4B, true,  "SPRTRNSPIDX"},
  {0x4C, true,  "L3TRNSPIDX" },
  {0x50, true,  "MMUSLT0CTRL"},
  {0x51, true,  "MMUSLT1CTRL"},
  {0x52, true,  "MMUSLT2CTRL"},
  {0x53, true,  "MMUSLT3CTRL"},
  {0x54, true,  "MMUSLT4CTRL"},
  {0x55, true,  "MMUSLT5CTRL"},
  {0x56, true,  "MMUSLT6CTRL"},
  {0x57, true,  "MMUSLT7CTRL"},

  {0xFF, false, "RESERVED"   },
  /* ---------------------- */
  {0x00, false, 0            }
};

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
/* dumpRegisters()                                                            */
/*----------------------------------------------------------------------------*/
int dumpRegisters(void)
{
  int iReturn = EOK;

  uint8_t  uiValue;
  uint16_t uiValue16;

  zprintf("--- NEXT REGISTERS -------------\n");

  const regentry_t* pReg = &g_tRegisters[0];
  while (0 != pReg->acName)
  {
    if (pReg->bVisible)
    {
      uiValue = (0x07 != pReg->uiNumber ? ZXN_READ_REG(pReg->uiNumber) : _cpuspeed());  /* UGLY HACK ! */

      zprintf(DUMP_REGNUM "-" DUMP_REGNAME " = 0x%02X\n", pReg->uiNumber, pReg->acName, uiValue);

      switch (pReg->uiNumber)
      {
        case 0x00: /* 00 MACHINEID ------------------------------------------ */
          zprintf(" + " DUMP_REGSUB " = ", "MACHINEID");
          switch (uiValue)
          {
            case 0b00001000:
              zprintf("Emulator");
              break;
            case 0b00001010:
              zprintf("ZX Spectrum Next");
              break;
            case 0b11111010:
              zprintf("ZX Spectrum Next Anti Brick");
              break;
            case 0b10011010:
              zprintf("NEXT Core on UnAmiga Reloaded");
              break;
            case 0b10101010:
              zprintf("NEXT Core on UnAmiga");
              break;
            case 0b10111010:
              zprintf("NEXT Core on SiDi");
              break;
            case 0b11001010:
              zprintf("NEXT Core on MiST");
              break;
            case 0b11011010:
              zprintf("NEXT Core on MiSTer");
              break;
            case 0b11101010:
              zprintf("NEXT Core on ZX-DOS/gomaDOS");
              break;
            default:
              zprintf("UNKNOWN: 0x%X", uiValue);
          }
          zprintf("\n");
          break;

        case 0x01: /* 01 COREVERSION ---------------------------------------- */
          uiValue16  = uiValue << 8;
          uiValue16 |= ZXN_READ_REG(REG_SUB_VERSION);
          zprintf(" + " DUMP_REGSUB " = %u.%02u.%02u\n", "VERSION",
                                                         (uiValue16 >> 12) & 0x0F,
                                                         (uiValue16 >>  8) & 0x0F,
                                                          uiValue16        & 0xFF);
          break;

        case 0x02: /* 02 RESET ---------------------------------------------- */
          zprintf(" + " DUMP_REGSUB " = ", "LSTSYSRSTTY");
          switch (uiValue  & 0x03)
          {
            case 0x01:
              zprintf("Soft Reset");
              break;
            case 0x02:
              zprintf("Hard Reset");
              break;
            default:
              zprintf("UNKNOWN: 0x%X", uiValue & 0x03);
          }
          zprintf("\n");
          zprintf(" + " DUMP_REGSUB " = divMMC NMI %sgenerated by NR 0x02\n", "DVMMCNMISRC", uiValue & (1 << 2) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = MF NMI %sgenerated by NR 0x02\n",     "MFNMISOURCE", uiValue & (1 << 3) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = RESET %sasserted\n",                  "EXTBUSRSTFG", uiValue & (1 << 7) ? "" : "not ");
          break;

        case 0x03: /* 03 MACHINETYPE ---------------------------------------- */
          for (uint8_t i = 0; i < 2; ++i)
          {
            if (0 == i)
            {
              uiValue16 = uiValue & 0x07; 
              zprintf(" + " DUMP_REGSUB " = ", "MACHINETYPE");
            }
            else
            {
              uiValue16 = (uiValue >> 4) & 0x07; 
              zprintf(" + " DUMP_REGSUB " = ", "DISPTIMING");
            }

            switch (uiValue16)
            {
              case 0x00:
                zprintf("Config Mode");
                break;
              case 0x01:
                zprintf("ZX 48K");
                break;
              case 0x02:
                zprintf("ZX 128K/+2");
                break;
              case 0x03:
                zprintf("ZX +2A/+2B/+3/NEXT");
                break;
              case 0x04:
                zprintf("Pentagon Clone");
                break;
              default:
                zprintf("UNKNOWN: 0x%X", uiValue16 & 0xFF);
            }
            zprintf("\n");
          }
          break;

        case 0x05: /* 05 Peripheral 1 Settings ------------------------------ */
          zprintf(" + " DUMP_REGSUB " = %s\n",    "SCANDOUBLER", uiValue & (1 << 0) ? "on" : "off");
          zprintf(" + " DUMP_REGSUB " = %u Hz\n", "VERTFREQ",    uiValue & (1 << 2) ? 60   : 50);

          for (uint8_t i = 0; i < 2; ++i)
          {
            if (0 == i)
            {
              uiValue16 = ((uiValue & (3 << 6)) >> 5) | ((uiValue & (1 << 3)) >> 3);
            }
            else
            {
              uiValue16 = ((uiValue & (3 << 4)) >> 3) | ((uiValue & (1 << 1)) >> 1);
            }

            zprintf(" + JOYSTICK%u   = ", i + 1);
            switch (uiValue16 & 0x07)
            {
              case 0x00:
                zprintf("Sinclair2");
                break;
              case 0x01:
                zprintf("Kempston2");
                break;
              case 0x02:
                zprintf("Kempston1");
                break;
              case 0x03:
                zprintf("MegaDrive1");
                break;
              case 0x04:
                zprintf("Cursor");
                break;
              case 0x05:
                zprintf("MegaDrive2");
                break;
              case 0x06:
                zprintf("Sinclair1");
                break;
              case 0x07:
                zprintf("User Defined Keys Joystick");
                break;
            }
            zprintf("\n");
          }
          break;

        case 0x06: /* 06 Peripheral 2 Settings ------------------------------ */
          zprintf(" + " DUMP_REGSUB " = ", "PSGMODECTRL");
          switch (uiValue & 0x03)
          {
            case 0x00:
              zprintf("YM");
              break;
            case 0x01:
              zprintf("AY");
              break;
            case 0x03:
              zprintf("Hold all PSGs in RESET");
              break;
            default:
              zprintf("UNKNOWN: 0x%X", uiValue & 0x03);
          }
          zprintf("\n");
          zprintf(" + " DUMP_REGSUB " = %s primary\n", "PS2MODECTL",  uiValue & (1 << 2) ? "Mouse"     : "Keyboard");
          zprintf(" + " DUMP_REGSUB " = %s\n",         "NMIBTNCTL",   uiValue & (1 << 3) ? "enabled"   : "disabled");
          zprintf(" + " DUMP_REGSUB " = %s\n",         "DVMMCNMICTL", uiValue & (1 << 4) ? "enabled"   : "disabled");
          zprintf(" + " DUMP_REGSUB " = %s\n",         "F3HOTKEYCTL", uiValue & (1 << 5) ? "enabled"   : "disabled");
          zprintf(" + " DUMP_REGSUB " = %s\n",         "INTSPKRCTL",  uiValue & (1 << 6) ? "only BEEP" : "all audio");
          zprintf(" + " DUMP_REGSUB " = %s\n",         "F568HKEYCTL", uiValue & (1 << 7) ? "enabled"   : "disabled");
          break;

        case 0x07: /* 07 CPU Speed ------------------------------------------ */
          for (uint8_t i = 0; i < 2; ++i)
          {
            if (0 == i)
            {
              uiValue16 = uiValue & 0x03;
              zprintf(" + " DUMP_REGSUB " = ", "SETSPEED");
            }
            else
            {
              uiValue16 = (uiValue >> 4) & 0x03;
              zprintf(" + " DUMP_REGSUB " = ", "CURRNTSPEED");
            }

            switch (uiValue16 & 0x03)
            {
              case 0x00:
                zprintf("3.5");
                break;
              case 0x01:
                zprintf("7");
                break;
              case 0x02:
                zprintf("14");
                break;
              case 0x03:
                zprintf("28");
                break;
            }
            zprintf(" MHz\n");
          }
          break;

        case 0x08: /* 08 Peripheral 3 Settings ------------------------------ */
          zprintf(" + " DUMP_REGSUB " = %s\n", "ISSUE2KBD",   uiValue & (1 << 0) ? "on"     : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "NEXTSOUND",   uiValue & (1 << 1) ? "on"     : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "TMXVIDPTCTL", uiValue & (1 << 2) ? "on"     : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "DACSCTRL",    uiValue & (1 << 3) ? "on"     : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "INTSPEAKER",  uiValue & (1 << 4) ? "on"     : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "PSGMODECTL",  uiValue & (1 << 5) ? "ACB"    : "ABC");
          zprintf(" + " DUMP_REGSUB " = %s\n", "CONTENTION",  uiValue & (1 << 6) ? "off"    : "on");
          zprintf(" + " DUMP_REGSUB " = %s\n", "128KBNKUCTL", uiValue & (1 << 7) ? "unlock" : "lock");
          break;

        case 0x0F: /* 15 BOARDID -------------------------------------------- */
          zprintf(" + " DUMP_REGSUB " = ", "REVISION");
          switch (uiValue & 0x0F)
          {
            case 0x00:
              zprintf("ZX Spectrum Next Issue 2");
              break;
            case 0x01:
              zprintf("ZX Spectrum Next Issue 3");
              break;
            case 0x02:
              zprintf("ZX Spectrum Next Issue 4");
              break;
            default:
              zprintf("UNKNOWN: 0x%X", uiValue);
          }
          zprintf("\n");
          break;

        case 0x10: /* 10 Core Boot ------------------------------------------ */
          zprintf(" + " DUMP_REGSUB " = %spressed\n", "NMIBUTTON",   uiValue & (1 << 0) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = %spressed\n", "DRIVEBUTTON", uiValue & (1 << 1) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = %u\n",        "COREID",     (uiValue >> 2) & 0x1F);
          break;
      }
    }

    ++pReg;
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
