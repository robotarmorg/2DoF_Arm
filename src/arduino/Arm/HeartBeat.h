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

#ifndef HEARTBEAT_H
#define HEARTBEAT_H

class HeartBeat
{
public:
    HeartBeat();
    void Init(int pin);
    void Process();

private:
    int             mPin;
    int             mCounter;
    unsigned long   mLastTick;
};

#endif  // HEARTBEAT_H
