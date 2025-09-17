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

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/
#define ERROR_SPECIFIC (0x0200)
#define EBREAK   (ERROR_SPECIFIC + 0x00)
#define ETIMEOUT (ERROR_SPECIFIC + 0x01)


#ifndef ERANGE
  #define ERANGE __ERANGE
  #warning "ERANGE not defined in errno.h (typo ?)"
#endif

#ifndef RTM_28MHZ
  #define RTM_28MHZ 0x03
#endif

#define INV_FILE_HND (0xFF)
#define ESX_DIR_SEP "/"

#define MAX_LINE_LEN (80)

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
  const unsigned char* acText;
} errentry_t;

/*!
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
int zprintf(char* acFmt, ...);

/*!
This function returns a pointer to a textual error message for the given
error code.
*/
const unsigned char* _strerror(int iCode);

/*!
Internal function: The cpu speed is latched at startup of the application. The
value read from the register is "28 MHz" because that is set at startup ...
*/
uint8_t _cpuspeed(void);

/*!
Internal function: Convert the 24bit-value from sysvar "FRAMES" to 32bit.
*/
uint32_t _frames(uint8_t * pFrames);

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
