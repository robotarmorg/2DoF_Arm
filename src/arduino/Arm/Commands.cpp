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

#include "Commands.h"

#include "Arduino.h"
#include "CommandLine.h"
#include "Kinematics.h"
#include "Log.h"
#include <avr/pgmspace.h>

inline double d2r(double degrees) {
    return degrees * M_PI / 180.0;
}

inline double r2d(double radians) {
    return radians * 180.0 / M_PI;
}

static void argsFunc(const char *cmd, StrTokenizer &tokenizer)
{
    const char *arg;

    Log("args\n");
    while ((arg = tokenizer.NextToken()) != NULL) {
        Log("  %s\n", arg);
    }
}

static void fwdFunc(const char *cmd, StrTokenizer &tokenizer)
{
    double  x;
    double  y;
    double q1;
    double q2;

    if (!tokenizer.NextNum(&q1) || !tokenizer.NextNum(&q2)) {
        LogError("Usage: fwd q1 q2 (angles in degrees)\n");
        return;
    }

    ForwardKinematicsQ(d2r(q1), d2r(q2), &x, &y);

    // Alas printf doesn't support floats, so we do it this way.
    Serial.write("fwd x = ");
    Serial.print(x);
    Serial.write(" y = ");
    Serial.print(y);
    Serial.write("\n");
}

static void invFunc(const char *cmd, StrTokenizer &tokenizer)
{
    double  x;
    double  y;
    double q1;
    double q2;

    if (!tokenizer.NextNum(&x) || !tokenizer.NextNum(&y)) {
        LogError("Usage: inv x y\n");
        return;
    }

    InverseKinematicsQ(x, y, &q1, &q2);

    // Alas printf doesn't support floats, so we do it this way.
    Serial.write("inv q1 = ");
    Serial.print(r2d(q1));
    Serial.write(" q2 = ");
    Serial.print(r2d(q2));
    Serial.write("\n");
}

static const char   argsStr[]   PROGMEM = "args";
static const char   fwdStr[]    PROGMEM = "fwd";
static const char   invStr[]    PROGMEM = "inv";

CommandLine::Entry_t cmdEntry[] = {
    { argsStr,  argsFunc },
    { fwdStr,   fwdFunc },
    { invStr,   invFunc },
};

static CommandLine  gCommandLine;

void InitCommands()
{
    gCommandLine.Init(cmdEntry, sizeof(cmdEntry) / sizeof(cmdEntry[0]));
}

void ProcessCommands()
{
    gCommandLine.Process();
}
