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
*   @file   log.c
*
*   @brief  This file contains the implementation of the logging functions.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"

#if CFG_LOG_USE_STDIO
#   include <stdio.h>
#endif
#if CFG_LOG_USE_SERIAL
#include "Arduino.h"
#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

#if defined(AVR)

#undef  Log
#undef  LogError
#undef  LogAssertFailed
#undef  vLog

#define Log             Log_P
#define LogError        LogError_P
#define LogAssertFailed LogAssertFailed_P
#define vLog            vLog_P

#else

#define PSTR(str)   str

#endif

int gVerbose = 0;
int gDebug = 0;

// ---- Private Variables ---------------------------------------------------

#if CFG_LOG_USE_STDIO

FILE   *gLogFs = NULL;

#endif

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Log
 * @{
 */

//***************************************************************************
/**
*   Sets the logging stream
*/

#if CFG_LOG_USE_STDIO
void LogInit(FILE *logFs) {
    gLogFs = logFs;

}

#elif CFG_LOG_USE_SERIAL

void LogInit(unsigned long baudrate) {
    Serial.begin(baudrate);
    // Print a newline so that our first print starts on a new line
    // rather than after the chars printed by the bootloader.
    Serial.write("\n");
}

#else
#error  Need to define an appropriate CFG_LOG_USE_XXX macro
#endif

//***************************************************************************
/**
*   Logs a string using printf like semantics
*/

void Log(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
#if defined(AVR)
    vLog_P(fmt, args);
#else
    vLog(fmt, args);
#endif
    va_end(args);
}

//***************************************************************************
/**
*   Logs a string using printf like semantics
*/

void vLog(const char *fmt, va_list args) {
#if defined( AVR )
#if CFG_LOG_USE_STDIO
    if (gLogFs != NULL) {
        vfprintf_P(gLogFs, fmt, args);
    }
#elif CFG_LOG_USE_SERIAL
    char buf[128];

    vsnprintf_P(buf, sizeof(buf), fmt, args);
    Serial.write(buf);
#else
#error Need to define an approproate CFG_LOG_USE_XXX
#endif
#else
    vfprintf(stdout, fmt, args);
#endif
}

/***************************************************************************/
/**
*   Logs an error
*/

void LogError(const char *fmt, ...) {
    va_list args;

#if defined( AVR )
    Log_P(PSTR("ERROR: "));

    va_start(args, fmt);
    vLog_P(fmt, args);
    va_end(args);
#else
    Log("ERROR: ");

    va_start(args, fmt);
    vLog(fmt, args);
    va_end(args);
#endif
}

/***************************************************************************/
/**
*   Logs an assertion failure
*/

void LogAssertFailed(const char *expr, const char *fileName, unsigned lineNum, const char *function) {
#if defined( AVR )
    Log_P(PSTR( "ASSERT failed: "));
    Log_P(fileName);
    Log_P(PSTR( ": %d: " ), lineNum);
    Log_P(function);
    Log_P(PSTR( " Assertion '"));
    Log_P(expr);
    Log_P(PSTR( "' failed.\n"));
#else
    Log("ASSERT: %s: %d: %s Assertion '%s' failed.\n",
        fileName, lineNum, function, expr);
#endif
}

/** @} */

