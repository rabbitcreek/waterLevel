/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
//#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#define DONEPIN 13
//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#include <SparkFun_MS5803_I2C.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
int pressure_abs;
int airPressure;
MS5803 sensor(ADDRESS_HIGH);
Adafruit_BMP3XX bmp; // I2C
//packet counter
int counter = 0;
String message;
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {

  //reset OLED display via software
  pinMode(DONEPIN, OUTPUT);
  digitalWrite(DONEPIN, LOW);
  //pinMode(OLED_RST, OUTPUT);
  //digitalWrite(OLED_RST, LOW);
  delay(20);
  //digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  /*
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  */
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("BMP388 test");
  sensor.reset();
  sensor.begin();
  pressure_abs = sensor.getPressure(ADC_4096);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  //display.setCursor(0,10);
  //display.print("LoRa Initializing OK!");
  //display.display();
  delay(2000);
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
}

void loop() {
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
   pressure_abs = sensor.getPressure(ADC_4096);
   bmp.performReading();
 airPressure = bmp.pressure / 100;
 
  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.pressure / 100.0);
  Serial.println(" hPa");

 
  Serial.println();
  Serial.print("Underwater Sensor:");
  Serial.print(pressure_abs);
  Serial.println(" hPa");  
  Serial.print("Sending packet: ");
  Serial.println(counter);
  message = String(airPressure) + "/" + String(pressure_abs) + "#";
  delay(1000);
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.print("   ");
  LoRa.print(counter);
  LoRa.endPacket();
  /*
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("LoRa packet sent.");
  display.setCursor(0,30);
  display.print("Counter:");
  
  display.setCursor(50,30);
  display.print(counter);
  display.setCursor(0,40);
  display.print("Data: ");
  display.setCursor(50, 40);
  display.print(message);      
  display.display();
*/
  counter++;
  
  delay(10000);
  /*
  while(1) {
    digitalWrite(DONEPIN, HIGH);
    delay(1);
    digitalWrite(DONEPIN, LOW);
    delay(1);
  
}
  */  
}
