// Example of simple Web UI to display data from nodeMCU with button to turno onboard LED on or off
// ezTime library for getting time/date from online NTP servers
// DHT library for AM2301(DHT21) temperature and humidity sensor connected to D5 pin

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ezTime.h>                       // ezTime library for NTP time sync
#include <DHT.h>                          // DHT library 1.2.3 (newer library returns error)

#define DHTTYPE DHT21                     // dht type of sensor, change if you have other
#define DHTPIN 14                         // D5 pin on nodeMCU board, GPIO pin 14 - connected DATA from sensor 
DHT dht(DHTPIN, DHTTYPE);                 // declare dht object to access sensor

float temperature = 0; // for storing temperature from sensor
float humidity = 0; // for storing humidity from sensor

int counter = 0; // counter for tracking intervals for reading data from sensor

#define ledPin D4 // onboardLED connected to D4
bool ledState = false; // tracking LED state, starts turned off

Timezone myLocalTime; // ezTime lib object for tracking and syncing NTP time

// wifi connection data
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
   
  pinMode(ledPin, OUTPUT); // set onboard led pin to output
   
  // Starting Webserver
  server.begin();

  // define Webserver handles
  server.on("/", handleRoot); // root page
  server.on("/lighttoggle", handleLightToggle); // command for switching onboard LED on or off

  // Set desired time zone for Timezone object declared in the beginning
  myLocalTime.setLocation(F("de")); // set your time zone

  // Sync NTP time for ezTime library
  waitForSync(); 

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
  digitalWrite(ledPin, !ledState); // esp8266 is active low thats why we use inverse of state
}

// WebServer handles:
void handleRoot() {
  server.send(200, "text/html", getDefaultPage()); // returns root webpage
}

void handleLightToggle() {
  ledState = !ledState;
  server.sendHeader("Location", String("/"), true); // change ledState and return root web page
  server.send(302, "text/plain", "");
}

// webServer handles END


// Default page, root page
String getDefaultPage() {

  // HTML code as C string
  String page = "<!DOCTYPE html>\n<html>\n<head>\n    <title>nodeMCU minWebUI</title>\n    <meta charset=\"utf-8\" />\n    <meta name=\"viewport\" content=\"width=600, user-scalable=no\" />\n\n    <style>\n        body {\n            background-color: #fff;\n            margin: auto;\n            font-family: sans-serif;\n            font-weight: bold;\n            text-decoration: none !important;\n            text-align: center;\n        }\n\n        a {\n            text-decoration: none;\n        }\n\n        .container {\n            display: block;\n            width: 600px;\n            clear: both;\n            margin: auto;\n        }\n\n        /* ROWS */\n        .row {\n            display: block;\n            height: 80px;\n            clear: both;\n            vertical-align: middle;\n            line-height: 80px;\n            border-bottom: 2px solid #1a1a1a;\n            overflow: hidden;\n            font-size: 30px;\n        }\n\n        .row-sm {\n            height: 60px;\n            line-height: 60px;\n            font-size: 24px;\n        }\n\n        .row-lg {\n            height: 120px;\n            line-height: 120px;\n            font-size: 36px;\n        }\n\n        /* CELLS */\n\n        .cell-lg {\n            width: 600px;\n            float: left;\n        }\n\n        .cell-md {\n            width: 400px;\n            float: left;\n        }\n\n        .cell-sm {\n            width: 200px;\n            float: left;\n            text-align: center;\n        }\n\n        .indicator {\n            display: block;\n            width: 60px;\n            float: left;\n            height: 100%;\n            text-align: center;\n        }\n\n        .btn {\n            width: 180px;\n            height: 100%;\n            float: left;\n            font-size: 24px;\n            font-weight: bold;\n            border-left: 10px solid #000;\n            border-right: 10px solid #000;\n            background-color: #0094ff;\n            color: white;\n        }\n\n        /*colors and backrounds*/\n        .bg-info {\n            background-color: #e6e6e6;\n            color: #1a1a1a;\n        }\n\n        .bg-data {\n            background-color: #0c336e;\n            color: white;\n        }\n\n        .bg-title {\n            background-color: #4e4e4e;\n            color: white;\n        }\n\n        /* indicator styles */\n        .on {\n            background-color: #59e430;\n            color: white;\n        }\n\n        .off {\n            color: white;\n            background-color: #ff0000;\n        }\n        /* Empty Cell */\n        .empty {\n            background-color: #fff;\n            color: #fff;\n        }\n    </style>\n\n</head>\n<body>\n\n    <div class=\"container\">\n        <div class=\"row row-lg bg-title\">EXAMPLE</div>\n        <div class=\"row\">\n            <div class=\"cell-md bg-info\">Current time</div>\n            <div class=\"cell-sm bg-data\">##time##</div>\n        </div>\n        <div class=\"row row-sm\">\n            <div class=\"cell-md bg-info\">Date</div>\n            <div class=\"cell-sm bg-data\">##date##</div>\n        </div>\n        <div class=\"row row-sm\">\n            <div class=\"cell-md bg-info\">IP Address</div>\n            <div class=\"cell-sm bg-data\">##ip##</div>\n        </div>\n        <div class=\"row row-sm\">\n            <div class=\"cell-md bg-info\">Wireless network</div>\n            <div class=\"cell-sm bg-data\">##wifi##</div>\n        </div>\n        <div class=\"row row-sm\">\n            <div class=\"cell-md bg-info\">Temperature</div>\n            <div class=\"cell-sm bg-data\">##temperature## C</div>\n        </div>\n        <div class=\"row row-sm\">\n            <div class=\"cell-md bg-info\">Humidity</div>\n            <div class=\"cell-sm bg-data\">##humidity## %</div>\n        </div>\n        <div class=\"row row-sm\">\n            <div class=\"cell-md bg-info\">\n                <div class=\"indicator ##indicator-light##\">¤</div>\n                Onboard LED\n            </div>\n            <a href=\"/lighttoggle\" class=\"btn\">SWITCH</a>\n        </div>\n     </div>\n\n</body>\n</html>\n";
  
  page.replace("##time##", myLocalTime.dateTime("H:i:s")); // replace placeholder with current time from NTP - ezTime lib
  page.replace("##date##", myLocalTime.dateTime("d.m.Y")); // replace placeholder with current date from NTP - ezTime lib
  page.replace("##temperature##", String(temperature)); // replace placeholder with current temperature from AM2301 sensor
  page.replace("##humidity##", String(humidity)); // replace placeholder with current humidity from AM2301 sensor

  // Get current IP address and replace placeholder in HTML page
  IPAddress wifiIP = WiFi.localIP();
  // String ipaddr = wifiIP[0] + "." + wifiIP[1];// + "." + wifiIP[2] + "." + wifiIP[3];
  page.replace("##ip##", String(wifiIP[0]) + "." + String(wifiIP[1]) + "." + String(wifiIP[2]) + "." + String(wifiIP[3]));
  
  page.replace("##wifi##", WiFi.SSID()); // replace placeholder with name of wifi network nodeMCU is connected to

  // Set indicator for onboard LED in HTML page depending if its on or off
  if (ledState)
  {
    page.replace("##indicator-light##", " on");
  }
  else
  {
    page.replace("##indicator-light##", " off");
  }
  
  return page; // return HTML page filled with current data
}

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
