// Note: some of this code originated from this tutorial: http://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples/

#include "wifi_strings.h"

#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino.h>
#include <ArduinoJson.h>

const int button1Pin = 2;
const int button2Pin = 4;
const int button3Pin = 6;
const int button4Pin = 8;
const int button5Pin = 13;
const int led1Pin = 3;
const int led2Pin = 5;
const int led3Pin = 7;
const int led4Pin = 12;
const int led5RedPin = 9;
const int led5BluePin = 10;
const int led5GreenPin = 11;

int button1State = 0;
int button1Toggle = 0;
int led1State = 0;
int button2State = 0;
int button2Toggle = 0;
int led2State = 0;
int button3State = 0;
int button3Toggle = 0;
int led3State = 0;
int button4State = 0;
int button4Toggle = 0;
int led4State = 0;
int button5State = 0;
int button5Toggle = 0;
int button5Success = 0;

const char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;
int status = WL_IDLE_STATUS;
int HTTP_PORT = 80;
String HTTP_METHOD = "POST";
char HOST_NAME[] = BACKEND_HOST_NAME;
String PATH_NAME = "/appointment";

const int enable_wifi = 1;

void setup() {
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(button4Pin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
  pinMode(led5RedPin, OUTPUT);
  pinMode(led5BluePin, OUTPUT);
  pinMode(led5GreenPin, OUTPUT);

  Serial.begin(9600);

  if (enable_wifi == 1) {
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("Communication with WiFi module failed!");
      // don't continue
      while (true)
        ;
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
      Serial.println("Please upgrade the firmware");
    }

    // attempt to connect to WiFi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);

      // wait 10 seconds for connection:
      delay(10000);
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    setLed5Color(255, 255, 255);
    delay(1000);
    setLed5Color(0, 0, 0);
  }
}

void loop() {
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  button4State = digitalRead(button4Pin);
  button5State = digitalRead(button5Pin);

  if (button1State == HIGH) {
    if (button1Toggle == 0) {
      if (led1State == 0) {
        digitalWrite(led1Pin, HIGH);
        led1State = 1;
      } else {
        digitalWrite(led1Pin, LOW);
        led1State = 0;
      }
    }
    button1Toggle = 1;
  } else {
    button1Toggle = 0;
  }

  if (button2State == HIGH) {
    if (button2Toggle == 0) {
      if (led2State == 0) {
        digitalWrite(led2Pin, HIGH);
        led2State = 1;
      } else {
        digitalWrite(led2Pin, LOW);
        led2State = 0;
      }
    }
    button2Toggle = 1;
  } else {
    button2Toggle = 0;
  }

  if (button3State == HIGH) {
    if (button3Toggle == 0) {
      if (led3State == 0) {
        digitalWrite(led3Pin, HIGH);
        led3State = 1;

        digitalWrite(led4Pin, LOW);
        led4State = 0;
        button4Toggle = 0;
      } else {
        digitalWrite(led3Pin, LOW);
        led3State = 0;
      }
    }
    button3Toggle = 1;
  } else {
    button3Toggle = 0;
  }

  if (button4State == HIGH) {
    if (button4Toggle == 0) {
      if (led4State == 0) {
        digitalWrite(led4Pin, HIGH);
        led4State = 1;

        digitalWrite(led3Pin, LOW);
        led3State = 0;
        button3Toggle = 0;
      } else {
        digitalWrite(led4Pin, LOW);
        led4State = 0;
      }
    }
    button4Toggle = 1;
  } else {
    button4Toggle = 0;
  }

  if (button5State == HIGH && button5Toggle == 0 && button5Success == 0 && (led1State == 1 || led2State == 1) && (led3State == 1 || led4State == 1) && enable_wifi == 1) {
    button5Toggle = 1;

    const int cooking = led1State;
    const int shopping = led2State;
    const int morning = led3State;
    const int afternoon = led4State;
    int time = 0;
    if (afternoon == 1) {
      time = 1;
    }

    digitalWrite(led1Pin, LOW);
    led1State = 0;
    button1Toggle = 0;

    digitalWrite(led2Pin, LOW);
    led2State = 0;
    button2Toggle = 0;

    digitalWrite(led3Pin, LOW);
    led3State = 0;
    button3Toggle = 0;

    digitalWrite(led4Pin, LOW);
    led4State = 0;
    button4Toggle = 0;

    setLed5Color(255, 255, 255);
    
    char* volunteer_first_name;
    char* volunteer_last_name;

    // connect to web server on port 80:
    if (client.connect(HOST_NAME, HTTP_PORT)) {
      String body = "{\n\"first_name\": \"Marie\",\n\"last_name\": \"Schmidt\",\n\"time\": "+String(time)+",\n\"day\": 0,\n\"cooking\": "+String(cooking)+",\n\"shopping\": "+String(shopping)+",\n\"childcare\": 0,\n\"emotional_support\": 0,\n\"laundry\": 0,\n\"cleaning\": 0,\n\"gardening\": 0,\n\"repair\": 0\n}\n";
      // if connected:
      Serial.println("Connected to server");
      // make a HTTP request:
      // send HTTP header
      client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("ngrok-skip-browser-warning: true");
      client.println("Content-Type: application/json");
      client.println("Content-Length: " + String(body.length()));
      client.println("Connection: close");
      client.println();  // end HTTP header
      client.print(body);

      String reply = "";
      while (client.connected()) {
        if (client.available()) {
          // read an incoming byte from the server and print it to serial monitor:
          char c = client.read();
          reply += c;
        }
      }
      char *str = reply.c_str();
      char *delimiter = "\n\r\n";
      char *start = str;
      char *end;

      while ((end = strstr(start, delimiter)) != NULL) {
          *end = '\0';
          start = end + strlen(delimiter);
      }

      const int capacity = JSON_OBJECT_SIZE(3);
      DynamicJsonDocument doc(capacity);

      char *json = start;
      deserializeJson(doc, json);

      const bool doc_accepted = doc["accepted"];
      const char* doc_first_name = doc["first_name"];
      const char* doc_last_name = doc["last_name"];
      volunteer_first_name = doc_first_name;
      volunteer_last_name = doc_last_name;
      if (doc_accepted == true) {
        button5Success = 1;
      }

      // the server's disconnected, stop the client:
      client.stop();
      Serial.println();
      Serial.println("disconnected");
    } else {  // if not connected:
      Serial.println("connection failed");
      button5Success = 0;
    }

    if (button5Success == 1) {
      setLed5Color(0, 255, 0);
      delay(3000);
      if (strcmp(volunteer_first_name, "Ava") == 0 && strcmp(volunteer_last_name, "Fischer") == 0) {
        setLed5Color(255, 0, 255);
      } else {
        setLed5Color(0, 255, 255);
      }
      button5Success = 1;
    } else {
      setLed5Color(255, 0, 0);
      delay(3000);
      setLed5Color(0, 0, 0);
    }
    button5Toggle = 0;
  }

  if (enable_wifi == 0) {
    if (button5State == HIGH) {
      setLed5Color(255, 255, 255);
    } else {
      setLed5Color(0, 0, 0);
    }
  }
}

void setLed5Color(int redValue, int greenValue,  int blueValue) {
  analogWrite(led5RedPin, redValue);
  analogWrite(led5GreenPin, greenValue);
  analogWrite(led5BluePin, blueValue);
}
