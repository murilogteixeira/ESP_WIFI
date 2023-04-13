#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <InputPin.h>

class Relay: public InputPin {
public:
    Relay(int pin, bool initialState): InputPin(pin, initialState){};
    void toggleHandler(ESP8266WebServer *server);
};