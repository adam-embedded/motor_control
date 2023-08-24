#include <Arduino.h>
#include "main.h"

#include <AFMotor.h>

// A simple motor controller program for PWM motors
// Adam Slaymark

uint8_t motorControl(uint8_t, uint8_t);
AF_DCMotor right(1, MOTOR12_64KHZ);
AF_DCMotor left(2, MOTOR12_64KHZ);

void setup()
{
//   TCCR1B = TCCR1B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz D9 and D10
//   TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz D3 and D11

//   pinMode(RIGHTFWD, OUTPUT);
//   pinMode(RIGHTRWD, OUTPUT);
//   pinMode(LEFTFWD, OUTPUT);
//   pinMode(LEFTRWD, OUTPUT);

  
  right.setSpeed(0);
  right.run(RELEASE);
  delay(500);
  left.setSpeed(0);
  left.run(RELEASE);

  Serial.begin(115200);
}

bool commandReceived = false;

uint32_t timeStart = 0;

void loop()
{

  if (Serial.available())
  {
    if (!commandReceived)
    {
      byte commandByte = Serial.read();

      switch (commandByte)
      {
      case STATUSBIT: // if status, return OK
        Serial.write(byte(STATUSOK));
        break;
      case MOTORBIT: // If motor, return OK and then prep for payload
        timeStart = millis();
        commandReceived = true;
        Serial.write(byte(STATUSOK));
        break;
      }
    }
    else
    {
      byte tmpBuffer[2];
      Serial.readBytes(tmpBuffer, 2);

      Serial.write(tmpBuffer[0]);
      Serial.write(tmpBuffer[1]);

      // motorControl(command, value);

      Serial.write(motorControl(tmpBuffer[0], tmpBuffer[1]));
      //Serial.write(STATUSOK);

      // Reset the command flag
      commandReceived = false;
    }
  }
  uint32_t elasped = timeStart + MAXWAIT;
  if (millis() > (elasped) && commandReceived == true)
  {
    commandReceived = false;
    timeStart = 0;
    Serial.write(STATUSTIME);
  }
}

uint8_t motorControl(uint8_t command, uint8_t value)
{
  uint8_t status = STATUSOK;

  switch (command)
  {
  case RIGHTFWDCMD:
    //analogWrite(RIGHTFWD, value);
    right.setSpeed(value);
    right.run(FORWARD);
    break;
  case RIGHTRWDCMD:
    //analogWrite(RIGHTRWD, value);
    right.setSpeed(value);
    right.run(BACKWARD);
    break;
  case LEFTFWDCMD:
    //analogWrite(LEFTFWD, value);
    left.setSpeed(value);
    left.run(FORWARD);
    break;
  case LEFTRWDCMD:
    //analogWrite(LEFTRWD, value);
    left.setSpeed(value);
    left.run(BACKWARD);
    break;
  case FORWARDCMD:
    // analogWrite(RIGHTFWD, value);
    // analogWrite(LEFTFWD, value);
    left.setSpeed(value);
    left.run(FORWARD);
    right.setSpeed(value);
    right.run(FORWARD);
    break;
  case BACKWARDS:
    // analogWrite(RIGHTRWD, value);
    // analogWrite(LEFTRWD, value);
    left.setSpeed(value);
    left.run(BACKWARD);
    right.setSpeed(value);
    right.run(BACKWARD);
    break;
  case STOPCMD:
    // analogWrite(RIGHTRWD, 0);
    // analogWrite(LEFTRWD, 0);
    // analogWrite(RIGHTFWD, 0);
    // analogWrite(LEFTFWD, 0);
    left.run(RELEASE);
    left.setSpeed(0);
    right.run(RELEASE);
    right.setSpeed(0);
    break;

  default:
    status = STATUSFAIL;
    break;
  }

  return status;
}