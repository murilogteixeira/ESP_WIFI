#include <Arduino.h>
#include <WiFiManager.h>

char ssid[] = "Murilo";
char password[] = "94658243";
WiFiManager wifiManager;

void setup() {
  Serial.begin(9600);
  wifiManager.connectStation(ssid, password);
}

void loop() {
  if(!wifiManager.isConnected()) 
    wifiManager.reconnect();
}