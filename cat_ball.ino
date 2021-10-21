#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "Cat Ball"
#define APPSK  "churchill"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

const int ledPin = 2;
const int motorPin = 1;
const int sensorPin = 3;
bool motorStatus = true;
bool manualControl = false;

const int onTime = 1000*2;
const int offTime = 1000*60;
const int liveTime = 1000*60*5;
unsigned long ticker = millis();
unsigned long touchedAt = millis() - liveTime;

String createHtml() {
  String res = "<!DOCTYPE html><html>\n";
  res += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  res += "<title>Cat Ball</title>\n";
  res += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  res += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  res += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 7px;}\n";
  res += ".button-off {background-color: #1abc9c;}\n";
  res += ".button-off:active {background-color: #16a085;}\n";
  res += ".button-on {background-color: #34495e;}\n";
  res += ".button-on:active {background-color: #2c3e50;}\n";
  res += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  res += "</style>\n";
  res += "</head>\n";
  res += "<body>\n";
  res += "<h1>Cat Ball</h1>\n";
  
  if (manualControl) {
    res += "<a class=\"button button-on\" href=\"/manual\">Disable manual mode</a>\n";
    res += "<a class=\"button\" href=\"/motoron\">Turn motor on</a>\n";
    res += "<a class=\"button\" href=\"/motoroff\">Turn motor off</a>\n";
  } else {
    res += "<a class=\"button button-off\" href=\"/manual\">Enable manual mode</a>\n";
  }

  res += "</body>\n";
  res += "</html>\n";

  return res;
}

void handleRoot() {
  server.send(200, "text/html", createHtml());
}

void manual() {
  manualControl = !manualControl;
  motorStatus = false;
  server.send(200, "text/html", createHtml());
}

void motorOn() {
  motorStatus = true;
  server.send(200, "text/html", createHtml());
}

void motorOff() {
  motorStatus = false;
  server.send(200, "text/html", createHtml());
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/manual", manual);
  server.on("/motoron", motorOn);
  server.on("/motoroff", motorOff);
  server.begin();
}

void loop() {
  server.handleClient();
  if (manualControl) {
    digitalWrite(motorPin, motorStatus);
  } else {
    if (millis()-touchedAt < liveTime) { // If touched less than 5 minutes ago then act alive
      if (motorStatus) {
        if (millis()-ticker > onTime) {
          ticker = millis();
          motorStatus = false;
          digitalWrite(motorPin, false);
        }
      } else {
        if (millis()-ticker > offTime) {
          ticker = millis();
          motorStatus = true;
          digitalWrite(motorPin, motorStatus);
        }
      }
    } else {
      motorStatus = false;
      digitalWrite(motorPin, false);
      digitalWrite(ledPin, true);
      
      if (digitalRead(sensorPin)) {
        touchedAt = millis();
        digitalWrite(ledPin, false);
      }
    }
  }
}
