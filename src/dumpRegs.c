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
  {0x60, true,  "COPRDTA8WR" },
  {0x61, true,  "COPRADDRLSB"},
  {0x62, true,  "COPPERCTRL" },
  {0x63, true,  "COPRDTA16WR"},
  {0x64, true,  "VRTLNCNTOFS"},
  {0x68, true,  "ULACTRL"    },
  {0x69, true,  "DISPCTRL1"  },
  {0x6A, true,  "LAYER01CTRL"},
  {0x6B, true,  "LAYER3CTRL" },
  {0x6C, true,  "L3DEFATTR"  },
  {0x6E, true,  "L3BASEADDR" },
  {0x6F, true,  "L3TILEBASAD"},
  {0x70, true,  "L2RSLTNCTRL"},
  {0x71, true,  "L2HSCRLCTLM"},
 
  {0x80, true,  "EXPBUSENABL"},
  {0x81, true,  "EXPBUSCTRL" },

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
  uint32_t uiValue32;
  const unsigned char* acValue = 0;

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
          switch (uiValue)
          {
            case 0b00001000: acValue = "Emulator"; break;
            case 0b00001010: acValue = "ZX Spectrum Next"; break;
            case 0b11111010: acValue = "ZX Spectrum Next Anti Brick"; break;
            case 0b10011010: acValue = "NEXT Core on UnAmiga Reloaded"; break;
            case 0b10101010: acValue = "NEXT Core on UnAmiga"; break;
            case 0b10111010: acValue = "NEXT Core on SiDi"; break;
            case 0b11001010: acValue = "NEXT Core on MiST"; break;
            case 0b11011010: acValue = "NEXT Core on MiSTer"; break;
            case 0b11101010: acValue = "NEXT Core on ZX-DOS/gomaDOS"; break;
            default:         acValue = "UNKNOWN";
          }
          zprintf(" + " DUMP_REGSUB " = %s\n", "MACHINEID", acValue);
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
          switch (uiValue & 0x03)
          {
            case 0x01: acValue = "Soft Reset"; break;
            case 0x02: acValue = "Hard Reset"; break;
            default:   acValue = "UNKNOWN";
          }
          zprintf(" + " DUMP_REGSUB " = %s\n", "LSTSYSRSTTY", acValue);
          zprintf(" + " DUMP_REGSUB " = divMMC NMI %sgenerated by NR 0x02\n", "DVMMCNMISRC", uiValue & (1 << 2) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = MF NMI %sgenerated by NR 0x02\n",     "MFNMISOURCE", uiValue & (1 << 3) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = RESET %sasserted\n",                  "EXTBUSRSTFG", uiValue & (1 << 7) ? "" : "not ");
          break;

        case 0x03: /* 03 MACHINETYPE ---------------------------------------- */
          for (uint8_t i = 0; i < 2; ++i)
          {
            uiValue16 = (0 == i ? uiValue & 0x07 : (uiValue >> 4) & 0x07); 

            switch (uiValue16)
            {
              case 0x00: acValue = "Config Mode"; break;
              case 0x01: acValue = "ZX 48K"; break;
              case 0x02: acValue = "ZX 128K/+2"; break;
              case 0x03: acValue = "ZX +2A/+2B/+3/NEXT"; break;
              case 0x04: acValue = "Pentagon Clone"; break;
              default:   acValue = "UNKNOWN";
            }
            zprintf(" + " DUMP_REGSUB " = %s\n", (0 == i ? "MACHINETYPE" : "DISPTIMING"), acValue);
          }
          break;

        case 0x05: /* 05 Peripheral 1 Settings ------------------------------ */
          zprintf(" + " DUMP_REGSUB " = %s\n",    "SCANDOUBLER", uiValue & (1 << 0) ? "on" : "off");
          zprintf(" + " DUMP_REGSUB " = %u Hz\n", "VERTFREQ",    uiValue & (1 << 2) ? 60   : 50);

          for (uint8_t i = 0; i < 2; ++i)
          {
            uiValue16 = (0 == i) ?
                        ((uiValue & (3 << 6)) >> 5) | ((uiValue & (1 << 3)) >> 3) :
                        ((uiValue & (3 << 4)) >> 3) | ((uiValue & (1 << 1)) >> 1);

            switch (uiValue16 & 0x07)
            {
              case 0x00: acValue = "Sinclair2";  break;
              case 0x01: acValue = "Kempston2";  break;
              case 0x02: acValue = "Kempston1";  break;
              case 0x03: acValue = "MegaDrive1"; break;
              case 0x04: acValue = "Cursor";     break;
              case 0x05: acValue = "MegaDrive2"; break;
              case 0x06: acValue = "Sinclair1";  break;
              case 0x07: acValue = "User Defined Keys Joystick"; break;
            }
            zprintf(" + JOYSTICK%u   = %s\n", i + 1, acValue);
          }
          break;

        case 0x06: /* 06 Peripheral 2 Settings ------------------------------ */
          switch (uiValue & 0x03)
          {
            case 0x00: acValue = "YM"; break;
            case 0x01: acValue = "AY"; break;
            case 0x03: acValue = "Hold all PSGs in RESET"; break;
            default:   acValue = "UNKNOWN";
          }
          zprintf(" + " DUMP_REGSUB " = %s\n",         "PSGMODECTRL", acValue);
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
            uiValue16 = (0 == i ? uiValue & 0x03 : (uiValue >> 4) & 0x03);

            switch (uiValue16 & 0x03)
            {
              case 0x00: acValue = "3.5"; break;
              case 0x01: acValue = "7";   break;
              case 0x02: acValue = "14";  break;
              case 0x03: acValue = "28";  break;
            }
            zprintf(" + " DUMP_REGSUB " = %s MHz", (0 == i ? "SETSPEED" : "CURRNTSPEED"), acValue);
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
          switch (uiValue & 0x0F)
          {
            case 0x00: acValue = "ZX Spectrum Next Issue 2"; break;
            case 0x01: acValue = "ZX Spectrum Next Issue 3"; break;
            case 0x02: acValue = "ZX Spectrum Next Issue 4"; break;
            default:   acValue = "UNKNOWN";
          }
          zprintf(" + " DUMP_REGSUB " = %s\n", "REVISION", acValue);
          break;

        case 0x10: /* 10 Core Boot ------------------------------------------ */
          zprintf(" + " DUMP_REGSUB " = %spressed\n", "NMIBUTTON",   uiValue & (1 << 0) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = %spressed\n", "DRIVEBUTTON", uiValue & (1 << 1) ? "" : "not ");
          zprintf(" + " DUMP_REGSUB " = %u\n",        "COREID",     (uiValue >> 2) & 0x1F);
          break;

        case 0x12: /* Layer 2 active RAM bank ------------------------------- */ 
        case 0x13: /* Layer 2 shadow RAM bank ------------------------------- */
          uiValue32 = ((uint32_t) 0x4000) * ((uint32_t) uiValue); // physical address
          zprintf(" + " DUMP_REGSUB " = BANK16:%02X (%06lX-%06lX)\n", "MAPPING", 
                  uiValue, uiValue32, uiValue32 + 0x3FFF);
          break;

        case 0x50: /* 50 MMU slot X control --------------------------------- */
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
          if (0xFF == uiValue)
          {
            uiValue16 = ((uint16_t) 0x2000) * (pReg->uiNumber - 0x50); // logical address 
            zprintf(" + " DUMP_REGSUB " = (%04X-%04X) => ROM\n",
                    "MAPPING", 
                    uiValue16, uiValue16 + 0x1FFF);
          }
          else
          {
            uiValue16 = ((uint16_t) 0x2000) * (pReg->uiNumber - 0x50); // logical address 
            uiValue32 = ((uint32_t) 0x2000) * uiValue;                 // physical address
            zprintf(" + " DUMP_REGSUB " = (%04X-%04X) => BANK8:%02X (%06lX-%06lX)\n",
                    "MAPPING", 
                    uiValue16, uiValue16 + 0x1FFF,
                    uiValue,
                    uiValue32, uiValue32 + 0x1FFF);
          }
          break;

        case 0x62: /* Copper control ---------------------------------------- */
          zprintf(" + " DUMP_REGSUB " = 0x%02X\n", "CPPRADDRMSB", uiValue & 0x07);
          switch ((uiValue >> 6) & 0x03)
          {
            case 0x00: acValue = "Copper fully stopped"; break;
            case 0x01: acValue = "Copper start, exec from 0, loop"; break;
            case 0x02: acValue = "Copper start, exec from last, loop"; break;
            case 0x03: acValue = "Copper start, exec from 0, reset at raster 0:0"; break;
          }
          zprintf(" + " DUMP_REGSUB " = %s\n", "COPPERCTRL", acValue);
          break;

        case 0x68: /* ULA control ------------------------------------------- */
          zprintf(" + " DUMP_REGSUB " = %s\n", "STENCILMODE", uiValue & (1 << 0) ? "on"  : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "ULAHPSCROLL", uiValue & (1 << 2) ? "on"  : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "ULAPLUSCTRL", uiValue & (1 << 3) ? "on"  : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "EXTKEYS",     uiValue & (1 << 4) ? "off" : "on");
          switch ((uiValue >> 5) & 0x03)
          {
            case 0x00: acValue = "ULA colour"; break;
            case 0x01: acValue = "No colour blending"; break;
            case 0x02: acValue = "ULA + tilemax mix"; break;
            case 0x03: acValue = "Tilemap colour"; break;
          }
          zprintf(" + " DUMP_REGSUB " = %s\n", "BLNDCLRUSED", acValue);
          zprintf(" + " DUMP_REGSUB " = %s\n", "OUTPUTENABL", uiValue & (1 << 7) ? "off" : "on");
          break;

        case 0x69: /* Display control 1 ------------------------------------- */
          zprintf(" + " DUMP_REGSUB " = %u\n", "P255ALIAS",   uiValue & 0x1F);
          zprintf(" + " DUMP_REGSUB " = %s\n", "ULASHDWDISP", uiValue & (1 << 6) ? "on" : "off");
          zprintf(" + " DUMP_REGSUB " = %s\n", "LAYER2",      uiValue & (1 << 7) ? "on" : "off");
          break;

        case 0x6A: /* Layer 1,0 control ------------------------------------- */
          zprintf(" + " DUMP_REGSUB " = %u\n",          "PALOFFSET", uiValue & 0x0F);
          zprintf(" + " DUMP_REGSUB " = %s\n",          "RADASMODE", uiValue & (1 << 4) ? "on" : "off");
          zprintf(" + " DUMP_REGSUB " = Radastan %s\n", "LORESMODE", uiValue & (1 << 5) ? "on" : "off");
          break;

        case 0x70: /* Layer 2 resolution control ---------------------------- */
          zprintf(" + " DUMP_REGSUB " = %u\n", "L2PALOFFSET", uiValue & 0x0F);
          switch ((uiValue >> 4) & 0x03)
          {
            case 0x00: acValue = "256 x 192 x 8bpp"; break;
            case 0x01: acValue = "320 x 192 x 8bpp"; break;
            case 0x02: acValue = "640 x 192 x 4bpp"; break;
            case 0x03: acValue = "<o>"; break;
          }
          zprintf(" + " DUMP_REGSUB " = %s\n", "L2RESSELECT", acValue);
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
