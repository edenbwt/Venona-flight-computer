#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>

// Code written by Eden https://github.com/edenbwt if your using it please give me credit for it i work a lot to make this board and software.

//========================================variables========================================//

// Create objects for each sensor
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

//Create objects for each user device
//for the Rgb Led on the board the LED is a common-anode on = low
const int Led_red = 9;
const int Led_green = 6;
const int Led_bleu = 7; 

const int Buzzer = 8;

const int voltagePin = A0;  // Pin where the voltage divider is connected
const float batteryVoltage = 12.0;  // The nominal voltage of the battery

const int pyro1Pin = 2; // pin for pyrotechnic channel 1
const int pyro2Pin = 3; // pin for pyrotechnic channel 2
const int pyro3Pin = 4; // pin for pyrotechnic channel 3
const int pyro4Pin = 5; // pin for pyrotechnic channel 4

int pyro1Status = LOW; // status of pyro channel 1 (LOW = off, HIGH = on)
int pyro2Status = LOW; // status of pyro channel 2 (LOW = off, HIGH = on)

// Create variables to store sensor data
float temperature, pressure, altitude;
int16_t ax, ay, az, gx, gy, gz; //a is for acceleration read in Gforce, g is for gyroscope axes placement in space

// Create variables for login data
File dataFile;
String dataString;


//=====================================================================================================================================================//
//                                                                        Setup                                                                        //
//=====================================================================================================================================================//


void setup() {
// Initialize pyro channels 
pinMode(pyro1Pin, OUTPUT);
pinMode(pyro2Pin, OUTPUT);
pinMode(pyro3Pin, OUTPUT);
pinMode(pyro4Pin, OUTPUT);
digitalWrite(pyro1Pin, pyro1Status);
digitalWrite(pyro2Pin, pyro2Status);
digitalWrite(pyro3Pin, pyro3Status);
digitalWrite(pyro4Pin, pyro4Status);

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



//====================================================================================================================================================================//
//                                                                        Main Loop                                                                                   //
//====================================================================================================================================================================//


void loop() {
//==================================================Batt================================================//
int voltage = analogRead(voltagePin);

// Convert the voltage to a battery voltage
float batteryVoltage = (voltage / 1023.0) * 5.0 * 2.0;
  
  // Calculate the percentage of charge remaining
float chargePercentage = (batteryVoltage / batteryVoltage) * 100.0;
  
  // Print the percentage of charge remaining to the serial console
Serial.println("[Batt] : " + chargePercentage + "%\n");
if (chargePercentage < 50.0) {
    digitalWrite(Led_red, LOW);
} else {
    digitalWrite(Led_green, LOW)
}

//=============================================Sensors===========================================================//
// Read sensor data
temperature = bmp.readTemperature();
pressure = bmp.readPressure() / 100.0F;
altitude = bmp.readAltitude(1013.25); // assuming standard pressure at sea level, well noing that never the case 
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

// Build data string to log
dataString = "[Temperature] : " + String(temperature) + " C\n[Pressure] : " + String(pressure) + " hPa\n[Altitude] : " + String(altitude) + " m\n[Acceleration] (x, y, z): " + String(ax) + ", " + String(ay) + ", " + String(az) + "\n[Gyroscope] (x, y, z): " + String(gx) + ", " + String(gy) + ", " + String(gz) + "\n";

// Pyro event 
PyroStringEventN°1 = "[!] Event Pyro n°1 GO OFF\n";
PyroStringEventN°2 = "[!] Event Pyro n°2 GO OFF\n";
// Write data string to file
dataFile.println(dataString);


//===============================================pyrotechnic 1-4 =================================================//

// Check if pyro channels should be activated
if (ay < -0.5) { //y-axis acceleration of the object. If the acceleration is less than a certain negative threshold (in this case, -0.5 g), then the object is considered to be falling.
        // setting off Pyro channels 1
    pyro1Status = HIGH;
    digitalWrite(pyro1Pin, pyro1Status);

        // Write in dataFile event for pyro going off 
    dataFile.println(PyroStringEventN°1);
    digitalWrite(Led_bleu, LOW);
}
    
    //if (/ some condition */) {
    //pyro2Status = HIGH;
    //digitalWrite(pyro2Pin, pyro2Status);
    //}

    //if (/ some condition */) {
    //pyro3Status = HIGH;
    //digitalWrite(pyro3Pin, pyro3Status);
    //}

    //if (/ some condition */) {
    //pyro4Status = HIGH;
    //digitalWrite(pyro4Pin, pyro4Status);
    //}
}


//fonction for the different sond i will use to notify my self fot differnt events 

//i did not add any sond yett because i want to hear what i code :)
void buzzer_batt_low{
    

}

void buzzer_reday{
    

}



void buzzer_hope_to_tutch_ground{
    

}


