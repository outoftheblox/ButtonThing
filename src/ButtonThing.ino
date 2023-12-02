#include <Thing.h>
#include <BlinkPattern.h>
#include "Button.h"

#define BUTTON_PIN D3

using namespace ootb;

Thing thing;
Button button(BUTTON_PIN, true, 100);

BlinkPattern led(BUILTIN_LED);

BlinkPattern::Pattern<2> initialize{{1, 1}, 50};
BlinkPattern::Pattern<2> initialized{{1, 1}, 250};
BlinkPattern::Pattern<2> on{{39, 1}, 25};
BlinkPattern::Pattern<2> off{{1, 39}, 25};
BlinkPattern::Pattern<2> reqOn{{1, 3}, 250};
BlinkPattern::Pattern<2> reqOff{{3, 1}, 250};

bool feedbackState = false;

String stateTopic = "things/" + thing.clientId() + "/button/state";
String clickedTopic = "things/" + thing.clientId() + "/button/clicked";
String feedbackTopic = "things/" + thing.clientId() + "/button/feedback";

void setup()
{
    Serial.begin(230400);
    Serial.println();
    Serial.println("ClientID:" + thing.clientId());

    button.begin();

    led.begin();
    led.setPattern(initialize);

    thing.onStateChange([](const String &msg)
    {
        Serial.println(msg);
    });
    
    button.onChange([](){
        Value value = button.pressed();
        thing.publish(stateTopic, value);
        led.setPattern(button.pressed() ? reqOn : reqOff);
        Serial.println("Send " + String(button.pressed()));
        if (button.pressed())
        {
            thing.publish(clickedTopic, value);
        }
    });

    thing.begin();

    thing.addActuator(feedbackTopic, [](Value &value)
    {
        feedbackState = (bool)value;
        led.setPattern(feedbackState ? on : off);
        Serial.println("Received " + (String)value);
    });

    led.setPattern(initialized);
}

void loop()
{
    led.handle();
    button.handle();
    thing.handle();
}
