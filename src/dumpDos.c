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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>
#include <arch/zxn/sysvar.h>

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
  uint16_t uiOsVersion = 0xFFFF; 
  const char_t* acValue = 0;

  zheader("ESXDOS/NEXTOS");

  // DOS-Version (ESXDOS) ...
  /*
  ACHTUNG:  Liefert seit Umstellung DOT->DOTN keine brauchbare Info mehr ?
                                                                  SZ, 14.09.2025
  */
  if (EOK == iReturn)
  {
    switch (uiOsVersion = esx_m_dosversion())
    {
      case ESX_DOSVERSION_ESXDOS:     acValue = "esxDOS"; break;
      case ESX_DOSVERSION_NEXTOS_48K: acValue = "48K";    break;
      default:                        acValue = "128K/NEXT";
    }

    zprintf(DUMP_NOSNAME " = %u.%02u\n", "DOSVERSION",
            ESX_DOSVERSION_NEXTOS_MAJOR(uiOsVersion),
            ESX_DOSVERSION_NEXTOS_MINOR(uiOsVersion));
    zprintf("+ " DUMP_NOSSUB " = %s\n", "MODE", acValue);
  }

  // Date & Time
  if (EOK == iReturn)
  {
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
  }

  // Free memory
  if (EOK == iReturn)
  {
    zprintf(DUMP_NOSNAME " = %lu\n", "MEMFREE", esx_f_getfree());
  }
  
  // Screen mode
  if (EOK == iReturn)
  {
    struct esx_mode tMode;
    memset(&tMode, 0, sizeof(tMode));

    if (0 == esx_ide_mode_get(&tMode))
    {
      zprintf(DUMP_NOSNAME " = %u:%u\n", "SCREENMODE", tMode.mode8.layer, tMode.mode8.submode);
      zprintf("+ " DUMP_NOSSUB " = %u\n",     "INK|ATTR", tMode.ink);
      zprintf("+ " DUMP_NOSSUB " = %u\n",     "PAPER",    tMode.paper);
      zprintf("+ " DUMP_NOSSUB " = 0x%02X\n", "FLAGS",    tMode.flags);
      zprintf("+ " DUMP_NOSSUB " = %u\n",     "WIDTH",    tMode.width);
      zprintf("+ " DUMP_NOSSUB " = %u\n",     "COLS",     tMode.cols);
      zprintf("+ " DUMP_NOSSUB " = %u\n",     "ROWS",     tMode.rows);
    }
  }

  // Current working directory
  if (EOK == iReturn)
  {
    char_t* acPathName = 0;
    if (0 != (acPathName = (char_t*) malloc(ESX_PATHNAME_MAX)))
    {
      memset(acPathName, 0, ESX_PATHNAME_MAX);

      if (0 == esx_f_getcwd(acPathName))
      {
        zprintf(DUMP_NOSNAME " = %s\n", "CURRENTWORKDIR", acPathName);
      }

      free(acPathName);
      acPathName = 0;
    }
  }

  // Current drive
  if (EOK == iReturn)
  {
    uint8_t uiDrive = esx_m_getdrv();
    char_t  cLetter = 'A' + (uiDrive >> 3);

    zprintf(DUMP_NOSNAME " = 0x%02X\n", "DEFAULTDRIVE", uiDrive);
    zprintf("+ " DUMP_NOSSUB " = %c\n", "LETTER", cLetter);
    zprintf("+ " DUMP_NOSSUB " = %u\n", "INDEX", uiDrive & 0x07);
  }

  // Disk info (current drive)
#if 0
  if (EOK == iReturn)
  {
    uint8_t uiDrive;
    int iResult;
    struct esxdos_device tDevice;
    memset(&tDevice, 0, sizeof(tDevice));

    uiDrive = esx_m_getdrv();

    /*
    esxdos_disk_info returns "-1" ?
    */

    if (0 == (iResult = esxdos_disk_info('A' + (uiDrive >> 3), &tDevice)))
    {
      zprintf("+ " DUMP_NOSSUB " = 0x%02X\n", "PATH",  tDevice.path);
      zprintf("+ " DUMP_NOSSUB " = 0x%02X\n", "FLAGS", tDevice.flags);
      zprintf("+ " DUMP_NOSSUB " = 0x%lX\n",  "SIZE",  tDevice.size);
    }
    else
    {
      fprintf(stderr, "dumpOs() - esxdos_disk_info(0x%02X): %d\n", uiDrive, iResult);
    }
  }
