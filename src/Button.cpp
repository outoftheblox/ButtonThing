#include "Button.h"
#include "FunctionPointer.h"
#include <Arduino.h>

using namespace ootb;
using namespace std;

Button::Button(uint8_t pin, bool invert)
:
    pin(pin),
    invert(invert)
{
}

Button::~Button()
{
}

void Button::onChange(std::function<void()> function)
{
    onChangeHandler = function;
}

void Button::handle()
{
    bool current = !digitalRead(pin);
    unsigned long now = millis();
    if (current != pushed && now - lastChange > debounceDelay)
    {
        lastChange = now;
        changed = true;
        pushed = current;
    }
    if (!changed) return;
    if (!onChangeHandler) return;
    onChangeHandler();
    changed = false;
}

void Button::setup()
{
    pinMode(pin, INPUT_PULLUP);
    lastChange = millis() - debounceDelay;
}

bool Button::isPushed()
{
    return pushed;
}

