#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SimpleDHT.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define pinDHT11 33
#define cooling 19

#define ON 1
#define OFF 0

float BMP_temp = 10.0;
float BMP_airp = 20.0;
byte DHT_temp = 30;
byte DHT_humi = 40;

int Local_cooling = OFF;

SimpleDHT11 dht11(pinDHT11);
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {
    Serial.begin(9600);
    //setup for pressure
    while ( !Serial ) delay(100);   // wait for native usb
    Serial.println(F("BMP280 test"));
    unsigned status;
    //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
    status = bmp.begin(0x76);
    if (!status) {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                        "try a different address!"));
      Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      //while (1) delay(10);
    }
    
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
        
    // setup for dht
    pinMode(cooling, OUTPUT);
    pinMode(23, OUTPUT);
}

void loop() {
    Serial.print(F("BMP Temperature = "));
    BMP_temp = bmp.readTemperature();
    Serial.print(BMP_temp);
    Serial.println(" *C");
    
    //dht
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&DHT_temp, NULL, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); 
      Serial.println(err); 
      delay(1000);
    }
    Serial.print("DHT11 Temperature="); Serial.println((int)DHT_temp);
    
    if(BMP_temp >= 28.9)
      Local_cooling = ON;
    else
      Local_cooling = OFF;
    
    if(Local_cooling){
      digitalWrite(cooling, LOW);
      digitalWrite(23, LOW);}
    else{
      digitalWrite(cooling, HIGH);
      digitalWrite(23, HIGH);}
    
    Serial.println("\n");
    delay(5000);
}
