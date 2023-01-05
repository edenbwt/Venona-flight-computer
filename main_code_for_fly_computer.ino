#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>

// Create objects for each sensor
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

//Create objects for each user device
const int Led_red = 5;
const int Led_green = 6;
const int Led_bleu = 7; //for the Rgb Led on the board

const int Buzzer = 8;

const int pyro1Pin = 2; // pin for pyrotechnic channel 1
const int pyro2Pin = 3; // pin for pyrotechnic channel 2

int pyro1Status = LOW; // status of pyro channel 1 (LOW = off, HIGH = on)
int pyro2Status = LOW; // status of pyro channel 2 (LOW = off, HIGH = on)

// Create variables to store sensor data
float temperature, pressure, altitude;
int16_t ax, ay, az, gx, gy, gz; //a is for acceleration read in Gforce, g is for gyroscope axes placement in space

// Create variables for login data
File dataFile;
String dataString;

void setup() {
// Initialize pyro channels 
pinMode(pyro1Pin, OUTPUT);
pinMode(pyro2Pin, OUTPUT);
digitalWrite(pyro1Pin, pyro1Status);
digitalWrite(pyro2Pin, pyro2Status);

// Initialize Led 
pinMode(Led_red, OUTPUT);
pinMode(Led_bleu, OUTPUT);
pinMode(Led_green, OUTPUT);

digitalWrite(Led_bleu, HIGH);
digitalWrite(Led_red, HIGH);
digitalWrite(Led_green, HIGH);

// Initialize Buzzer
pinMode(Buzzer, OUTPUT);

// Initialize sensors
if (!bmp.begin()) {
Serial.println("Error initializing BMP280");
digitalWrite(Led_red, LOW);
}
if (!mpu.begin()) {
Serial.println("Error initializing MPU6050");
digitalWrite(Led_red, LOW);
}

// Initialize SD card
if (!SD.begin(4)) {
Serial.println("Error initializing SD card");
digitalWrite(Led_red, LOW);
}

// Open data file for writing
dataFile = SD.open("flight_data.txt", FILE_WRITE);
}

void loop() {
// Read sensor data
temperature = bmp.readTemperature();
pressure = bmp.readPressure() / 100.0F;
altitude = bmp.readAltitude(1013.25); // assuming standard pressure at sea level
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

// Build data string to log
dataString = "Temperature: " + String(temperature) + " C\nPressure: " + String(pressure) + " hPa\nAltitude: " + String(altitude) + " m\nAcceleration (x, y, z): " + String(ax) + ", " + String(ay) + ", " + String(az) + "\nGyroscope (x, y, z): " + String(gx) + ", " + String(gy) + ", " + String(gz) + "\n";

// Write data string to file
dataFile.println(dataString);

// Check if pyro channels should be activated
if (ay < -0.5) {
pyro1Status = HIGH;
digitalWrite(pyro1Pin, pyro1Status);
}
    //if (/ some condition */) {
    //pyro2Status = HIGH;
    //digitalWrite(pyro2Pin, pyro2Status);
    //}
}
