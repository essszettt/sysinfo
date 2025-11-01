/*-----------------------------------------------------------------------------+
|                                                                              |
| filename: dumpVars.h                                                         |
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

#if !defined(__DUMPVARS_H__)
  #define __DUMPVARS_H__

/*============================================================================*/
/*                               Includes                                     */
/*============================================================================*/

/*============================================================================*/
/*                               Defines                                      */
/*============================================================================*/
#define DUMP_VARNUM  "%04X"
#define DUMP_VARNAME "%-9s"
#define DUMP_VARSUB  "%-9s"

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
Structure to describe a single system variable
*/
typedef struct _varentry
{
  /*!
  Address of variable in memory
  */
  const uint16_t uiAddress;

  /*!
  Size of variable in memory
  */
  const uint8_t uiSize;

  /*!
  Flags to process the value of variable
  @code
  BIT7  Value is a pointer
  @endcode
  */
  const uint8_t uiFlags;

  /*!
  Name of variable
  */
  const char_t* acName;
} varentry_t;

/*============================================================================*/
/*                               Prototypen                                   */
/*============================================================================*/
/*!
Output all information about system variables of the Next
*/
int dumpVariables(void);

/*!
Mit dieser Funktion kann eine numerischer Wert in eine hexadezimale Ziffer
transformiert werden.
@param uiValue Zu transformierender Wert (nur das Low-Nibble wird verwendet)
@return Transformierte Hexadezimalziffer
*/
char_t nibble2hex(uint8_t uiValue);

/*!
Mit dieser Funktion kann eine Hex-Darstellung (als String) eines beliebigen
Speicherbereichs erzeugt werden.
@param pData Zeiger auf den Quellspeicherbereich
@param uiSize Laenge des darzustellenden Speicherbereichs in [byte]
@param acBuffer Zeiger auf den Zielpuffer, in dem der erzeugte String
                gespeichert werden soll.
@param uiBufferSize Laenge des Zielpuffers in [byte]
@param uiGrouping Anzahl der "Bytes" im Zielstring, nach denen jeweils ein
                  Leerzeichen zur Gruppierung eingefuegt werden soll.
@return Laenge des erzeugten Strings _OHNE_ abschliessende Null.
@remark Der erzeugte String wird auf jeden Fall mit "0" terminiert !

@code
 Beispiel
==========

#include <iostream>
#include "PTypes.h"

int main(int argc, char* argv[])
{
  P_UINT8 pSrc[10];
  P_CHARACTER acDest[64];

  snprintf((char*) pSrc, sizeof(pSrc), "ABCDEFGHIJ");

  P_UINT32 uiSize = 0;

  uiSize = mem2hex(pSrc, sizeof(pSrc), acDest, sizeof(acDest), 2);
  std::cout << "Laenge: " << uiSize << ": " << acDest << std::endl;

  return 0;
}

Ausgabe:
"Laenge: 25: 4142 4344 4546 4748 4900"

@endcode
*/
unsigned long mem2hex(const void* const pData,
                      unsigned long uiSize,
                      char_t* acBuffer,
                      unsigned long uiBufferSize,
                      unsigned long uiGrouping);

/*============================================================================*/
/*                               Klassen                                      */
/*============================================================================*/

/*============================================================================*/
/*                               Implementierung                              */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#endif /* __DUMPVARS_H__ */
