<h1>WIFI and SMTP web configuration</h1>

Uses <b>ESPMailer</b> library from https://github.com/ArduinoHannover/ESPMailer

Basic settings web page with wifi and smtp configurations.
Stores values in EEPROM. Press FLASH button on nodeMCU to clear all values in EEPROM and put board in AP mode so you can configure it on spot without reflashing. 
Needs to be flashed 2 times, first time with call to clearEEPROM() function in setup(). This will write default values to EEPROM. Then comment call to this function and reflash nodeMCU board. It will start in AP mode with "factory" settings. 
