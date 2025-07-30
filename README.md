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

## How to run

First, host the web files (volunteer.html and backend.js). The best way to do this is probably to use a web hosting service, but I ended up using a temporary measure. Here is what I did:
* I installed ngrok.
* I ran the command "node backend.js" in the console (after cd'ing to the director where the web files were).
* I opened volunteer.html in my browser.
* I used this command in ngrok: ngrok http --scheme=http http://localhost:3000
  * Note: has to be http and not https because the microcontroller only works with http.
  * I used 3000 because that is also used in the code.

Next, download the Arduino files (nest.ino and wifi_strings.h). Edit wifi_strings.h to contain your WiFi SSID, your WiFi password, and the host name you are using (for me an ngrok URL). Then upload nest.ino onto the Arduino board.

## How to use

After flashing the microcontroller, it will take several seconds before it connects to the WiFi. Once it does, the multi-colored LED will briefly flash white, and the device will be usable (i.e. you can now press buttons).
* The buttons in the top row from left-to-right are cooking and shopping.
* The buttons in the middle row from left-to-right are morning and afternoon.
* The button in the bottom row is the submit button.
After making a request and submitting, go to the webpage that uses volunteer.html. You will be able to see the request you sent, and now have the option to accept or decline it. If you accept, the multi-colored LED will turn green for three seconds and then turn purple; in order to use the device again, you must replug it. (Ideally, there would be a "cancel request" button one could press to regain control of the device, but I ran out of GPIO pins.) If you decline, the multi-colored LED will turn red for three seconds and then the board will be usable again (i.e. you can make another request). Ideally, there would be other LEDs to indicacte success or failure and the multi-colored LED should just be restricted to indicating which volunteer agreed, but I had limited GPIO pins.
When you are finished, unplug the device.
