#include <Arduino.h>
#include <WiFiManager.h>
#include <OTA.h>
#include <Configuration.h>
#include <EspFile.h>
#include <Relay.h>

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

String host = "esp";
char ssid[] = "Murilo";
char password[] = "94658243";
// char ssid[] = "Maria Helena";
// char password[] = "gabrielldavy2011";
WiFiManager wifi;
OTA ota;
EspFile espFile;
Configuration config;
ESP8266WebServer server;

Relay relay(2, LOW);

void setupConfig();
void setupConnection();
void setupServer();

void setup() {
    Serial.begin(9600);
    Serial.println("\n\nBooting...");
    LittleFS.begin();
    setupConfig();
    setupConnection();
}

void loop() {
    server.handleClient();
    MDNS.update();
    wifi.refreshConnection();
}

void setupConfig() {
    config.begin();

    String espName = config.getEspName();

    if(espName != "null" && espName != "" && espName != NULL)
        host = espName;
    else 
        config.setEspName(host);
}

void setupConnection() {
    MDNS.begin(host);
    wifi.connectStation(ssid, password, []() {
        setupServer();
        MDNS.addService("http", "tcp", 80);
        Serial.printf("\nMDNS: http://%s.local\n", host.c_str());
    });
}

void setupServer() {
    // OTA
    server.on("/ota/update", HTTP_GET, []() { server.send(200, "text/html", espFile.getUploadFileHtml()); });
    server.on("/ota/update", HTTP_POST, []() { ota.updateHandler(&server); }, []() { ota.uploadHandler(&server); });

    // CONFIG
    server.on("/config/updateName", HTTP_GET, []() { config.updateNameHandler(&server); });

    // FILE
    server.on(espFile.getUploadPath(), HTTP_GET, []() { server.send(200, "text/html", espFile.getUploadFileHtml()); });
    server.on(espFile.getUploadPath(), HTTP_POST, []() { server.send(200); },[]() { espFile.uploadFileHandler(&server); });
    server.on(espFile.getDeletePath(), HTTP_GET, []() { espFile.deleteFileHandler(&server); });
    server.on(espFile.getListPath(), HTTP_GET, []() { espFile.listFileHandler(&server); });

    // RELAY
    server.on("/relay", []() { relay.toggleHandler(&server); });
    server.on("/relayOn", []() { relay.on(); server.send(200); });
    server.on("/relayOff", []() { relay.off(); server.send(200); });

    // 404
    server.onNotFound([]() { server.send(404, "text/plain", "404: Not Found"); });

    server.begin();
}