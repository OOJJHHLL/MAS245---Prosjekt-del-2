/* Teensy CAN-Bus with OLED 128x64 */

#include <FlexCAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

static CAN_message_t msg, rxmsg; // Creates a message variable from the datatype 'CAN_message_t'
IntervalTimer TX_timer; //https://www.pjrc.com/teensy/td_timing_IntervalTimer.html
uint msg_count;
String CANStr(""); 

void setup() {

  Can0.begin(500000); //set speed here. 
  Can1.begin(500000); //set speed here. 


// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  //Serial.begin(500000);
  //Serial.println(F("Hello Teensy 3.6 CAN Test."));

  msg.buf[0] = 1;
  msg.buf[1] = 2;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.len = 8;
  msg.id = 0x7DF;

  TX_timer.begin(tx_CAN, 1000000);    /* Start interrupt timer */

}


//void set_display()
//{
  /*
display.setCursor(0, 0); // top left
display.setCursor(15, 0); // top right
display.setCursor(0, 1); // bottom left
display.setCursor(15, 1); // bottom right
display.setTextSize(1); Default 1
 */ 

 /* display.clearDisplay(); // Clears the display
  //display.drawRect(1,1,127,63,WHITE); // display.drawRect(x, y, width, height)
  display.drawRoundRect(1,1,127,63,4,WHITE);
  display.setTextSize(0);
  display.setTextColor(WHITE);

  display.setCursor(5,5);
  display.println("MAS245 - Gruppe 2");
  display.drawLine(1,14,127,14, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);
  
  display.setCursor(3,18);
  display.setTextSize(0.5);
  display.println("CAN-statistikk");
  display.drawLine(1,28,127,28, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);

  display.setCursor(3,30);
  display.println("Antall mottatt:");

  display.setCursor(3,40);
  display.setTextSize(0.5);
  display.println("Mottok sist ID:");
  display.drawLine(1,49,127,49, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);
  
  display.setCursor(3,52);
  display.println("IMU-maaling:");
  display.display();

}*/


void tx_CAN() // Using serial communication on TX-pin (serial pin). T for transmitter, R for reciever
{
  msg.len = 8; // Length of the message. 8 bit
  msg.id = 0x7DF; // The address the message goes to! Can identifier
  // 0 -> 7. The message has a length of 8
  msg.buf[0] = 0x02;
  msg.buf[1] = 0x01;
  msg.buf[2] = 0x0c;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  Can0.write(msg);
  Can1.write(msg);
  msg_count++;
}


void loop()
{

  //tx_CAN();

  while(Can0.read(rxmsg)) // Recieve message (rx)
  {
    String CANStr("");
    for (int i=0; i < 8; i++) {     

         CANStr += String(rxmsg.buf[i],HEX);
         CANStr += (" ") ;
     } 
  }

  display.clearDisplay(); // Clears the display
  //display.drawRect(1,1,127,63,WHITE); // display.drawRect(x, y, width, height)
  display.drawRoundRect(1,1,127,63,4,WHITE);
  display.setTextSize(0);
  display.setTextColor(WHITE);

  display.setCursor(5,5);
  display.println("MAS245 - Gruppe 2");
  display.drawLine(1,14,127,14, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);
  
  display.setCursor(3,18);
  display.setTextSize(0.5);
  display.println("CAN-statistikk");
  display.drawLine(1,28,127,28, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);

  display.setCursor(3,30);
  display.println("Antall mottatt:");

  display.setCursor(3,40);
  display.setTextSize(0.5);
  display.println("Mottok sist ID:");
  display.setCursor(3,49);
  display.println("--------------");
  
  display.setCursor(3,52);
  display.println("IMU-maaling:");
  //display.println(msg_count++);
  display.display();

    /*Serial.print(rxmsg.id,HEX); 
    Serial.print(' '); 
    Serial.print(rxmsg.len,HEX); 
    Serial.print(' ');
    Serial.println(CANStr);  
     
    display.fillRect(0,15,128, 30,BLACK);
    display.setCursor(0,15);
     
    display.println(rxmsg.id,HEX); 
    display.println(rxmsg.len,HEX); 
    display.println(CANStr);
    display.display();*/

}
