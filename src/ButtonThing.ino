#include <Thing.h>
#include <BlinkPattern.h>

#define BUTTON_PIN D3

using namespace ootb;

Thing thing;
BlinkPattern led(BUILTIN_LED);

BlinkPattern::Pattern<2> initialize{{1,1},50};
BlinkPattern::Pattern<2> on{{39,1},25};
BlinkPattern::Pattern<2> off{{1,39},25};
BlinkPattern::Pattern<2> reqOn{{1,3},250};
BlinkPattern::Pattern<2> reqOff{{3,1},250};

bool state = true;
bool buttonState = false;
bool feedbackState = false;
long nextSwitch = 0;

void setup()
{
  Serial.begin(230400);
  Serial.println();

  Serial.println("ClientID:" + thing.clientId());

  led.begin();

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  led.setPattern(initialize);
  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  thing.begin();

  //thing.addSensor(thing.clientId() + "/button/state", 1000, [](Value& value){
  //  value = state;
  //});

  thing.addActuator("button/feedback/" + thing.clientId(), [](Value& value){
    Serial.println("Got " + (String)value);
    feedbackState = (bool)value;
    state = feedbackState;
    //digitalWrite(BUILTIN_LED, !feedbackState);

    if(feedbackState)
      led.setPattern(on);
    else
      led.setPattern(off);
  });
}

void loop()
{
  handle();
  led.handle();
  thing.handle();
}

void handle()
{
  bool pushed = !digitalRead(BUTTON_PIN);
  if (pushed && pushed != buttonState && millis() > nextSwitch)
  {

    nextSwitch = millis() + 500;
    state = !state;
    if(state)
      led.setPattern(reqOn);
    else
      led.setPattern(reqOff);

    Value value = state;
    thing.publish("button/state/" + thing.clientId(), value);
  }
  buttonState = pushed;
}