#endif

  // Available drives
  if (EOK == iReturn)
  {
    uint8_t uiResult;
    uint8_t uiDrive;
    uint8_t uiIdx = 0;
    char_t  acDrives[20];

    acDrives[0] = '\0';

    if (ESX_DOSVERSION_NEXTOS_48K == uiOsVersion) /* NEXTZXOS == 48K Mode */
    {
      uint8_t uiEsxDrv;

      uiDrive = esx_m_getdrv();
      for (char_t cDrive = 'A'; cDrive <= 'P'; ++cDrive)
      {
        uiEsxDrv = ((cDrive - 'A') << 3) | 0x01;

        /*
        'C'  => 16
        'M'  => 96
        Rest => 255
        */
        if (0xFF != (uiResult = esx_m_setdrv(uiEsxDrv)))
        {
          acDrives[uiIdx] = cDrive;
          ++uiIdx;
          acDrives[uiIdx] = '\0';
        }
       #if defined(__DEBUG__)
        else
        {
          fprintf(stderr, "dumpOs() - esx_m_setdrv(%c|0x%X) = %u\n", cDrive, uiEsxDrv, uiResult);
        }
       #endif
      }
      esx_m_setdrv(uiDrive);
    }
    else /* NEXTZXOS != 48K Mode */
    {
      uiDrive = esx_dos_get_drive();
      for (char_t cDrive = 'A'; cDrive <= 'P'; ++cDrive)
      {
        if (0 == (uiResult = esx_dos_set_drive(cDrive)))
        {
          acDrives[uiIdx] = cDrive;
          ++uiIdx;
          acDrives[uiIdx] = '\0';
        }
       #if defined(__DEBUG__)
        else
        {
          fprintf(stderr, "dumpOs() - esx_dos_set_drive(%c) = %u\n", cDrive, uiResult);
        }
       #endif
      }
      esx_dos_set_drive(uiDrive);
      SYSVAR_LODDRV = uiDrive;
      SYSVAR_SAVDRV = uiDrive;
    }

    zprintf(DUMP_NOSNAME " = %s\n", "AVAIL.DRIVES", acDrives);
  }

#if 0
  /* !!! CRASHING !!! */

  if (EOK == iReturn)
  {
    uint8_t uiIdx = 0;
    char_t  acDrive[4]; /* "C:/" */
    char_t  acDrives[20];
    uint8_t hDir = INV_FILE_HND;

    acDrives[0] = '\0';
    strcpy(acDrive, "?:/");

    for (char_t cDrive = 'A'; cDrive <= 'C'; ++cDrive)
    {
      acDrive[0] = cDrive;

      DBGPRINTF("dumpDos() - drive = %s\n", acDrive);

      if (INV_FILE_HND != (hDir = esx_f_opendir(acDrive)))
      {
        acDrives[uiIdx] = cDrive;
        ++uiIdx;
        acDrives[uiIdx] = '\0';

        DBGPRINTF("dumpDos() - hDir = %u\n", hDir);

        (void) esx_f_closedir(hDir);
      }
      else
      {
        fprintf(stderr, "dumpDos() - esx_f_opendir(%s) = %d\n", acDrive, hDir);
      }

      DBGPRINTF("dumpDos() - drives = %s\n", acDrives);
    }

    zprintf(DUMP_NOSNAME " = %s\n", "DRIVES", acDrives);
  }
#endif

#if 0
  /* !!! CRASHING !!! */

  if (EOK == iReturn)
  {
    uint8_t uiIdx = 0;
    uint8_t uiDrive;
    char_t  acDrives[20];
    char_t  acPathName = 0;

    if (0 != (acPathName = (char_t*) malloc(ESX_PATHNAME_MAX)))
    {
      acDrives[0] = '\0';

      uiDrive = esx_m_getdrv();
      for (char_t cDrive = 'A'; cDrive <= 'P'; ++cDrive)
      {
        if (0 == esx_f_getcwd_drive(cDrive, acPathName))
        {
          acDrives[uiIdx] = cDrive;
          ++uiIdx;
          acDrives[uiIdx] = '\0';
        }
      }
      esx_m_setdrv(uiDrive);

      zprintf(DUMP_NOSNAME " = %s\n", "DRIVES", acDrives);

      free(acPathName);
      acPathName = 0;
    }
  }
#endif

  // Environment variables
  if (EOK == iReturn)
  {
    zprintf(DUMP_NOSNAME " = \"%s\"\n", "ENV.PATH", getenv("path"));
    zprintf(DUMP_NOSNAME " = \"%s\"\n", "ENV.TMP",  getenv("tmp"));
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
