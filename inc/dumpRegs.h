/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: dumpRegs.h                                                         |
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

#if !defined(__DUMPREGS_H__)
  #define __DUMPREGS_H__

/*============================================================================*/
/*                               Includes                                     */
/*============================================================================*/

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/
/*!
Formatz string to define the output of nreg numbers
*/
#define DUMP_REGNUM  "%02X"

/*!
Formatstring to specify output of nreg names
*/
#define DUMP_REGNAME "%-11s"

/*!
Formatstring to specify output of nreg subtopics
*/
#define DUMP_REGSUB  "%-11s"

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
Structure of a entry in the list of registers to be dumped
*/
typedef struct _regentry
{
  /*!
  Number of nreg
  */
  const uint8_t uiNumber;

  /*!
  Special flags to to signal various output formats
  */
  const uint8_t uiFlags;

  /*!
  Name of a nreg
  */
  const char_t* acName;
} regentry_t;

/*============================================================================*/
/*                               Prototypen                                   */
/*============================================================================*/
/*!
Output all information about registers of the Next
@return EOK = "no error"
*/
int dumpRegisters(void);

/*============================================================================*/
/*                               Klassen                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Implementierung                              */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#endif /* __DUMPREGS_H__ */
