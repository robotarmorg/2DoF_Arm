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

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <stdint.h>
#include "StrToken.h"

class CommandLine
{
public:
    typedef void (*CmdFunc_t)(const char *cmd, StrTokenizer &tokenizer);

    typedef struct {
        const char *mCmd;           // Pointer to a flash string
        const char *mArgs;          // Pointer to a flash string
        const char *mDescription;   // Pointer to a flash sting
        CmdFunc_t   mFunc;
    } Entry_t;

                CommandLine();
    void        Init(Entry_t *cmdEntries, size_t numCmdEntries);
    Entry_t    *Commands()      { return mCmdEntry; }
    size_t      NumCommands()   { return mNumCmdEntries; }
    void        Process();

private:
    char        mBuffer[128];
    uint8_t     mIndex;
    bool        mPromptDisplayed;
    char        mToken[32];

    Entry_t    *mCmdEntry;
    size_t      mNumCmdEntries;
};

#endif  // COMMANDLINE_H

