#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <OTA.h>
#include <EspFile.h>

using namespace std;

class WiFiManager {
    private:
        unsigned long lastMillisConnection;
        unsigned long lastMillisConnecting;
        unsigned long firstMillisConnecting;
        unsigned long intervalToConnect = 20000;
        unsigned long timeout = 60000;
        String uploadHtmlString = "<form method=\"post\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"name\"><input class=\"button\" type=\"submit\" value=\"Upload\"></form>";
        OTA ota;

        void connecting();
        void reconnect();
        void setupAccessPoint();
        void setupServer();
    public:
        WiFiManager();
        ESP8266WebServer server;
        bool isConnected();
        void setupIp(IPAddress ip, IPAddress gateway, IPAddress subnet);
        void connectWithConfiguration();
        void connectStation(char *ssid, char *password, std::function<void ()> fn = [](){});
        void refreshConnection();
};