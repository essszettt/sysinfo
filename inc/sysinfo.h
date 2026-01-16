/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: sysinfo.h                                                          |
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

#if !defined(__SYSINFO_H__)
  #define __SYSINFO_H__

/*============================================================================*/
/*                               Includes                                     */
/*============================================================================*/
#include "libzxn.h"

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/
/*!
Maximum length of a text buffer that is used to render a line on screen output
and the log file. 
*/
#define LINE_LEN_MAX (0x180)

#define sKEY_ON       "on"
#define sKEY_OFF      "off"
#define sKEY_TRUE     "true"
#define sKEY_FALSE    "false"
#define sKEY_ENABLED  "enabled"
#define sKEY_DISABLED "disabled"
#define sKEY_FIRST    "first"
#define sKEY_SECOND   "second"
#define sKEY_UNKNOWN  "unknown"
#define sKEY_PRESSED  "pressed"
#define sKEY_RELEASED "released"

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
/*!
Enumeration to describe all valid tasks of the application
*/
typedef enum _action
{
  ACTION_NONE = 0,
  ACTION_HELP,
  ACTION_INFO,
  ACTION_DUMP
} action_t;

/*!
Structure to describe a entry of the global table with all valid error codes and
messages that can be handovered back to BASIC.
*/
typedef struct _errentry
{
  int iCode;
  const char_t* acText;
} errentry_t;

/*!
Enumeration of all categories of displayed/saved topics
*/
typedef enum _features
{
  FEATURE_REGISTERS = 0x01,
  FEATURE_NEXTOS    = 0x02,
  FEATURE_SYSVARS   = 0x04
} features_t;

/*============================================================================*/
/*                               Prototypen                                   */
/*============================================================================*/
/*!
Replacement for "printf". This funkction prints on the screen and dumps to
a file.
*/
int zprintf(const char_t* acFmt, ...);

/*!
Print a title line on the screen and the log file
*/
int zheader(const char_t* acFmt, ...);

/*!
Internal function: The cpu speed is latched at startup of the application. The
value read from the register is "28 MHz" because that is set at startup ...
*/
uint8_t _cpuspeed(void);

/*============================================================================*/
/*                               Klassen                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Implementierung                              */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#endif /* __SYSINFO_H__ */
