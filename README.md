# NEST

## Requirements

### Web
* Node.js
* Express.js (npm module)
* WebSocket (npm module)

### Arduino
* Arduino Uno WiFi Rev2 (microcontroller board)
* Arduino IDE
* WiFiNINA (Arduino library)
* ArduinoJson (Arduino library)

## How to setup

First, host the web files (volunteer.html and backend.js). The best way to do this is probably to use a web hosting service, but I ended up using a temporary measure. Here is what I did:
* I installed ngrok.
* I ran the command "node backend.js" in the console (after cd'ing to the director where the web files were).
* I opened volunteer.html in my browser.
* I used this command in ngrok: ngrok http --scheme=http http://localhost:3000
  * Note: has to be http and not https because the microcontroller only works with http.
  * I used 3000 because that is also used in the code.

Next, download the Arduino files (nest.ino and wifi_strings.h). Edit wifi_strings.h to contain your WiFi SSID, your WiFi password, and the host name you are using (for me an ngrok URL). Then upload nest.ino onto the Arduino board.

## How to use

placeholder
