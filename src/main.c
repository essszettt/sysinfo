/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: main.c                                                             |
| project:  ZX Spectrum Next - SYSINFO                                         |
| author:   Stefan Zell                                                        |
| date:     09/11/2025                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| description:                                                                 |
|                                                                              |
| Read and show system information of the ZX Spectrum Next (dot command)       |
|                                                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| Copyright (c) 09/11/2025 STZ Engineering                                     |
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
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>

#include "sysinfo.h"
#include "dumpDos.h"
#include "dumpRegs.h"
#include "dumpVars.h"
#include "version.h"

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
In dieser Struktur werden alle globalen Daten der Anwendung gespeichert.
*/
static struct _state
{
  /*!
  If this flag is set, then this structure is initialized
  */
  bool bInitialized;

  /*!
  Action to execute (help, version, sreenshot, ...)
  */
  action_t eAction;

  /*!
  If this flag is set, existing output files are overwritten
  */
  bool bForce;

  /*!
  If this flag is set, no messages are printed to stdout/stderr
  */
  bool bQuiet;

  /*!
  Bitmasked list of features to show
  */
  uint8_t uiFeatures;

  /*!
  Backup: Current speed of Z80
  */
  uint8_t uiCpuSpeed;

  /*!
  Properties of the current screen mode
  */
  struct esx_mode tScreen;

  /*!
  Exitcode of the application, that is handovered to BASIC
  */
  int iExitCode;

  /*!
  All information about the dump file 
  */
  struct _dump
  {
    /*!
    Buffer to render text output (for screen and file)
    */
    char_t acBuffer[LINE_LEN_MAX];

    /*!
    PathName of the dump file.
    */
    char_t acPathName[ESX_PATHNAME_MAX];

    /*!
    Handle of the file that's used to save the output
    */
    uint8_t hFile;
  } dump;

} g_tState;

/*============================================================================*/
/*                               Strukturen                                   */
/*============================================================================*/

/*============================================================================*/
/*                               Typ-Definitionen                             */
/*============================================================================*/

/*============================================================================*/
/*                               Prototypen                                   */
/*============================================================================*/

/*!
Diese Funktion wird einmalig beim Start der Anwendung zur Initialisierung aller
benoetigten Ressourcen aufgerufen.
*/
void _construct(void);

/*!
Diese Funktion wird einmalig beim Beenden der Anwendung zur Freigabe aller
belegten Ressourcen aufgerufen.
*/
void _destruct(void);

/*!
Diese Funktion interpretiert alle Argumente, die der Anwendung uebergeben
wurden.
*/
int parseArguments(int argc, char_t* argv[]);

/*!
Ausgabe der Hilfe dieser Anwendung.
*/
int showHelp(void);

/*!
Ausgabe der Versionsinformation dieser Anwendung.
*/
int showInfo(void);

/*!
Read, detect and show system information
*/
int dumpSystemInfo(void);

/*============================================================================*/
/*                               Klassen                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Implementierung                              */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* _construct()                                                               */
/*----------------------------------------------------------------------------*/
void _construct(void)
{
  g_tState.eAction       = ACTION_NONE;
  g_tState.bForce        = false;
  g_tState.bQuiet        = false;
  g_tState.uiFeatures    = 0xFF;
  g_tState.uiCpuSpeed    = zxn_getspeed();
  g_tState.dump.acPathName[0] = '\0';
  g_tState.dump.hFile    = INV_FILE_HND;
  g_tState.iExitCode     = EOK;

  zxn_setspeed(RTM_28MHZ);

  if (0 != esx_ide_mode_get(&g_tState.tScreen))
  {
    g_tState.tScreen.cols = 32;
    g_tState.tScreen.rows = 24;
  }

  g_tState.bInitialized = true;
}


/*----------------------------------------------------------------------------*/
/* _destruct()                                                                */
/*----------------------------------------------------------------------------*/
void _destruct(void)
{
  if (g_tState.bInitialized)
  {
    zxn_setspeed(g_tState.uiCpuSpeed);
    g_tState.bInitialized = false;
  }
}


