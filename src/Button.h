#ifndef _Button_H
#define _Button_H

#include <functional>

namespace ootb
{

    class Button
    {
        private:
            uint8_t pin;

            unsigned long debounceDelay = 200;
            unsigned long lastChange;

            bool changed;
            bool invert;
            bool pushed = false;

            std::function<void(void)> onChangeHandler;

        public:
            Button(uint8_t pin, bool invert = false);
            ~Button();

            void onChange(std::function<void()> function);

            void setup();
            void handle();
            bool isPushed();

        private:
            void isr();
    };
}

#endif
