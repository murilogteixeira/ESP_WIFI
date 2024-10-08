#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

using namespace std;

class WiFiManager {
    private:
        unsigned long lastMillisConnection;
        unsigned long lastMillisConnecting;
        unsigned long intervalToConnect = 20000;
        unsigned long timeout = 60000;
        ESP8266WebServer server;
        void connecting();
        void reconnect();
        void setupAccessPoint();
        void setupServer();
    public:
        WiFiManager();
        bool isConnected();
        void setupIp(IPAddress ip, IPAddress gateway, IPAddress subnet);
        void connectStation(char *ssid, char *password, std::function<void ()> fn = [](){});
        void refreshConnection();
};