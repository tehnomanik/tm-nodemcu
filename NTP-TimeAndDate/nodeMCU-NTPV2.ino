#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ezTime.h>

Timezone myLocalTime; 

const char* ssid = "your_wifi_name"; //SSID of wifi network
const char* password = "your_wifi_password"; // Password for wifi network
 
// config static IP
IPAddress ip(192, 168, 1, 100); // where xx is the desired IP Address
IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

ESP8266WebServer server(80);

void setup() {
  // put your setup code here, to run once:
 
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

  // Webserver handles
  server.on("/", handleRoot);

  // Set desired time zone for Timezone object declared in the beginning
  myLocalTime.setLocation(F("de")); // set your time zone

  // Sync NTP time for ezTime library
  waitForSync(); 

  delay(2000);
}

void loop() {
  server.handleClient();
  delay(50);
}


// WebServer handles:
void handleRoot() {
  server.send(200, "text/html", getDefaultPage());
}
// webServer handles END

// Default page
String getDefaultPage() {
   
  String page = "<!DOCTYPE html>\n\n<html>\n\n<head>\n    <title>nodeMCU</title>\n    <meta charset=\"utf-8\">\n</head>\n\n<body>\n    <h1>ezTIME Library example</h1>\n    <h2>Time: ##time##</h2>\n    <h2>Date: ##date##</h2>\n</body>\n\n</html>";
  
  page.replace("##time##", myLocalTime.dateTime("H:i:s"));
  page.replace("##date##", myLocalTime.dateTime("d.m.Y"));
  
  return page;
}

// End Default page