/*----------------------------------------------------------------------------*/
/* main()                                                                     */
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  _construct();
  atexit(_destruct);

  if (EOK == (g_tState.iExitCode = parseArguments(argc, argv)))
  {
    switch (g_tState.eAction)
    {
      case ACTION_NONE:
        g_tState.iExitCode = EOK;
        break;

      case ACTION_INFO:
        g_tState.iExitCode = showInfo();
        break;

      case ACTION_HELP:
        g_tState.iExitCode = showHelp();
        break;

      case ACTION_DUMP:
        g_tState.iExitCode = dumpSystemInfo();
        break;
    }
  }

  return (int) (EOK == g_tState.iExitCode ? 0 : zxn_strerror(g_tState.iExitCode));
}


/*----------------------------------------------------------------------------*/
/* parseArguments()                                                           */
/*----------------------------------------------------------------------------*/
int parseArguments(int argc, char_t* argv[])
{
  int iReturn = EOK;

  g_tState.eAction = ACTION_NONE;

  int i = 1;

  while (i < argc)
  {
    const char_t* acArg = argv[i];

    if ('-' == acArg[0])
    {
      if ((0 == stricmp(acArg, "-h")) || (0 == stricmp(acArg, "--help")))
      {
        g_tState.eAction = ACTION_HELP;
      }
      else if ((0 == strcmp(acArg, "-v")) || (0 == stricmp(acArg, "--version")))
      {
        g_tState.eAction = ACTION_INFO;
      }
      else if ((0 == strcmp(acArg, "-f")) || (0 == stricmp(acArg, "--force")))
      {
        g_tState.bForce = true;
      }
      else if ((0 == strcmp(acArg, "-q")) || (0 == stricmp(acArg, "--quiet")))
      {
        g_tState.bQuiet = true;
      }
      else if ((0 == stricmp(acArg, "-t")) || (0 == stricmp(acArg, "--topic")))
      {
        if ((i + 1) < argc)
        {
          const char_t* acNextArg = argv[i + 1];
          const uint8_t uiArgLen = strlen(acNextArg);
          uint8_t uiFeatures = 0;

          DBGPRINTF("parseArgs() - narg=%s; nlen=%u\n", acNextArg, uiArgLen);

          for (uint8_t j = 0; j < uiArgLen; ++j)
          {
            register const char_t cTopic = tolower(acNextArg[j]);

            DBGPRINTF("parseArgs() - idx=%u; opt=%c:%c\n", j, acNextArg[j], cTopic);

            switch (cTopic)
            {
              case 'r': uiFeatures |= ((uint8_t) FEATURE_REGISTERS); break;
              case 'o': uiFeatures |= ((uint8_t) FEATURE_NEXTOS   ); break;
              case 'v': uiFeatures |= ((uint8_t) FEATURE_SYSVARS  ); break;

              default:
                fprintf(stderr, "unknown topic: \"%c\"\n", cTopic);
            }
          }

          /*
          REMARK: SDCC seems to remove to assignment of "uiFeatures" to
                  "g_tState.uiFeatures" if I do it with the if-statement ?
                  If I do it with the trigraph everything works as expected ...
                  If I activte the DBGPRINTFs, it works also ;-) ...
                                                              S.Zell, 10/25/2025
          */
         #if 1
          g_tState.uiFeatures = (uiFeatures ? uiFeatures : g_tState.uiFeatures);
         #else
          if (0 != uiFeatures)
          {
            g_tState.uiFeatures = uiFeatures;
          }
         #endif
          DBGPRINTF("parseArgs() - features = 0x%02X:0x%02X\n", uiFeatures, g_tState.uiFeatures);

          ++i;
        }
      }
      else
      {
        fprintf(stderr, "unknown option: %s\n", acArg);
        iReturn = EINVAL;
        break;
      }
    }
    else
    {
      snprintf(g_tState.dump.acPathName, sizeof(g_tState.dump.acPathName), "%s", acArg);
      zxn_normalizepath(g_tState.dump.acPathName);
    }

    ++i;
  }

  if (ACTION_NONE == g_tState.eAction)
  {
    g_tState.eAction = ACTION_DUMP;
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/* dumpSystemInfo()                                                           */
/*----------------------------------------------------------------------------*/
int dumpSystemInfo(void)
{
  int iReturn = EOK;

  if ('\0' != g_tState.dump.acPathName[0])
  {
    if (EOK == iReturn)
    {
      /* Is argument a directory ? */
      if (INV_FILE_HND != (g_tState.dump.hFile = esx_f_opendir(g_tState.dump.acPathName)))
      {
        uint16_t uiIdx = 0;
        char_t acPathName[ESX_PATHNAME_MAX];

        esx_f_closedir(g_tState.dump.hFile);
        g_tState.dump.hFile = INV_FILE_HND;

        while (uiIdx < 0xFFFF)
        {
          snprintf(acPathName, sizeof(acPathName),
                  "%s" ESX_DIR_SEP APP_INTERNALNAME_STR "-%u.txt",
                  g_tState.dump.acPathName,
                  uiIdx);

          if (INV_FILE_HND == (g_tState.dump.hFile = esx_f_open(acPathName, ESXDOS_MODE_R | ESXDOS_MODE_OE)))
          {
            snprintf(g_tState.dump.acPathName, sizeof(g_tState.dump.acPathName), "%s", acPathName);
            break;  /* filename found */
          }
          else
          {
            esx_f_close(g_tState.dump.hFile);
            g_tState.dump.hFile = INV_FILE_HND;
          }

          ++uiIdx;
        }

        if (0xFFFF == uiIdx)
        {
          iReturn = ERANGE; /* Error */
        }
      }
      else /* Argument is a file ... */
      {
        g_tState.dump.hFile = esx_f_open(g_tState.dump.acPathName, ESXDOS_MODE_R | ESXDOS_MODE_OE);

        if (INV_FILE_HND != g_tState.dump.hFile)
        {
          esx_f_close(g_tState.dump.hFile);
          g_tState.dump.hFile = INV_FILE_HND;

          if (g_tState.bForce)
          {
            esx_f_unlink(g_tState.dump.acPathName);
          }
          else
          {
            iReturn = EBADF; /* Error: File exists */
          }
        }
      }
    }

    if (EOK == iReturn)
    {
      g_tState.dump.hFile = esx_f_open(g_tState.dump.acPathName, ESXDOS_MODE_W | ESXDOS_MODE_CN);

      if (INV_FILE_HND == g_tState.dump.hFile)
      {
        iReturn = EACCES; /* Error */
      }
    }
  }

  if (EOK == iReturn)
  {
    DBGPRINTF("dumpSysInfo() - features = 0x%02X\n", g_tState.uiFeatures);

    if (g_tState.uiFeatures & FEATURE_REGISTERS)
    {
      dumpRegisters();
    }

    if (g_tState.uiFeatures & FEATURE_SYSVARS)
    {
      dumpVariables();
    }

    if (g_tState.uiFeatures & FEATURE_NEXTOS)
    {
      dumpOperatingSystem();
    }
  }

  if (EOK == iReturn)
  {
    zheader("%s (version " APP_VERSION_STR ")", strupr(APP_INTERNALNAME_STR));
  }

  if (INV_FILE_HND != g_tState.dump.hFile)
  {
    esx_f_close(g_tState.dump.hFile);
    g_tState.dump.hFile = INV_FILE_HND; 
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/* showHelp()                                                                 */
/*----------------------------------------------------------------------------*/
int showHelp(void)
{
  char_t acAppName[0x10];
  strncpy(acAppName, APP_INTERNALNAME_STR, sizeof(acAppName));
  strupr(acAppName);

  printf("%s\n\n", APP_DESCRIPTION_STR);

  printf("%s file [-t rvo][-f][-q][-h][-v]\n\n", acAppName);
  //      0.........1.........2.........3.
  printf(" file        name of logfile\n");
  printf(" -t[opic]    topics to show:\n");
  printf("             r[eg] v[ar] o[s]\n");
  printf(" -f[orce]    force overwrite\n");
  printf(" -q[uiet]    no screen output\n");
  printf(" -h[elp]     print this help\n");
  printf(" -v[ersion]  print version info\n");

  return EOK;
}


/*----------------------------------------------------------------------------*/
/* showInfo()                                                                 */
/*----------------------------------------------------------------------------*/
int showInfo(void)
{
  uint16_t uiVersion;
  char_t acBuffer[0x10];

  strncpy(acBuffer, APP_INTERNALNAME_STR, sizeof(acBuffer));
  strupr(acBuffer);

  printf("%s " APP_LEGALCOPYRIGHT_STR "\n", acBuffer);

  if (ESX_DOSVERSION_NEXTOS_48K != (uiVersion = esx_m_dosversion()))
  {
    snprintf(acBuffer, sizeof(acBuffer), "NextOS %u.%02u",
             ESX_DOSVERSION_NEXTOS_MAJOR(uiVersion),
             ESX_DOSVERSION_NEXTOS_MINOR(uiVersion));
  }
  else
  {
    strncpy(acBuffer, "48K mode", sizeof(acBuffer));
  }

  //      0.........1.........2.........3.
  printf(" Version %s (%s)\n", APP_VERSION_STR, acBuffer);
  printf(" Version %s (libzxn)\n", ZXN_VERSION_STR);
  printf(" Stefan Zell (info@diezells.de)\n");

  return EOK;
}


/*----------------------------------------------------------------------------*/
/* zprintf()                                                                  */
/*----------------------------------------------------------------------------*/
int zprintf(const char_t* acFmt, ...)
{
  int iReturn = 0;
  size_t uiLen;
  va_list args;

  va_start(args, acFmt);
  iReturn = vsnprintf(g_tState.dump.acBuffer, sizeof(g_tState.dump.acBuffer), acFmt, args);
  va_end(args);

  uiLen = strnlen(g_tState.dump.acBuffer, sizeof(g_tState.dump.acBuffer));

  if (!g_tState.bQuiet)
  {
    (void) printf(g_tState.dump.acBuffer);
  }

  if (INV_FILE_HND != g_tState.dump.hFile)
  {
    if (uiLen != esx_f_write(g_tState.dump.hFile, g_tState.dump.acBuffer, uiLen))
    {
      iReturn = 0;
    }
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/* zheader()                                                                  */
/*----------------------------------------------------------------------------*/
int zheader(const char_t* acFmt, ...)
{
  int iReturn = 0;
  va_list args;

  static char_t* acBuffer = 0;

  if (0 == acBuffer)
  {
    acBuffer = malloc(g_tState.tScreen.cols + 1);
  }

  if (0 != acBuffer)
  {
    for (uint8_t i = 0; i < g_tState.tScreen.cols; ++i)
    {
      acBuffer[i] = '_';
    }

    acBuffer[g_tState.tScreen.cols] = '\0';
    zprintf("%s", acBuffer);

    if (INV_FILE_HND != g_tState.dump.hFile) /* UGLY HACK ;-) */
    {
      esx_f_write(g_tState.dump.hFile, "\n", 1);
    }

    va_start(args, acFmt);
    iReturn = vsnprintf(acBuffer, g_tState.tScreen.cols + 1, acFmt, args);
    va_end(args);

    zprintf("%s\n\n", acBuffer);
  }

  return iReturn;
}


/*----------------------------------------------------------------------------*/
/* _cpuspeed()                                                                */
/*----------------------------------------------------------------------------*/
uint8_t _cpuspeed(void)
{
  return g_tState.uiCpuSpeed;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
