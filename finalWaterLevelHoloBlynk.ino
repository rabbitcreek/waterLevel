/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example shows how to use Arduino MKRGSM 1400
  to connect your project to Blynk.
  Note: This requires MKRGSM library
    from https://www.arduino.cc/en/Reference/MKRGSM
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <SPI.h>
#include <MKRGSM.h>
#include <BlynkSimpleMKRGSM.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#define BMP_SCK 9
#define BMP_MISO 8
#define BMP_MOSI 7
#define BMP_CS 6

#include <SparkFun_MS5803_I2C.h> // Click here to get the library: http://librarymanager/All#SparkFun_MS5803-14BA
MS5803 sensor(ADDRESS_HIGH);
MS5803 sensor2(ADDRESS_LOW);
Adafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
//Create variables to store results
float temperature_c, temperature_f, temperature2_f;
double pressure_abs, pressure_abs2, pressure_relative, altitude_delta, pressure_baseline, pressure_baseline2;
#define DONEPIN 4
int pressureOne;
int pressureTwo;
int pressureThree;
int waterFeet;
int counter = 0;
GSMClient client;
GPRS gprs;
GSM gsmAccess;
BlynkTimer timer;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "liW-QBd-FBP5nFYHxd-04ixhXGLSAOWg";

// Your SIM and GPRS credentials
// Leave empty, if missing pin, user or pass
char pin[]  = "";
char apn[]  = "hologram";
char user[] = "";
char pass[] = "";
void endGame()
{ 
  delay(5000);
  Serial.print("got here");
  while (1) {
    digitalWrite(DONEPIN, HIGH);
    delay(1);
    digitalWrite(DONEPIN, LOW);
    delay(1);
  }
}
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, pressureOne);
  Blynk.virtualWrite(V6, pressureTwo);
  Blynk.virtualWrite(V4, pressureThree);
  waterFeet = ( pressureOne - pressureThree ) * (pressureOne - pressureTwo);
  Blynk.virtualWrite(V3, waterFeet);
  counter = counter + 1;
  if (counter = 2){
    endGame();
  }
 
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  delay(1500); 
  Wire.begin();
   
  pinMode(DONEPIN, OUTPUT);
  pinMode(DONEPIN, LOW);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  delay(1000);
  sensor.reset();
  sensor.begin();
  sensor2.reset();
  sensor2.begin();
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  Blynk.begin(auth, gsmAccess, gprs, client, pin, apn, user, pass);
  timer.setInterval(2000L, myTimerEvent);
  
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  delay(2000);
  bmp.performReading();
  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");
  pressureThree = bmp.pressure / 100;
  Serial.print("Pressure = ");
  Serial.print(bmp.pressure / 100.0);
  Serial.println(" hPa");
    // Read temperature from the sensor in deg F. Converting
  // to Fahrenheit is not internal to the sensor.
  // Additional math is done to convert a Celsius reading.
  temperature_f = sensor.getTemperature(FAHRENHEIT, ADC_512);
  temperature2_f = sensor2.getTemperature(FAHRENHEIT, ADC_512);
  // Read pressure from the sensor in mbar.
  pressureOne = sensor.getPressure(ADC_4096);
  pressureTwo = sensor2.getPressure(ADC_4096);
  // Let's do something interesting with our data.

 
  
  // Report values via UART
  Serial.print("Temperature2 F = ");
  Serial.println(temperature2_f);

  Serial.print("Temperature F = ");
  Serial.println(temperature_f);

  Serial.print("Pressure abs (mbar)= ");
  Serial.println(pressureOne);

  Serial.print("Pressure abs 2(mbar)= ");
  Serial.println(pressureTwo);
  delay(2000);
  //myTimerEvent();

}
