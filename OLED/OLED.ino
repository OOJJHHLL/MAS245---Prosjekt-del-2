/* Teensy CAN-Bus with OLED 128x64 */

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
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";
IntervalTimer TX_timer;


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

  int rCount = 1;


void setup() {
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

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();

}


void set_display()
{
  
 
  display.clearDisplay(); // Clears the display
  display.drawRoundRect(1,1,127,63,4,WHITE);
  display.setTextSize(0);
  display.setTextColor(WHITE);

  display.setCursor(5,5);
  display.println("MAS245 - Gruppe 2");
  display.drawLine(1,14,127,14, WHITE); 
  display.setCursor(3,18);
  display.println("CAN-statistikk");
  display.setCursor(3,25);
  display.println("--------------");

  display.setCursor(3,30);
  display.println("Antall mottatt:");


  display.setCursor(3,40);
  display.println("Mottok sist ID:");
  display.setCursor(3,47);
  display.println("--------------");
  
  display.setCursor(3,52);
  display.println("IMU-maaling:");

  display.display();


}

void loop() {

CAN_message_t inMsg;


while (Can0.available()) 
  {

    set_display();
    Can0.read(inMsg); // Leser fra transmit i PCAN-view?
    Serial.print("CAN bus 0: "); 
    hexDump(8, inMsg.buf);

    display.setCursor(100,30);
    Can0.write(inMsg);
    display.println(rCount);

    display.setCursor(100,40);
    display.println(inMsg.id,HEX); //Converterer fra hex til int hvis jeg ikke skriver ,hex

    rCount++;
    display.display();
    delay(20);
    display.clearDisplay();


  }
  msg.buf[0]++;
  msg.buf[1]++;
  msg.buf[2]++;
  msg.buf[3]++;
  msg.buf[4]++;
  msg.buf[5]++;
  msg.buf[6]++;
  msg.buf[7]++;
  Can0.write(msg);      
  delay(1000);


}
