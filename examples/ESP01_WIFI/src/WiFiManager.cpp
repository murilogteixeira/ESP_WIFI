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
    firstMillisConnecting = 0;
}

void WiFiManager::setupIp(IPAddress ip, IPAddress gateway, IPAddress subnet) {
    if(!WiFi.config(ip, gateway, subnet))
        Serial.println("\nCould not configure static IP.");
}

void WiFiManager::connectWithConfiguration() {
    Configuration config;
    config.begin();
    config.
}

void WiFiManager::connectStation(char *ssid, char *password, std::function<void ()> fn){
    Serial.printf("\nConnecting to %s", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    lastMillisConnection = millis();

    while(!WiFiManager::isConnected()) {
        WiFiManager::connecting();
        if(millis() - lastMillisConnection >= intervalToConnect) {
            if(millis() - firstMillisConnecting >= timeout) {
                WiFiManager::setupAccessPoint();
                return;
            }

            Serial.println("\nConnection failed. Reconnecting...");
            WiFiManager::connectStation(ssid, password, fn);
            return;
        }
    }

    Serial.printf("\nConnected! SSID: %s, IP: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
    fn();
}

void WiFiManager::setupAccessPoint() {
    Serial.println("\nConfiguring AccessPoint to setup connection...");

    char apName[] = "ESP8266_AP";
    WiFi.softAP(apName);
    MDNS.addService("http", "tcp", 80);
    Serial.printf("AP name: %s\n", apName);
    Serial.printf("MDNS: http://%s.local\n", "server");
    Serial.printf("IP: %s\n", WiFi.softAPIP().toString().c_str());
    setupServer();
}

bool WiFiManager::isConnected() {
    if(WiFi.getMode() == WIFI_AP_STA)  return true;
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::refreshConnection() {
    if(millis() - lastMillisConnection < intervalToConnect) return;
    if(!WiFiManager::isConnected()) WiFiManager::reconnect();
}

String getSSIDs() {
    int numberOfNetworks = WiFi.scanNetworks();
    String ssids = "";

    for(int i = 0; i < numberOfNetworks; i++) {
        ssids += WiFi.SSID(i);
        ssids += "\n";
    }

    return ssids;
}

void WiFiManager::setupServer() {
    // OTA
    server.on("/ota/update", HTTP_GET, [&]() { server.send(200, "text/html", uploadHtmlString.c_str()); });
    server.on("/ota/update", HTTP_POST, [&]() { ota.updateHandler(&server); }, [&]() { ota.uploadHandler(&server); });

    // CONFIG
    server.on("/", HTTP_GET, [&]() { server.send(200, "text/plain", getSSIDs().c_str()); });

    // 404
    server.onNotFound([&]() { server.send(404, "text/plain", "404: Not Found"); });

    server.begin();
}
