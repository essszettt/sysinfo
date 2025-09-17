/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: dumpDos.c                                                          |
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
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>

#include "sysinfo.h"
#include "dumpDos.h"

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
/* dumpOperatingSystem()                                                      */
/*----------------------------------------------------------------------------*/
int dumpOperatingSystem(void)
{
  int iReturn = EOK;

  uint8_t  uiValue8;
  uint16_t uiValue16;
 
  zprintf("--- ESXDOS/NEXTOS --------------\n");

  // DOS-Version (ESXDOS) ...
  /*
  ACHTUNG:  Liefert seit Umstellung DOT->DOTN keine brauchbare Info mehr ?
                                                                  SZ, 14.09.2025
  */
  uiValue16 = esx_m_dosversion();
  zprintf(DUMP_NOSNAME " = %u.%02u\n", "DOSVERSION", ESX_DOSVERSION_NEXTOS_MAJOR(uiValue16),
                                                     ESX_DOSVERSION_NEXTOS_MINOR(uiValue16));
  // Date & Time
  struct dos_tm tRaw;
  if (0 == esx_m_getdate(&tRaw))
  {
    struct tm tNow;
    tm_from_dostm(&tNow, &tRaw);
    zprintf(DUMP_NOSNAME " = %02u/%02u/%04u %02u:%02u:%02u%s\n", "DATETIME",
            1 + tNow.tm_mon, tNow.tm_mday, 1900 + tNow.tm_year,
            tNow.tm_hour, tNow.tm_min, tNow.tm_sec,
            tNow.tm_isdst ? " DST" : "");
  }

  // Screen mode
  struct esx_mode tMode;
  memset(&tMode, sizeof(tMode), 0);

  if (0 == esx_ide_mode_get(&tMode))
  {
    zprintf(DUMP_NOSNAME " = %u:%u\n", "SCREENMODE", tMode.mode8.layer, tMode.mode8.submode);
    zprintf("+ " DUMP_NOSSUB " = %u:%u\n", "COLOR", tMode.ink,   tMode.paper);
    zprintf("+ " DUMP_NOSSUB " = %u:%u\n", "FLAGS", tMode.flags, tMode.width);
    zprintf("+ " DUMP_NOSSUB " = %u:%u\n", "SIZE",  tMode.cols,  tMode.rows);
  }

  // Current working directory ...
  unsigned char* acPathName = 0;
  if (0 != (acPathName = (unsigned char*) malloc(ESX_PATHNAME_MAX)))
  {
    if (0 == esx_f_getcwd(acPathName))
    {
      zprintf(DUMP_NOSNAME " = %s\n", "CURRWORKDIR", acPathName);
    }

    free(acPathName);
    acPathName = 0;
  }

  // Current drive ...
  uint8_t uiRawDrive;
  uiValue8 = esx_m_getdrv();
  uiRawDrive = 0x41 + ((uiValue8 >> 3) & 0x1F);
  zprintf(DUMP_NOSNAME " = 0x%02X\n", "DEFAULTDRIVE", uiValue8);
  zprintf("+ " DUMP_NOSSUB " = %c\n", "LETTER", uiRawDrive);
  zprintf("+ " DUMP_NOSSUB " = %u\n", "INDEX", uiValue8 & 0x07);

  // Disk info (current drive) ...
  struct esxdos_device tDevice;
  memset(&tDevice, sizeof(tDevice), 0);

  if (/* 0 == */ (uiValue16 = esxdos_disk_info(uiRawDrive, &tDevice)))
  {
    // printf("\x10\x02"); // INK 2
    zprintf("+ " DUMP_NOSSUB " = 0x%X\n",  "RESULT", uiValue16); /* TODO */
    zprintf("+ " DUMP_NOSSUB " = 0x%02X\n", "PATH",  tDevice.path);
    zprintf("+ " DUMP_NOSSUB " = 0x%02X\n", "FLAGS", tDevice.flags);
    zprintf("+ " DUMP_NOSSUB " = 0x%lX\n",  "SIZE",  tDevice.size);
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
