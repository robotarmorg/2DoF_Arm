/****************************************************************************
*
*   Copyright (c) 2016 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/
/**
*
*   @file   Log.h
*
*   @brief  Contains some logging macros.
*
****************************************************************************/
/**
*   @defgroup   Log   Logging
*
*   @brief      Provides a common interface for logging.
*
****************************************************************************/

#if !defined( LOG_H )
#define LOG_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <stdarg.h>
#include "Config.h"

#if defined( AVR )
#   include <avr/pgmspace.h>

#define LOG_PSTR(s)     PSTR(s)

#endif  // AVR

/**
 * @addtogroup Log
 * @{
 */

#if !defined( CFG_LOG_ENABLED )
#   define  CFG_LOG_ENABLED 1
#endif

#if !CFG_LOG_ENABLED

#define  Log( fmt, args... )
#define  LogError( fmt, args... )
#define  LogAssertFailed( exprStr, file, lineNum, function )

#define  ASSERT(expr)   ((void)0)

#else

#define  ASSERT(expr)   ((expr) ? ((void)0) : LogAssertFailed( #expr, __FILE__, __LINE__, __FUNCTION__ ), ((void)0))

#if defined( __cplusplus )
extern "C"
{
#endif

/***************************************************************************
*
*   Regular logging support
*/

#if CFG_LOG_USE_STDIO
#include <stdio.h>

extern FILE *gLogFs;

void LogInit(FILE *logFs);
#endif

#if CFG_LOG_USE_SERIAL
void LogInit(unsigned long baudRate);
#endif

#if defined(AVR)

void Log_P(const char *fmt, ...);
void LogError_P(const char *fmt, ...);
void LogAssertFailed_P(const char *exprStr, const char *file, unsigned lineNum, const char *function);
void vLog_P(const char *fmt, va_list args);

#define Log(fmt, args...)           Log_P(LOG_PSTR(fmt), ## args)
#define LogError(fmt, args...)      LogError_P(LOG_PSTR(fmt), ## args)
#define LogAssertFailed(exprStr, file, lineNum, function)    LogAssertFailed_P(LOG_PSTR(expr), LOG_PSTR(file), lineNum, LOG_PSTR(function))
#define vLog(fmt, args)             vLog_P(LOG_PSTR(fmt), args)

#else   // AVR

void Log(const char *fmt, ...);
void LogError(const char *fmt, ...);
void LogAssertFailed(const char *expr, const char *file, unsigned lineNum, const char *function);
void vLog(const char *fmt, va_list args);

#endif  // AVR

extern  int     gVerbose;
extern  int     gDebug;

#define LogDebug(fmt, args...)      do { if (gDebug)   { Log(fmt, ## args); }} while (0)
#define LogVerbose(fmt, args...)    do { if (gVerbose) { Log(fmt, ## args); }} while (0)

#if defined( __cplusplus )
}
#endif

#endif  // CFG_LOG_ENABLED

/** @} */

#endif // LOG_H

