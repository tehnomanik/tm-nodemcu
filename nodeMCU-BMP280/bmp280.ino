// BMP280 Pressure sensor connected to I2C bus
// If board doesn't start, BMP280 sensor probably uses alternative I2C bus address
// Default address is 0x77, but some sensors use 0x76 which is defined as alternative address in library
// If that is the case just start sensor with alternative method -> bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)
// ======   Connections:  ============
// VCC => 3.3V or 5V
// GND => GND
// SCL => D4 (GPIO 2)
// SDA => D3 (GPIO 0)
// CSB => not connected
// SDD => not connected
// ====================================
// Uses Adafruit Unified Sensor library and Adafruit BMP280 sensor library
// If readings are incorrect, connect CSB to VCC for a second to reset I2C bus

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

Adafruit_BMP280 bmp; // declare bmp280 pressure sensor object

// for altitude conversion, needs current and accurate pressure reading at sea level for your region
// precision of altitude will depend of this value
float altitude_base = 1015.25;
 
float temperature = 0; // holds temperature reading from sensor
float pressure = 0; // holds pressure reading from sensor
float altitude = 0; // holds altitude reading from sensor

int counter = 0; // counter for periodically checking temperature

const char* ssid = "your_wifi_name"; //SSID of wifi network
const char* password = "your_wifi_password"; // Password for wifi network
 
// config static IP
IPAddress ip(192, 168, 1, 100); // where xx is the desired IP Address
IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

ESP8266WebServer server(80);

void setup() {
  // WiFi configuration
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  WiFi.config(ip, gateway, subnet, gateway);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // BMP connected SDA => D3(0), SCL => D4(2)
  // I2C bus setup
  Wire.begin(0,2);
  Wire.setClock(100000);

  // Connecting to BMP280 sensor, if not connected it will loop indefinitivly
  // If your sensor uses alternative 0x76 address use following line instead of just (!bmp.begin())
  //if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    // Serial.println("ERROR: Cannot start pressure sensor...");
    while (1);
  }

  // Starting Webserver
  server.begin();

  // define Webserver handles
  server.on("/getTemperature", handleGetTemperature); // returns simple string with current sensor temperature for eg 22.55
  server.on("/getPressure", handleGetPressure); // returns simple string with current sensor humidity for eg 48.85
  server.on("/getAltitude", handleGetAltitude); // returns simple string with current sensor humidity for eg 48.85
  
  delay(2000);
}

void loop() {
  server.handleClient();
  delay(50);
  // Counter - set to read temperature about every 5 seconds, 100 counts = 5 seconds
  counter++;
  if(counter > 100) {
    getSensorValues(); // call function getTemp() to get readings from sensor
    counter = 0; // reset counter
 }    
}

// WebServer handles:
void handleGetTemperature() {
  server.send(200, "text/html", String(temperature));
}

void handleGetPressure() {
  server.send(200, "text/html", String(pressure));
}

void handleGetAltitude() {
  server.send(200, "text/html", String(altitude));
}
// webServer handles END


// reads remperature from sensor and saves it in temp - global variable if it's numerical number
void getSensorValues() {

  // read values from sensor
  float t = bmp.readTemperature();
  float p = bmp.readPressure();
  float a = bmp.readAltitude(altitude_base); // aproximate altitude based on current pressure at sea level for your region
  

  // check if all values from sensor are in correct number format
  if (!isnan(t) && !isnan(p) && !isnan(a)){
    // assign latest values to global variables
    temperature = t;
    pressure = p;
    altitude = a;    
  }
}
