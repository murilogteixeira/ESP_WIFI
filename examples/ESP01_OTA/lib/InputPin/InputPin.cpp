#include <InputPin.h>

InputPin::InputPin(int pin, bool initialState) {
    this->pin = pin;
    state = initialState;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
}

void InputPin::on() {
    state = HIGH;
    digitalWrite(pin, state);
}

void InputPin::off() {
    state = LOW;
    digitalWrite(pin, state);
}

void InputPin::toggle() {
    state = !state;
    digitalWrite(pin, state);
}