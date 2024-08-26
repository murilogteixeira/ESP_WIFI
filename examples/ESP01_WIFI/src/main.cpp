#include <Arduino.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <ESP8266WebServer.h>

char ssid[] = "MURILO";
char password[] = "94658243";
WiFiManager wifiManager;

void setup() {
  Serial.begin(9600);
  wifiManager.connectStation(ssid, password);
}

void loop() {
  // if(!wifiManager.isConnected()) 
  //   wifiManager.refreshConnection();
  
  wifiManager.server.handleClient();
}