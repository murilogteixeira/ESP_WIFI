#include <Relay.h>

void Relay::toggleHandler(ESP8266WebServer *server) {
    String filename = "relay.html";
    File file = LittleFS.open(filename, "r");
    if(file) server->send(200, "text/html", file.readString());
    else server->send(404, "text/plain", "File relay.html not exist");
    String state = server->arg("state");
    if(state == "toggle") toggle(); 
}