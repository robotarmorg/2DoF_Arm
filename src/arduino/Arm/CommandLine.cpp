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
#include "CommandLine.h"
#include "Log.h"

CommandLine::CommandLine()
    : mIndex(0),
      mPromptDisplayed(false),
      mCmdEntry(NULL),
      mNumCmdEntries(0)
{
}

void CommandLine::Init(CommandLine::Entry_t *cmdEntries, size_t numCmdEntries)
{
    mCmdEntry = cmdEntries;
    mNumCmdEntries = numCmdEntries;
    mPromptDisplayed = false;
}

void CommandLine::Process()
{
    if (mIndex == 0 && !mPromptDisplayed) {
        Log("Cmd> ");
        mPromptDisplayed = true;
    }

    if (Serial.available() == 0) {
        return;
    }

    int ch = Serial.read();

    if (ch == '\r' || ch == '\n') {
        Serial.write('\n');
        mBuffer[mIndex] = '\0';

        StrTokenizer tokenizer(mBuffer, mToken, sizeof(mToken));
        char *cmd;
        if ((cmd = tokenizer.NextToken()) != NULL) {
            int i;
            for (i = 0; i < mNumCmdEntries; i++) {
                if (strcmp_P(cmd, mCmdEntry[i].mCmd) == 0) {
                    mCmdEntry[i].mFunc(cmd, tokenizer);
                    break;
                }
            }
            if (i >= mNumCmdEntries) {
                Log("Unrecognized command: '%s'\n", cmd);
            }
        }

        mIndex = 0;
        mPromptDisplayed = false;
        return;
    }
    if (ch == '\x7f' || ch == '\x08') {
        if (mIndex > 0) {
            Serial.write("\x08 \x08");
            mIndex--;
        }
        return;
    }
    Serial.write(ch);
    if (mIndex + 1 < sizeof(mBuffer)) {
        mBuffer[mIndex++] = ch;
    }
}
