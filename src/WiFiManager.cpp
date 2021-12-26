#include <WiFiManager.h>

// Private methods
void WiFiManager::connecting() {
    if(millis() - lastMillisConnecting >= 200) {
        Serial.print(".");
        lastMillisConnecting = millis();
    }
}

// Public methods
WiFiManager::WiFiManager() {
    lastMillisConnection = 0;
    lastMillisConnecting = 0;
}

void WiFiManager::setupIp(IPAddress ip, IPAddress gateway, IPAddress subnet) {
    if(!WiFi.config(ip, gateway, subnet))
        Serial.println("\nCould not configure static IP.");
}

void WiFiManager::connectStation(char *ssid, char *password) {
    Serial.printf("\nConnecting to %s", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    lastMillisConnection = millis();

    while(!WiFiManager::isConnected()) {
        WiFiManager::connecting();
        if(millis() - lastMillisConnection >= intervalToConnect) {
            Serial.println("\nConnection failed. Reconnecting...");
            WiFiManager::connectStation(ssid, password);
            return;
        }
    }

    Serial.print("\nConnected. IP: ");
    Serial.print(WiFi.localIP());
}

void WiFiManager::reconnect() {
    if(millis() - lastMillisConnection < intervalToConnect) 
        return;

    Serial.print("\nReconnecting");
    WiFi.reconnect();
    lastMillisConnection = millis();

    while(!WiFiManager::isConnected())
        WiFiManager::connecting();

    Serial.print("\nConnected. IP: ");
    Serial.println(WiFi.localIP());
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}