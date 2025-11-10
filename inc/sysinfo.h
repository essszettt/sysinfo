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
/*!
Beginning of project specific error codes.
*/
#define ERROR_SPECIFIC (0x0200)

/*!
Error code: BREAK was pressed; abort execution
*/
#define EBREAK (ERROR_SPECIFIC + 0x00)

/*!
Error code: Timeout in operation
*/
#define ETIMEOUT (ERROR_SPECIFIC + 0x01)

#ifndef ERANGE
  /*!
  (Re)definition of error "ERANGE". Maybe just a type in <errno.h> that needs to
  be corrected ...
  */
  #define ERANGE __ERANGE
  #warning "ERANGE not defined in errno.h (typo ?)"
#endif

#ifndef RTM_28MHZ
  /*!
  Missing constant for 28-Mhz-speed of ZXN (missing in <zxn.h>)
  */
  #define RTM_28MHZ 0x03
  #warning "RTM_28MHZ not defined in zxn.h"
#endif

#define INV_FILE_HND (0xFF)
#define ESX_DIR_SEP "/"

/*!
Maximum length of a text buffer that is used to render a line on screen output
and the log file. 
*/
#define LINE_LEN_MAX (0x180)

/*!
This macro can be used like "printf" but it's implementation is only compiled
and linked in debug-builds ("#define __DEBUG__").
*/
#ifdef __DEBUG__
  #define DBGPRINTF(...) do { (void) printf(__VA_ARGS__); } while (0)
#else
  #define DBGPRINTF(...) do { } while (0)
#endif

#define sKEY_ON       "on"
#define sKEY_OFF      "off"
#define sKEY_TRUE     "true"
#define sKEY_FALSE    "false"
#define sKEY_ENABLED  "enabled"
#define sKEY_DISABLED "disabled"
#define sKEY_FIRST    "first"
#define sKEY_SECOND   "second"
#define sKEY_UNKNOWN  "unknown"

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
Alias for `unsigned char`.
Created for those too lazy to type “unsigned char” every time.
 */
typedef unsigned char char_t;

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
This function returns a pointer to a textual error message for the given
error code.
*/
const char_t* zxn_strerror(int iCode);

/*!
Internal function: The cpu speed is latched at startup of the application. The
value read from the register is "28 MHz" because that is set at startup ...
*/
uint8_t _cpuspeed(void);

/*!
Internal function: Convert the 24bit-value from sysvar "FRAMES" to 32bit.
*/
uint32_t zxn_frames(uint8_t* pFrames);

/*!
This function cleans up the given path ('\\' => '/', remove trailing '/' )
@param acPath Path to be cleaned up
@return "0" = no error
*/
int zxn_normalizepath(char_t* acPath);

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
