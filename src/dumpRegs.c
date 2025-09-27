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
  {0x00, 0x00, "MACHINEID"  },
  {0x01, 0x00, "COREVERSION"},
  {0x02, 0x00, "RESET"      },
  {0x03, 0x00, "MACHINETYPE"},
  {0x04, 0x00, "CONFMAPPING"},
  {0x05, 0x00, "PERIPHERAL1"},
  {0x06, 0x00, "PERIPHERAL2"},
  {0x07, 0x00, "CPUSPEED"   },
  {0x08, 0x00, "PERIPHERAL3"},
  {0x09, 0x00, "PERIPHERAL4"},
  {0x0A, 0x00, "PERIPHERAL5"},
  {0x0B, 0x00, "JOYPORTMODE"},
  {0x0E, 0x00, "COREVERSUB" },
  {0x0F, 0x00, "BOARDID"    },
  {0x10, 0x00, "COREBOOT"   },
  {0x11, 0x00, "VIDEOTIMING"},
  {0x12, 0x00, "L2ACTRAMBNK"},
  {0x13, 0x00, "L2SHARAMBNK"},
  {0x14, 0x00, "GLBTRNSCLR" },
  {0x15, 0x00, "SPRLYSYSSTP"},
  {0x16, 0x00, "L2HSCRLCTRL"},
  {0x17, 0x00, "L2VSCRLCTRL"},
  {0x18, 0x00, "L2CLPWINDEF"},
  {0x19, 0x00, "SPCLPWINDEF"},
  {0x1A, 0x00, "L0CLPWINDEF"},
  {0x1B, 0x00, "L3CLPWINDEF"},
  {0x1C, 0x00, "CLPWINCTRL" },
  {0x1E, 0x00, "ACTVIDLNMSB"},
  {0x1F, 0x00, "ACTVIDLNLSB"},
  {0x20, 0x00, "MASKINTGNRT"},
  {0x22, 0x00, "LINEINTCTRL"},
  {0x26, 0x00, "ULAHSCRCTRL"},
  {0x27, 0x00, "ULAVSCRCTRL"},
  {0x28, 0x00, "PS2KYMPADMB"},
  {0x29, 0x00, "PS2KYMPADLB"},
  {0x2A, 0x00, "PS2KYMPDTMB"},
  {0x2B, 0x00, "PS2KYMPDTLB"},
  {0x2C, 0x00, "DACBMIRROR" },
  {0x2D, 0x00, "DACADMIRROR"},
  {0x2E, 0x00, "DACCMIRROR" },
  {0x2F, 0x00, "L3HSCRCTRLM"},
  {0x30, 0x00, "L3HSCRCTRLL"},
  {0x31, 0x00, "L3VSCRCTRL" },
  {0x32, 0x00, "L10HSCRCTRL"},
  {0x33, 0x00, "L10VSCRCTRL"},
  {0x34, 0x00, "SPRITENUMBR"},
  {0x35, 0x00, "SPRITEARR0" },
  {0x36, 0x00, "SPRITEARR1" },
  {0x37, 0x00, "SPRITEARR2" },
  {0x38, 0x00, "SPRITEARR3" },
  {0x39, 0x00, "SPRITEARR4" },
  {0x40, 0x00, "PALINDEXSEL"},
  {0x41, 0x00, "8BITPALDATA"},
  {0x42, 0x00, "EULAATTRFMT"},
  {0x43, 0x00, "PALETTECTRL"},
  {0x44, 0x00, "9BITPALDATA"},
  {0x4A, 0x00, "FBACKCOLVAL"},
  {0x4B, 0x00, "SPRTRNSPIDX"},
  {0x4C, 0x00, "L3TRNSPIDX" },
  {0x50, 0x00, "MMUSLT0CTRL"},
  {0x51, 0x00, "MMUSLT1CTRL"},
  {0x52, 0x00, "MMUSLT2CTRL"},
  {0x53, 0x00, "MMUSLT3CTRL"},
  {0x54, 0x00, "MMUSLT4CTRL"},
  {0x55, 0x00, "MMUSLT5CTRL"},
  {0x56, 0x00, "MMUSLT6CTRL"},
  {0x57, 0x00, "MMUSLT7CTRL"},
  {0x60, 0x00, "COPRDTA8WR" },
  {0x61, 0x00, "COPRADDRLSB"},
  {0x62, 0x00, "COPPERCTRL" },
  {0x63, 0x00, "COPRDTA16WR"},
  {0x64, 0x00, "VRTLNCNTOFS"},
  {0x68, 0x00, "ULACTRL"    },
  {0x69, 0x00, "DISPCTRL1"  },
  {0x6A, 0x00, "LAYER01CTRL"},
  {0x6B, 0x00, "LAYER3CTRL" },
  {0x6C, 0x00, "L3DEFATTR"  },
  {0x6E, 0x00, "L3BASEADDR" },
  {0x6F, 0x00, "L3TILEBASAD"},
  {0x70, 0x00, "L2RSLTNCTRL"},
  {0x71, 0x00, "L2HSCRLCTLM"},
  {0x75, 0x00, "SPRITEATTR0"},
  {0x76, 0x00, "SPRITEATTR1"},
  {0x77, 0x00, "SPRITEATTR2"},
  {0x78, 0x00, "SPRITEATTR3"},
  {0x79, 0x00, "SPRITEATTR4"},
  {0x7F, 0x00, "USERREG0"   },
  {0x80, 0x00, "EXPBUSENABL"},
  {0x81, 0x00, "EXPBUSCTRL" },
  {0x82, 0x00, "INTPRTCTL14"},
  {0x83, 0x00, "INTPRTCTL24"},
  {0x84, 0x00, "INTPRTCTL34"},
  {0x85, 0x00, "INTPRTCTL44"},
  {0x86, 0x00, "EXPPRTCTL14"},
  {0x87, 0x00, "EXPPRTCTL24"},
  {0x88, 0x00, "EXPPRTCTL34"},
  {0x89, 0x00, "EXPPRTCTL44"},
  {0x8A, 0x00, "EXPIOPRPCTL"},
  {0x8C, 0x00, "ALTROMCTRL" },
  {0x8E, 0x00, "128KMEMMAP" },
  {0x8F, 0x00, "MEMMAPMDCTL"},
  {0x90, 0x00, "PIGPIOOUT14"},
  {0x91, 0x00, "PIGPIOOUT24"},
  {0x92, 0x00, "PIGPIOOUT34"},
  {0x93, 0x00, "PIGPIOOUT44"},
  {0x98, 0x00, "PIGPIOIN14" },
  {0x99, 0x00, "PIGPIOIN24" },
  {0x9A, 0x00, "PIGPIOIN34" },
  {0x9B, 0x00, "PIGPIOIN44" },
  {0xA0, 0x00, "PIPERIPENBL"},
  {0xA2, 0x00, "PII2SAUDCTL"},
  {0xA8, 0x00, "ESPWFGPOCTL"},
  {0xA9, 0x00, "ESPWFGPCTL" },
  {0xB0, 0x00, "EXTENDKEYS0"},
  {0xB1, 0x00, "EXTENDKEYS1"},
  {0xB2, 0x00, "EXTMDPADBTN"},
  {0xB8, 0x00, "DIVMMCEP0"  },
  {0xB9, 0x00, "DIVMMCEPVAL"},
  {0xBA, 0x00, "DIVMMCEPTC0"},
  {0xBB, 0x00, "DIVMMCEP1"  },
  {0xC0, 0x00, "IRQCTRL"    },
  {0xC2, 0x00, "NMIRETADRLS"},
  {0xC3, 0x00, "NMIRETADRMS"},
  {0xC4, 0x00, "IRQENABLES0"},
  {0xC5, 0x00, "IRQENABLES1"},
  {0xC6, 0x00, "IRQENABLES2"},
  {0xC8, 0x00, "IRQSTATUS0" },
  {0xC9, 0x00, "IRQSTATUS1" },
  {0xCA, 0x00, "IRQSTATUS2" },
  {0xCB, 0x00, "RESERVED"   },
  {0xCC, 0x00, "DMAIRQENAB0"},
  {0xCD, 0x00, "DMAIRQENAB1"},
  {0xCE, 0x00, "DMAIRQENAB2"},
  {0xCF, 0x00, "RESERVED"   },
  {0xD8, 0x00, "IOTRAPS"    },
  {0xD9, 0x00, "IOTRAPSWR"  },
  {0xDA, 0x00, "IOTRAPCAUSE"},
  {0xF0, 0x00, "XDEV"       },
  {0xF8, 0x00, "XADC"       },
  {0xF9, 0x00, "XADCD0"     },
  {0xFA, 0x00, "XADCD1"     },
  {0xFF, 0x00, "RESERVED"   },
  /* ---------------------- */
  {0x00, 0x00, NULL         }
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

  zheader("NEXT REGISTERS");

  const regentry_t* pReg = &g_tRegisters[0];
  while (0 != pReg->acName)
  {
    uiValue = (0x07 != pReg->uiNumber ? ZXN_READ_REG(pReg->uiNumber) : _cpuspeed());  /* UGLY HACK ! */

    zprintf(DUMP_REGNUM "-" DUMP_REGNAME " = 0x%02X\n", pReg->uiNumber, pReg->acName, uiValue);

    switch (pReg->uiNumber)
    {
      case 0x00: /* 00 MACHINEID -------------------------------------------- */
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

      case 0x01: /* 01 COREVERSION ------------------------------------------ */
        uiValue16  = uiValue << 8;
        uiValue16 |= ZXN_READ_REG(REG_SUB_VERSION);
        zprintf(" + " DUMP_REGSUB " = %u.%02u.%02u\n", "VERSION",
                                                        (uiValue16 >> 12) & 0x0F,
                                                        (uiValue16 >>  8) & 0x0F,
                                                        uiValue16        & 0xFF);
        break;

      case 0x02: /* 02 RESET ------------------------------------------------ */
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

      case 0x03: /* 03 MACHINETYPE ------------------------------------------ */
        for (uint8_t i = 0; i < 2; ++i)
        {
          uiValue16 = (0 == i ? uiValue : uiValue >> 4) & 0x07; 

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

      case 0x05: /* 05 Peripheral 1 Settings -------------------------------- */
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
            default:   acValue = "User Defined Keys Joystick";
          }
          zprintf(" + JOYSTICK%u   = %s\n", i + 1, acValue);
        }
        break;

      case 0x06: /* 06 Peripheral 2 Settings -------------------------------- */
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

      case 0x07: /* 07 CPU Speed -------------------------------------------- */
        for (uint8_t i = 0; i < 2; ++i)
        {
          uiValue16 = (0 == i ? uiValue : uiValue >> 4) & 0x03;

          switch (uiValue16 & 0x03)
          {
            case 0x00: acValue = "3.5"; break;
            case 0x01: acValue = "7";   break;
            case 0x02: acValue = "14";  break;
            default:   acValue = "28";
          }
          zprintf(" + " DUMP_REGSUB " = %s MHz\n", (0 == i ? "SETSPEED" : "CURRNTSPEED"), acValue);
        }
        break;

      case 0x08: /* 08 Peripheral 3 Settings -------------------------------- */
        zprintf(" + " DUMP_REGSUB " = %s\n", "ISSUE2KBD",   uiValue & (1 << 0) ? "on"     : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "NEXTSOUND",   uiValue & (1 << 1) ? "on"     : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "TMXVIDPTCTL", uiValue & (1 << 2) ? "on"     : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "DACSCTRL",    uiValue & (1 << 3) ? "on"     : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "INTSPEAKER",  uiValue & (1 << 4) ? "on"     : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "PSGMODECTL",  uiValue & (1 << 5) ? "ACB"    : "ABC");
        zprintf(" + " DUMP_REGSUB " = %s\n", "CONTENTION",  uiValue & (1 << 6) ? "off"    : "on");
        zprintf(" + " DUMP_REGSUB " = %s\n", "128KBNKUCTL", uiValue & (1 << 7) ? "unlock" : "lock");
        break;

      case 0x09: /* 09 Peripheral 4 Settings -------------------------------- */
        switch (uiValue & 0x03)
        {
          case 0x00: acValue = "off"; break;
          case 0x01: acValue = "50%%"; break;
          case 0x02: acValue = "25%%"; break;
          default:   acValue = "12.5%%";
        }
        zprintf(" + " DUMP_REGSUB " = scanlines %s\n", "SCANLINESTR", acValue);
        zprintf(" + " DUMP_REGSUB " = %s\n", "HDMIAUDCTRL", uiValue & (1 << 2) ? "mute" : "unmute");
        zprintf(" + " DUMP_REGSUB " = %s\n", "DIVMMCBCTRL", uiValue & (1 << 3) ? "reset bit6" : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "SPLCKSTPCTL", uiValue & (1 << 4) ? "on"   : "off"   );
        zprintf(" + " DUMP_REGSUB " = %s\n", "PSG0MONOCTL", uiValue & (1 << 5) ? "mono" : "stereo");
        zprintf(" + " DUMP_REGSUB " = %s\n", "PSG1MONOCTL", uiValue & (1 << 6) ? "mono" : "stereo");
        zprintf(" + " DUMP_REGSUB " = %s\n", "PSG2MONOCTL", uiValue & (1 << 7) ? "mono" : "stereo");
        break;

      case 0x0A: /* 0A Peripheral 5 Settings -------------------------------- */
        switch (uiValue & 0x03)
        {
          case 0x00: acValue = "low DPI"; break;
          case 0x01: acValue = "default"; break;
          case 0x02: acValue = "medium DPI"; break;
          default:   acValue = "high DPI";
        }
        zprintf(" + " DUMP_REGSUB " = %s\n", "MOUSERESCTL", acValue);
        zprintf(" + " DUMP_REGSUB " = %s\n", "MBTNSWAPCTL", uiValue & (1 << 3) ? "swapped" : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "DIVMMCMPCTL", uiValue & (1 << 4) ? "automap" : "off");
        switch ((uiValue >> 6) & 0x03)
        {
          case 0x00: acValue = "+3"; break;
          case 0x01: acValue = "128 v87.2"; break;
          case 0x02: acValue = "128 v87.12"; break;
          default:   acValue = "1";
        }
        zprintf(" + " DUMP_REGSUB " = Multiface %s\n", "MULTIFACETP", acValue);
        break;

      case 0x0B: /* 0B Joystick Port Mode Selection ------------------------- */
        switch ((uiValue >> 4) & 0x03)
        {
          case 0x00:
            acValue = "bit bang";
            break;
          case 0x01:
            zprintf(" + " DUMP_REGSUB " = %s\n", "PARAMCLOCK", uiValue & 0x01 ? "run" : "hold high when clock becomes high");
            acValue = "clock";
            break;
          case 0x02:
            zprintf(" + " DUMP_REGSUB " = redirect %s\n", "PARAMUART", uiValue & 0x01 ? "PI UART1" : "ESP UART0");
            acValue = "UART left joystick port";
            break;
          default:
            zprintf(" + " DUMP_REGSUB " = redirect %s\n", "PARAMUART", uiValue & 0x01 ? "PI UART1" : "ESP UART0");
            acValue = "UART right joystick port";
        }
        zprintf(" + " DUMP_REGSUB " = %s\n", "IOMODECTRL", acValue);
        zprintf(" + " DUMP_REGSUB " = %s enabled\n", "JOYPRTMDSEL", uiValue & (1 << 7) ? "I/O mode" : "joysticks");
        break;

      case 0x0F: /* 15 BOARDID ---------------------------------------------- */
        switch (uiValue & 0x0F)
        {
          case 0x00: acValue = "ZX Spectrum Next Issue 2"; break;
          case 0x01: acValue = "ZX Spectrum Next Issue 3"; break;
          case 0x02: acValue = "ZX Spectrum Next Issue 4"; break;
          default:   acValue = "UNKNOWN";
        }
        zprintf(" + " DUMP_REGSUB " = %s\n", "REVISION", acValue);
        break;

      case 0x10: /* 10 Core Boot -------------------------------------------- */
        zprintf(" + " DUMP_REGSUB " = %spressed\n", "NMIBUTTON",   uiValue & (1 << 0) ? "" : "not ");
        zprintf(" + " DUMP_REGSUB " = %spressed\n", "DRIVEBUTTON", uiValue & (1 << 1) ? "" : "not ");
        zprintf(" + " DUMP_REGSUB " = %u\n",        "COREID",     (uiValue >> 2) & 0x1F);
        break;

      case 0x11: /* 11 Video Timing ----------------------------------------- */
        switch (uiValue & 0x07)
        {
          case 0x00: acValue = "VGA base timing, clk=28000000"; break;
          case 0x01: acValue = "VGA setting 1, clk=28571429"; break;
          case 0x02: acValue = "VGA setting 2, clk=29464286"; break;
          case 0x03: acValue = "VGA setting 3, clk=30000000"; break;
          case 0x04: acValue = "VGA setting 4, clk=31000000"; break;
          case 0x05: acValue = "VGA setting 5, clk=32000000"; break;
          case 0x06: acValue = "VGA setting 6, clk=33000000"; break;
          default:   acValue = "Digital, clk=27000000";
        }
        zprintf(" + " DUMP_REGSUB " = %s\n", "VGATIMING", acValue);
        break;

      case 0x12: /* 18 Layer 2 active RAM bank ------------------------------ */ 
      case 0x13: /* 19 Layer 2 shadow RAM bank ------------------------------ */
        uiValue32 = UINT32_C(0x4000) * ((uint32_t) uiValue & 0x7F); // physical address
        zprintf(" + " DUMP_REGSUB " = BANK16:%02X (%06lX)\n", "START", 
                uiValue, uiValue32);
        break;

      case 0x50: /* 50 MMU slot X control ----------------------------------- */
      case 0x51:
      case 0x52:
      case 0x53:
      case 0x54:
      case 0x55:
      case 0x56:
      case 0x57:
        uiValue16 = UINT16_C(0x2000) * (pReg->uiNumber - 0x50); // logical address 
        if (0xFF == uiValue)
        {
          zprintf(" + " DUMP_REGSUB " = (%04X-%04X) => ROM\n",
                  "MAPPING", 
                  uiValue16, uiValue16 + 0x1FFF);
        }
        else
        {
          uiValue32 = UINT32_C(0x2000) * uiValue;               // physical address
          zprintf(" + " DUMP_REGSUB " = (%04X-%04X) => BANK8:%02X (%06lX-%06lX)\n",
                  "MAPPING", 
                  uiValue16, uiValue16 + 0x1FFF,
                  uiValue,
                  uiValue32, uiValue32 + 0x1FFF);
        }
        break;

      case 0x62: /* 98 Copper control --------------------------------------- */
        zprintf(" + " DUMP_REGSUB " = 0x%02X\n", "CPPRADDRMSB", uiValue & 0x07);
        switch ((uiValue >> 6) & 0x03)
        {
          case 0x00: acValue = "Copper fully stopped"; break;
          case 0x01: acValue = "Copper start, exec from 0, loop"; break;
          case 0x02: acValue = "Copper start, exec from last, loop"; break;
          default:   acValue = "Copper start, exec from 0, reset at raster 0:0";
        }
        zprintf(" + " DUMP_REGSUB " = %s\n", "COPPERCTRL", acValue);
        break;

      case 0x68: /* 104 ULA control ----------------------------------------- */
        zprintf(" + " DUMP_REGSUB " = %s\n", "STENCILMODE", uiValue & (1 << 0) ? "on"  : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "ULAHPSCROLL", uiValue & (1 << 2) ? "on"  : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "ULAPLUSCTRL", uiValue & (1 << 3) ? "on"  : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "EXTKEYS",     uiValue & (1 << 4) ? "off" : "on");
        switch ((uiValue >> 5) & 0x03)
        {
          case 0x00: acValue = "ULA colour"; break;
          case 0x01: acValue = "No colour blending"; break;
          case 0x02: acValue = "ULA + tilemax mix"; break;
          default:   acValue = "Tilemap colour";
        }
        zprintf(" + " DUMP_REGSUB " = %s\n", "BLNDCLRUSED", acValue);
        zprintf(" + " DUMP_REGSUB " = %s\n", "OUTPUTENABL", uiValue & (1 << 7) ? "off" : "on");
        break;

      case 0x69: /* 105 Display control 1 ----------------------------------- */
        zprintf(" + " DUMP_REGSUB " = %u\n", "P255ALIAS",   uiValue & 0x1F);
        zprintf(" + " DUMP_REGSUB " = %s\n", "ULASHDWDISP", uiValue & (1 << 6) ? "on" : "off");
        zprintf(" + " DUMP_REGSUB " = %s\n", "LAYER2",      uiValue & (1 << 7) ? "on" : "off");
        break;

      case 0x6A: /* 106 Layer 1,0 control ----------------------------------- */
        zprintf(" + " DUMP_REGSUB " = %u\n",          "PALOFFSET", uiValue & 0x0F);
        zprintf(" + " DUMP_REGSUB " = %s\n",          "RADASMODE", uiValue & (1 << 4) ? "on" : "off");
        zprintf(" + " DUMP_REGSUB " = Radastan %s\n", "LORESMODE", uiValue & (1 << 5) ? "on" : "off");
        break;

      case 0x70: /* 112 Layer 2 resolution control -------------------------- */
        zprintf(" + " DUMP_REGSUB " = %u\n", "L2PALOFFSET", uiValue & 0x0F);
        switch ((uiValue >> 4) & 0x03)
        {
          case 0x00: acValue = "256 x 192 x 8bpp"; break;
          case 0x01: acValue = "320 x 192 x 8bpp"; break;
          case 0x02: acValue = "640 x 192 x 4bpp"; break;
          default:   acValue = "<o>";
        }
        zprintf(" + " DUMP_REGSUB " = %s\n", "L2RESSELECT", acValue);
        break;

      case 0xCA: /* 202 Interrupt Status 2 ---------------------------------- */
        zprintf(" + " DUMP_REGSUB " = %s\n", "UART0RXAVIL", uiValue & (1 << 0) ? "true" : "false");
        zprintf(" + " DUMP_REGSUB " = %s\n", "UART0RXNFUL", uiValue & (1 << 1) ? "true" : "false");
        zprintf(" + " DUMP_REGSUB " = %s\n", "UART0TXEMPT", uiValue & (1 << 2) ? "true" : "false");
        zprintf(" + " DUMP_REGSUB " = %s\n", "UART1RXAVIL", uiValue & (1 << 4) ? "true" : "false");
        zprintf(" + " DUMP_REGSUB " = %s\n", "UART1RXNFUL", uiValue & (1 << 5) ? "true" : "false");
        zprintf(" + " DUMP_REGSUB " = %s\n", "UART1TXEMPT", uiValue & (1 << 6) ? "true" : "false");
        break;

      default:
        break;
    }

    ++pReg;
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
