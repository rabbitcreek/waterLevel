/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
int counter = 0;
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6
char auth[] = "NeLeTEk53gFyKePf0aCyuqZEUCtT-TEz";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "werner";
char pass[] = "9073456071";

BlynkTimer timer;

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
String airPressureS;
String pressure_absS;
int airPressure;
int pressure_abs;
String LoRaData;
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, airPressure);
  Blynk.virtualWrite(V4, pressure_abs);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Sending  packet:");
  display.setCursor(0,30);
  display.print(LoRaData);
  display.display();
}

void setup() { 
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(60000L, myTimerEvent);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA RECEIVER ");
  display.display();
  
  //initialize Serial Monitor
  Serial.begin(115200);

  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();  
}

void loop() {

  //try to parse packet
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    Serial.print("Received packet ");
   
    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
    }
    counter ++;
    int pos1 = LoRaData.indexOf('/');
    int pos2 = LoRaData.indexOf('#');
    airPressureS =  LoRaData.substring(0,pos1);
    pressure_absS = LoRaData.substring(pos1 + 1, pos2);
    airPressure = airPressureS.toInt();
    pressure_abs = pressure_absS.toInt();
    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");    
    Serial.println(rssi);

   // Dsiplay information
   display.clearDisplay();
   display.setCursor(0,0);
   display.print("LORA RECEIVER");
   display.setCursor(0,20);
   display.print("Received packet:");
   display.setCursor(0,30);
   display.print(LoRaData);
   display.setCursor(0,40);
   display.print("RSSI:");
   display.setCursor(30,40);
  display.print(rssi);
   display.setCursor(0,50);
   display.print("LoraCount:");
   display.setCursor(60,50);
   display.println(counter);  
   display.display(); 
   
  }
}
