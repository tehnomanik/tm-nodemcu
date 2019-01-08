# nodeMCU webserver - code examples
Some of nodeMCU/esp8266 code examples one could find interesting for their own project.

# DISCLAIMER
Feel free to use all code but if something goes wrong don't blame me.

<h2>nodeMCU - wifiConfig.ino</h2>

Basic webserver WiFi configuration. Board boots in AP mode and user can connect to it and enter data for their home network. Then reboot nodeMCU board and it should connect to user defined WiFi network in STATIC IP CLIENT mode.
Requires flashing 2 times, first time with clearEEPROM(); function in setup to write default values in EEPROM. Then flash again without call to clearEEPROM function. 


<h2>nodeMCU - TemperatureSensorDS18b20.ino</h2>

Uses wifiConfig as a base and adds reading from DS18b20 temperature sensor using OneWire and DallasTemperature libraries. Root web page of nodeMCU is settings webpage where user can enter their WiFi information. Calling /getTemp URL (e.g. boardIP/getTemp) will return simple string with current temperature in format ##.## (22.55).

<h2>nodeMCU - TemperatureSensorAM2301.ino</h2>

Uses wifiConfig as a base and adds reading from AM2301 temperature sensor using DHT library. Root web page of nodeMCU is settings webpage where user can enter their WiFi information. Calling /getTemp URL (e.g. boardIP/getTemp) will return simple string with current temperature and humidity in format temp#humidity (e.g. 22.50#43.70).
