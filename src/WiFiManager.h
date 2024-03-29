#include <ESP8266WiFi.h>

using namespace std;

class WiFiManager {
    private:
        unsigned long lastMillisConnection;
        unsigned long lastMillisConnecting;
        unsigned long intervalToConnect = 20000;
        unsigned long timeout = 60000;
        void connecting();
        void reconnect();
        void setupAccessPoint();
    public:
        WiFiManager();
        bool isConnected();
        void setupIp(IPAddress ip, IPAddress gateway, IPAddress subnet);
        void connectStation(char *ssid, char *password, std::function<void ()> fn = [](){});
        void refreshConnection();
};