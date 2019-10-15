# TCSintercomArduino

Here are the sketches for the following youtube video.

The Different sketches can be used like this: 


### OnlySendOneBell: Send one call command to a defined serial number, see video demo.

### SendCallToSpecificSerialNr: You can simply enter the serial nr. you want to call via "normal" numbers and it will calculate the cmd for you.

### TCSparserUNO: use external comparator on Interrupt pin D2, can be used with TCS software

### TCSparserUNOMitCOMP: use internal comparator of Arduino UNO on pin D6 and D7, can be used with TCS software.

### AnalogINparserESP8266: Parse incoming Bus messages via Analog in without interrupt on ESP8266/ not as reliable as the interrupt ones because the AnalogIn needs to be called as much as possible.

For all sketches see the Video for Schmeatic.

Happy Intercom Hacking!


[![YoutubeVideo](https://img.youtube.com/vi/xFLoauqj9yA/0.jpg)](https://www.youtube.com/watch?v=xFLoauqj9yA)


See this Repo for another aproach from another guy who is using the TCS intercom: https://github.com/Syralist/tcs-monitor
