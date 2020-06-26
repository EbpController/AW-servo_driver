#ifndef AW_SERVO_H
#define AW_SERVO_H

// included libraries
#include "AW-servo.h"
#include <Servo.h>
#include <Arduino.h>

// initialize static members
const unsigned int AW::_sweepPosition_90 = 90;

unsigned int AW::_sweepTime = 1000;
unsigned int AW::_sweepAngle = 120;
unsigned int AW::_sweepPosition_0;
unsigned int AW::_sweepPosition_180;
unsigned int AW::_sweepDelay;

// constructor
AW::AW() {
  initSweep();
}

// destructor
AW::~AW() {
}

// functions
void AW::setSweepTime(unsigned int sweepTime) {
  // set the sweep time (with a maximum of 10000 msec)
  if (sweepTime > 10000) {
    _sweepTime = 10000;
  }
  else {
    _sweepTime = sweepTime;
  }
  initSweep();
}

void AW::setSweepAngle(unsigned int sweepAngle) {
  // set the sweep angle (with a minimum of 10 degrees and a maximum of 180 degrees)
  if (sweepAngle > 180) {
    _sweepAngle = 180;
  }
  else if (sweepAngle < 10) {
    _sweepAngle = 10;
  }
  else {
    _sweepAngle = sweepAngle;
  }
  initSweep();
}

void AW::initSweep() {
  // with the sweep time and the sweep angle we can calculate the sweep delay
  _sweepDelay = _sweepTime / _sweepAngle;
  _sweepPosition_0 = _sweepPosition_90 - (_sweepAngle / 2);
  _sweepPosition_180 = _sweepPosition_90 + (_sweepAngle / 2);
}

void AW::setPorts(int servoPort, int relayPort, bool CAW) {
  // setup servo
  if (CAW == false) {
    _sweepPosition = _sweepPosition_0;
  }
  else {
    _sweepPosition = _sweepPosition_180;
  }
  _servo.write(_sweepPosition);
  _servo.attach(servoPort);

  // setup relay, if value = -1 then no relay needed
  _relayPort = relayPort;
  if (_relayPort != -1) {
    pinMode(relayPort, OUTPUT);
  }
}

bool AW::set_AW_0() {
  // sweep to minimum position
  if (_sweepPosition > _sweepPosition_0) {
    _KAW = false;
    if (_servo.attached() == true) {
      _timeNow = millis();
      _timeDelta = _timeNow - _timeOld;
      if (_timeDelta > _sweepDelay) {
        _timeOld = _timeNow;
        _sweepPosition--;
        _servo.write(_sweepPosition);
      }
    }
  }
  else {
    _KAW = true;
    if (_relayPort != -1) {
      digitalWrite(_relayPort, true);
    }
  }
  return _KAW;
}

bool AW::set_AW_180() {
  // sweep to maximum position
  if (_sweepPosition < _sweepPosition_180) {
    _KAW = false;
    if (_servo.attached() == true) {
      _timeNow = millis();
      _timeDelta = _timeNow - _timeOld;
      if (_timeDelta > _sweepDelay) {
        _timeOld = _timeNow;
        _sweepPosition++;
        _servo.write(_sweepPosition);
      }
    }
  }
  else {
    _KAW = true;
    if (_relayPort != -1) {
      digitalWrite(_relayPort, false);
    }
  }
  return _KAW;
}

#endif
