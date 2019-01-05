# nodeMCU code examples
Some of nodeMCU/esp8266 code examples one could find interesting for their own project.

# DISCLAIMER
Feel free to use all code but if something goes wrong don't blame me.

<strong>wifiConfig.ino</strong>
Basic webserver WiFi configuration. Board boots in AP mode and user can connect to it and enter data for their home network. Then reboot nodeMCU board and it should connect to user defined WiFi network in STATIC IP CLIENT mode.
Requires flashing 2 times, first time with clearEEPROM(); function in setup to write default values in EEPROM. Then flash again without call to clearEEPROM function. 
