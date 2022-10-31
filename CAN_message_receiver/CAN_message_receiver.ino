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
//Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);


void setup() {

  Can0.begin(500000); //set speed here. 
  Can1.begin(500000); //set speed here. 

// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();

}


void set_display(){
  /*
display.setCursor(0, 0); // top left
display.setCursor(15, 0); // top right
display.setCursor(0, 1); // bottom left
display.setCursor(15, 1); // bottom right
 */

  display.clearDisplay(); // Clears the display
  display.drawRect(1,1,127,63,WHITE); // display.drawRect(x, y, width, height)
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(5,5);
  //display.setTextSize(1); Default 1
  display.println("MAS245 - Gruppe 2");
  display.drawLine(1,14,127,14, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);
  display.setCursor(3,18);
  display.setTextSize(0.5);
  display.println("CAN-statistikk");
  display.drawLine(1,28,127,28, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);
  display.setCursor(3,30);
  display.setTextSize(0.5);
  display.println("Antall mottatt:");
  display.setCursor(3,40);
  display.setTextSize(0.5);
  display.println("Mottok sist ID:");
  display.drawLine(1,49,127,49, WHITE); // display.drawLine(x0,y0,x1,y1, WHITE);
  display.setCursor(3,52);
  display.println("IMU-maaling:");
  display.display();
}

void loop() {

 set_display();


}
