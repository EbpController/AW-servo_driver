// this project is part of the 'EBP controller' solution
//
// AW stands for turnout, point or switch on the track (belgium: Aiguillage - Wissel)
// this interface controls the AW (CAW) and returns information about the correct position of the AW (KAW)
// a maximum of 6 AWs can be controlled
// the 6 AW servomotors are connected to the Arduino pwm outputs: pins 3, 5, 6, 9, 10 and 11
// the 6 AW relays are connected to the Arduino outputs: pins 2, 4, 7, 8, 12 and 13
// the relay output follows the position of the AW and can be used for general purpose
// for communication, to control the AW, we use the EBP serial bus
//
// written by: EBP Controller
//
// version: 1.0   date: 26/06/2020    comment: creation
// version: 1.1   data: 20/06/2021    comment: returns KAW state also by receiving a comment where CAW has equal value
//                                             returns no KAW information by start-up
//
// included libraries
//
#include "AW-servo.h"
#include "EbpSerial.h"
#include "EEPROM.h"

#define ADDRESS_HIGH 0b0001
#define SWEEP_TIME 3000
#define SWEEP_ANGLE 65

// set AW and relay ports
AW AWs[6];
const int AW_servoPorts[] = {3, 5, 6, 9, 10, 11};
const int AW_relayPorts[] = {2, 4, 7, 8, 12, 13};

// define CAW and KAW
byte CAW_old = 0;
byte CAW_new = 0;
byte KAW_old = 0;
byte KAW_new = 0;

// EbpSerial
unsigned int myAddress;
EbpSerial EbpSerial;

// EEPROM
unsigned int CAW_EEPROM_address = 0;

// setup routine
void setup()
{
  // set bits 0 to 5 of port C as inputs with pullup, without changing the state of bits 6 and 7
  DDRC &= 0b11000000;
  PORTC |= 0b00111111;

  // parameters for AW (sweet time and sweep angle)
  AW::setSweepTime(SWEEP_TIME);
  AW::setSweepAngle(SWEEP_ANGLE);

  // get CAW from EEPROM
  CAW_old = EEPROM.read(CAW_EEPROM_address);
  CAW_new = CAW_old;
  
  // setup AW's (set ports for servo and relay)
  for (int i = 0; i < 6; i++) {
    bool CAW = true;
    if ((CAW_new & (1 << i)) == 0) {
      CAW = false;
    }
    AWs[i].setPorts(AW_servoPorts[i], AW_relayPorts[i], CAW);
  }
  KAW_old = 0b00111111;
  KAW_new = KAW_old;

  // setup serial port
  myAddress = (ADDRESS_HIGH << 6) + ((PINC ^ 0b11111111) & 0b00111111);

  EbpSerial.init(myAddress);
}

void loop()
{
  byte receivedData = EbpSerial.poll();
  byte subAddress = 0xff;
  
  if (receivedData != 0xff) {
    subAddress = (receivedData >> 1) & 0b111 ;
    if ((receivedData & 1) == 1) {
      CAW_new |= 1 << subAddress;
    }
    else {
      CAW_new &= ~(1 << subAddress);
    }
  }

  for (int i = 0; i < 6; i++) {
    // drive the AW's depending on state of CAW
    if ((CAW_new & (1 << i)) == 0) {
      if (AWs[i].set_AW_0() == true) {
        KAW_new |= (1 << i);
      }
      else {
        KAW_new &= ~(1 << i);
      }
    }
    else {
      if (AWs[i].set_AW_180() == true) {
        KAW_new |= (1 << i);
      }
      else {
        KAW_new &= ~(1 << i);
      }
    }

    // send KAW state
    byte KAW_changed = KAW_old ^ KAW_new;
     
    // check if KAW is changed
    if (((KAW_changed & (1 << i)) != 0) || (subAddress == i)) {
      if ((KAW_new & (1 << i)) == 0) {
        // bit 0 = '0'
        // bit 1 to 3 = device (subaddress)
        // refer to protocol information
        EbpSerial.sendData((i << 1) & 0b00001110);
      }
      else {
        // bit 0 = '1'
        // bit 1 to 3 = device (subaddress)
        // refer to protocol information
        EbpSerial.sendData((i << 1) | 1);
      }
    }
  KAW_old = KAW_new;
  }

  // serial poll (update CAW)
  if (CAW_new != CAW_old) {
    // if CAW is changed, store CAW in EEPROM
    EEPROM.write(CAW_EEPROM_address, CAW_new);
    CAW_old = CAW_new;
  }
}
