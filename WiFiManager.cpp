#include <WiFiManager.h>

// Private methods
void WiFiManager::connecting() {
    if(millis() - lastMillisConnecting >= 200) {
        Serial.print(".");
        lastMillisConnecting = millis();
    }
}

void WiFiManager::reconnect() {
    if(millis() - lastMillisConnection < intervalToConnect) return;

    Serial.print("\nReconnecting");
    WiFi.reconnect();
    lastMillisConnection = millis();

    while(!WiFiManager::isConnected())
        WiFiManager::connecting();

    Serial.print("\nConnected. IP: ");
    Serial.println(WiFi.localIP());
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

void WiFiManager::connectStation(char *ssid, char *password, std::function<void ()> fn){
    Serial.printf("\nConnecting to %s", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    lastMillisConnection = millis();

    while(!WiFiManager::isConnected()) {
        WiFiManager::connecting();
        if(millis() - lastMillisConnection >= intervalToConnect) {
            Serial.println("\nConnection failed. Reconnecting...");
            WiFiManager::connectStation(ssid, password, fn);
            return;
        }
    }

    Serial.print("\nConnected! SSID: %s, IP: %s\n", ssid, WiFi.localIP());
    Serial.print(WiFi.localIP());
    fn();
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::refreshConnection() {
    if(millis() - lastMillisConnection < intervalToConnect) return;
    if(!WiFiManager::isConnected()) WiFiManager::reconnect();
}
