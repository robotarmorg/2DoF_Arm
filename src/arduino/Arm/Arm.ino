/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include "Log.h"
#include "HeartBeat.h"
#include "Commands.h"

HeartBeat   heartbeat;

// the setup function runs once when you press reset or power the board
void setup() {
    LogInit(115200);
    Log("***** ARM Control Program *****\n");
    InitCommands();
    heartbeat.Init(13);
}

// the loop function runs over and over again forever
void loop() {
    heartbeat.Process();
    ProcessCommands();
}
