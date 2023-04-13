#include <Arduino.h>

class InputPin {
private:
    int pin;
    bool state;
public:
    InputPin(int pin, bool initialState);
    void on();
    void off();
    void toggle();
};