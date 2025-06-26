#include <Arduino.h>
#include <timer.h>
#include <motor.h>

// #include <Adafruit_NeoPixel.h>

#define mode_changepin 36

#define TEST 32
#define kicker 30

int kick_time;
int kick_test_time;
int mode_change;

bool KICK_fin = true;

void setup()
{

  pinMode(mode_changepin, INPUT_PULLDOWN);
  pinMode(TEST, OUTPUT);
  pinMode(kicker, OUTPUT);

  Serial.begin(115200);

  Serial1.begin(115200); // motor
  Serial1.setTimeout(10);

  Serial2.begin(115200); // gyro
  Serial2.setTimeout(10);

  Serial3.begin(115200); // camera
  Serial3.setTimeout(10);

  Serial4.begin(115200); // IR
  Serial4.setTimeout(10);

  Serial5.begin(115200); // line
  Serial5.setTimeout(10);
}
void loop()
{
  
  digitalWrite(TEST, HIGH);

  mode_change = digitalRead(mode_changepin);

  if (mode_change == 1 && KICK_fin)
  {
    KICK_TEST.start();
    KICK_TEST.tick();
    kick_test_time = KICK_TEST.get_value();
    if (kick_test_time >= 500 && kick_test_time <= 600)
    {
      digitalWrite(TEST, LOW);
      digitalWrite(kicker, HIGH);
      delay(30);
      digitalWrite(kicker, LOW);
      KICK_TEST.reset();
      kick_test_time = KICK_TEST.get_value();
      KICK_fin = false;
    }
  }
  else if (mode_change == 0 && KICK_fin == false)
  {
    KICK_fin = true;
  }

  move(0, 0, 0, 0);

  Serial.print("modechange ");
  Serial.print(mode_change);
  Serial.print("   ");
  Serial.print("\t");

  Serial.print(kick_test_time);
  Serial.print("   ");
  Serial.print("\t");

  Serial.print("kick_time ");
  Serial.print(kick_time);
  Serial.print("   ");
  Serial.print("\t");

  Serial.println("\t");
}
