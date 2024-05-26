/* 
  Climate Measuring Board (CMB)
  
  Author: Riccardo Finotello
  
  An Arduino Nano ESP32 project for measuring and displaying climate-related variables.
  
  Arduino IoT cloud reference sketch: https://create.arduino.cc/cloud/things/c16fd1dd-be8f-4d62-8ae2-67dcf48459be 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  CloudTemperatureSensor temperature;
  CloudIlluminance luminosity;
  CloudRelativeHumidity humidity;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
#include <DHT.h>
#include "thingProperties.h"


// Constants
const int DHTPIN = 3;  // humidity/temperature sensor data pin
const int DHTTYPE = DHT22;  // type of humidity sensor

const int PHOTOPIN = A2;  // photoresistor pin

const int WAIT_THRESH = 3*1000;  // serial console and sensor update waiting time (ms)

// Variables
DHT dht(DHTPIN, DHTTYPE);  // humidity/temperature sensor

int wait = 0;  // wait interval

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Initialize YAHP
  Serial.print("\n");
  Serial.print("*************************\n");
  Serial.print("    Initializing CMB     \n");
  Serial.print("                         \n");
  Serial.print("    auth: thesfinox      \n");
  Serial.print("    ver:  1.0.0          \n");
  Serial.print("*************************\n\n");

  // Initialize pins
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(PHOTOPIN, INPUT);

  // Initialize the DHT interface
  dht.begin();

  // Delay the execution
  delay(1500);

  // Print data
  Serial.println("HEADER");
  Serial.println("humidity,temperature,luminosity");
  Serial.println("HEADER");
}

void loop() {
  ArduinoCloud.update();
  
  if (millis() - wait > WAIT_THRESH)
  {
    wait = millis();
    
    // Read sensors
    humidity = dht.readHumidity();
    Serial.print(humidity);

    Serial.print(",");
    
    temperature = dht.readTemperature();
    Serial.print(temperature);

    Serial.print(",");

    luminosity = map(4095 - analogRead(PHOTOPIN), 0, 4095, 0, 100);
    Serial.print(luminosity);

    Serial.print("\n");
  }
}
