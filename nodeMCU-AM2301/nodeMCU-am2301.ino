// AM2301 temperature sensor, also works with other DHT sensors, just change type in code
// DATA pin from AM2301 (yellow wire) is connected to D5 or GPIO 14 pin on nodeMCU board
// AM2301 is DHT21 type 

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <DHT.h> // DHT library 1.2.3 (newer library returns error)

#define DHTTYPE DHT21 // dht type of sensor, change if you have other
#define DHTPIN 14 // D5 pin on nodeMCU board, GPIO pin 14 - connected DATA from sensor 

DHT dht(DHTPIN, DHTTYPE); // declare dht object to access sensor

float temperature = 0;
float humidity = 0;

int counter = 0;

const char* ssid = "name_of_your_network"; //SSID of wifi network
const char* password = "wifi_password"; // Password for wifi network
 
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

  // Starting Webserver
  server.begin();

  // define Webserver handles
  server.on("/getTemperature", handleGetTemperature); // returns simple string with current sensor temperature for eg 22.55
  server.on("/getHumidity", handleGetHumidity); // returns simple string with current sensor humidity for eg 48.85

  dht.begin(); // initialize sensor

  delay(2000);
}

void loop() {
  server.handleClient();
  delay(50);
  counter++;
  // Counter - set to read sensor values about every 5 seconds
  if(counter > 100) {
    readSensorValues();
    counter = 0;
  }
}

// WebServer handles:
void handleGetTemperature() {
  server.send(200, "text/html", String(temperature));
}

void handleGetHumidity() {
  server.send(200, "text/html", String(humidity));
}
// webServer handles END


// reads remperature and humidity from sensor and saves it in temp and humidity - global variables if both are numerical/number
void readSensorValues() {

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t))  {} // if either value is not numerical, do nothing
  else // else save data in global variables
  {
    temperature = t;
    humidity = h;
  }
}
