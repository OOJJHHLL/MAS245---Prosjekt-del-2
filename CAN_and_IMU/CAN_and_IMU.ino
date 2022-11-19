/* Sende IMU-verdier over CAN-bus
  * Koden baserer seg på:
  * CANtest for Teensy 3.6 dual CAN bus av Collin Kidder fra FlexCAN library master biblioteket
  * Teensy CAN-Bus with OLED 128x64 demoen til skpang
  * IMU...
*/

#include <FlexCAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU9250_WE.h>

#define MPU9250_ADDR 0x68 


MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


CAN_message_t msg;
CAN_message_t inMsg;

static uint8_t hex[17] = "0123456789abcdef";



// Hexdump: Prints contents of a file, byte by byte - https://people.sc.fsu.edu/~jburkardt/cpp_src/hexdump/hexdump.html
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


void setup() 
{
  Can0.begin(250000); // bit-rate

  // CAN0 porten benyttes
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  msg.ext = 0;
  msg.id = 0x22; 
  msg.len = 8;

  // Initialiserer meldingene som skal sendes
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
  

  Serial.begin(115200);
  Wire.begin();
  if(!myMPU9250.init()){
    Serial.println("MPU9250 does not respond");
  }
  else{
    Serial.println("MPU9250 is connected");
  }
 Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
  delay(1000);
  myMPU9250.autoOffsets();
  Serial.println("Done!");
 myMPU9250.setSampleRateDivider(5);
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
  myMPU9250.enableAccDLPF(true);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);

  myMPU9250.setSampleRateDivider(5);
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
  myMPU9250.enableAccDLPF(true);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);
}


void set_display()
{  
  display.clearDisplay(); // Innholder på displayet fjernes
  display.drawRoundRect(1,1,127,63,4,WHITE); // Rammen tegnes
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
  display.println("IMU-verdi:");
  display.setCursor(95, 52);
  display.println("m/s^2");

  display.display(); // Informasjonen vises på displayet
}


void loop() 
{

  xyzFloat accRaw = myMPU9250.getAccRawValues();
  xyzFloat accCorrRaw = myMPU9250.getCorrectedAccRawValues();
  xyzFloat gValue = myMPU9250.getGValues();
  float resultantG = myMPU9250.getResultantG(gValue);

while (Can0.available()) 
  {
    set_display(); // Henter display-funksjonen

    Can0.read(inMsg); // Leser melding sendt utenifra (fra PCAN-view)
    hexDump(8, inMsg.buf);

    display.setCursor(110,30);
    display.println(rCount);
    display.setCursor(120,40);
    display.println(inMsg.id,HEX); // Skriver HEX for å unngå at ID-en konverteres til int

  }  
  if (inMsg.id == 33) //  0x21 = 33
    {
      rCount = 1; 
      set_display();
      display.setCursor(110,30);
      display.println(rCount);
      display.setCursor(110,40);
      display.println(inMsg.id,HEX);
      display.setCursor(95, 52);
      display.println("m/s^2");

    // Meldingene som sendes til PCAN-view
      msg.buf[0] = gValue.z;
      msg.buf[1] = gValue.y;
      msg.buf[2] = gValue.x;

      Can0.write(msg); // Meldingen sendes

      display.setCursor(69,52);
      display.println(gValue.z*9.81); // Printer g-verdien på OLED-skjermen

      display.display();
      delay(20);
      display.clearDisplay();
      }
  delay(1000); // Meldingen sendes med en rate på 1Hz
}
