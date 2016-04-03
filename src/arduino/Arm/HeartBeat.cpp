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

#include "Arduino.h"
#include "HeartBeat.h"

HeartBeat::HeartBeat()
    : mPin(0),
      mCounter(0),
      mLastTick(0)
{
}

void HeartBeat::Init(int pin)
{
    mPin = pin;
    mCounter = 0;
    mLastTick = millis();

    pinMode(mPin, OUTPUT);
    digitalWrite(mPin, HIGH);
}

void HeartBeat::Process()
{
    unsigned long now = millis();

    if (now - mLastTick < 100) {
        return;
    }
    mLastTick = now;
    mCounter++;
    mCounter %= 10;
    if (mCounter == 1 || mCounter == 3) {
        digitalWrite(mPin, LOW);
    } else if (mCounter == 0 || mCounter == 2) {
        digitalWrite(mPin, HIGH);
    }
}

