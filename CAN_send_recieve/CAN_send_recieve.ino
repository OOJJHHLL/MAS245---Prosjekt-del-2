// -------------------------------------------------------------
// CANtest for Teensy 3.6 dual CAN bus
// by Collin Kidder, Based on CANTest by Pawelsky (based on CANtest by teachop)
//
// Both buses are left at default 250k speed and the second bus sends frames to the first
// to do this properly you should have the two buses linked together. This sketch
// also assumes that you need to set enable pins active. Comment out if not using
// enable pins or set them to your correct pins.
//
// This sketch tests both buses as well as interrupt driven Rx and Tx. There are only
// two Tx buffers by default so sending 5 at a time forces the interrupt driven system
// to buffer the final three and send them via interrupts. All the while all Rx frames
// are internally saved to a software buffer by the interrupt handler.
//

#include <FlexCAN.h>

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";

// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}


// -------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  delay(1000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));
  //Serial.begin(250000);

  Can0.begin(250000);  

  //if using enable pins on a transceiver they need to be set on
  // Sjekk at du bruker riktig pin
  pinMode(2, OUTPUT); //can0

  digitalWrite(2, HIGH);

  msg.ext = 0;
  msg.id = 0x101; //Det man sender ut
  msg.len = 8;
  // Meldingene som skal sendes
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
}


// -------------------------------------------------------------
void loop(void)
{
  CAN_message_t inMsg;
  while (Can0.available()) 
  {
    Can0.read(inMsg); // Leser fra transmit i PCAN-view?
    Serial.print("CAN bus 0: "); 
    hexDump(8, inMsg.buf);
    delay(20);
    //delay(1000);
    //Serial.write(insMsg);
    //Can0.write(inMsg); // Skriver meldingen til recieve i PCAN-view
    //delay(1000);
  }
  msg.buf[0]++;
  Can0.write(msg);
  msg.buf[1]++;
  Can0.write(msg);
  msg.buf[2]++;
  Can0.write(msg);
  msg.buf[3]++;
  Can0.write(msg);
  msg.buf[4]++;
  Can0.write(msg);  
  msg.buf[5]++;
  Can0.write(msg);
  msg.buf[6]++;
  Can0.write(msg);
  msg.buf[7]++;
  Can0.write(msg);      
  delay(1000);

}
