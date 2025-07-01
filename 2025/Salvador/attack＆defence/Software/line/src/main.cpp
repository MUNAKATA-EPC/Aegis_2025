#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <timer.h>
#include <average.h>

#define PIN 8
#define NUMPIXELS 6

#define center 9
#define right 5
#define left 1
#define back 3
#define left_center 0
#define right_center 10
#define left_back 2
#define right_back 4

int brightness = 255; // 明るさ
int line_dec = 0;
int line_center_val;
int line_right_val;
int line_left_val;
int line_back_val;
int right_center_val;
int left_center_val;
int right_back_val;
int left_back_val;

int line_deep_val;
int line_side_val;

int rainbowtime;
int neopixelright;
int neopixelcolor;

bool time_stop = false;
bool neopixel = true;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // neo pixel object that is used to control the LED strip

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait)
{
  if (neopixel == true)
  {
    for (int j = 0; j < 256; ++j)
    {                 // iterate over all pixels
      pixels.clear(); // すべてのLEDの色を0にセット//いじる必要なし
      neopixelcolor = j;
      delay(wait);
      for (int i = 0; i < NUMPIXELS; ++i)
      {
        neopixelright = i;
        pixels.setPixelColor(neopixelright, pixels.Color(neopixelcolor, 0, neopixelcolor)); // set color of pixel rgb
        pixels.show();                                                                      // Update strip with new contents
      }
    }
    if (neopixelcolor == 255)
    {
      neopixel = false;
    }
  }
  else
  {
    for (int i = 0; i < NUMPIXELS; ++i)
    {                                                     // iterate over all pixels
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // set color of pixel rgb
      pixels.show();
    }
  }
}

// void rainbow(int wait)
// {
//   for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
//   {

//     pixels.rainbow(firstPixelHue);

//     pixels.show(); // Update strip with new contents
//     delay(wait);   // Pause for a moment
//   }
// }

// int light()
// {
//   pixels.clear(); // すべてのLEDの色を0にセット//いじる必要なし
//   for (int i = 0; i < NUMPIXELS; ++i)
//   {                                                   // iterate over all pixels
//     pixels.setPixelColor(i, pixels.Color(255, 0, 255)); // set color of pixel rgb
//   }

//   //  rainbow(5);
//   pixels.show();
// }

void setup()
{
  pixels.begin();
  pixels.setBrightness(brightness); // 明るさ変える変数

  pinMode(center, INPUT_PULLDOWN);
  pinMode(right, INPUT_PULLDOWN);
  pinMode(left, INPUT_PULLDOWN);
  pinMode(back, INPUT_PULLDOWN);

  pinMode(left_center, INPUT_PULLDOWN);
  pinMode(right_center, INPUT_PULLDOWN);
  pinMode(left_back, INPUT_PULLDOWN);
  pinMode(right_back, INPUT_PULLDOWN);

  Serial.begin(115200);

  Serial1.begin(115200);
}

void loop()
{
  // if (time_stop == false)
  // {
  //   rainbow_timer.start();
  //   rainbow_timer.tick();
  //   rainbowtime = rainbow_timer.get_value();
  // }

  // if (rainbowtime <= 1000)
  // {
  rainbow(15);
  // }
  // else
  // {
  //   time_stop = true;
  //   light();
  // }

  // line_center_val = linecenterval.moving_average(analogRead(center));
  // line_left_val = lineleftval.moving_average(analogRead(left));
  // line_right_val = linerightval.moving_average(analogRead(right));
  // line_back_val = linebackval.moving_average(analogRead(back));
  // left_center_val = leftcenterval.moving_average(analogRead(left_center));
  // right_center_val = rightcenterval.moving_average(analogRead(right_center));
  // left_back_val = leftbackval.moving_average(analogRead(left_back));
  // right_back_val = rightbackval.moving_average(analogRead(right_back));
  // int line_dec_val =100;

  line_center_val = analogRead(center);
  line_left_val = analogRead(left);
  line_right_val = analogRead(right);
  line_back_val = analogRead(back);
  left_center_val = analogRead(left_center);
  right_center_val = analogRead(right_center);
  left_back_val = analogRead(left_back);
  right_back_val = analogRead(right_back);

  if (line_center_val >= 65 || line_left_val >= 40 || line_right_val >= 40 || line_back_val >= 40)
  {

    line_side_val = 100;
  }
  else
  {
    line_side_val = 10;
  }

  if (left_center_val >= 30 || right_center_val >= 60 || left_back_val >= 45 || right_back_val >= 45)
  {

    line_deep_val = 100;
  }
  else
  {
    line_deep_val = 10;
  }

  if (line_deep_val == 100 || line_side_val == 100) // && line_No1_side ==100
  {
    if (line_deep_val == 100)
    {
      line_dec = 2;
    }
    else
    {
      line_dec = 1;
    }
  }
  else
  {
    line_dec = 0;
    // Serial.print(line_dec);
  }

  Serial1.print(String(line_dec));
  Serial1.print(String("a"));
  Serial1.println(String("\t"));
  Serial1.flush();

  Serial.print("反応");
  Serial.print(line_dec);
  Serial.print("\t");
  Serial.print("前");
  Serial.print(line_center_val);
  Serial.print("\t");
  Serial.print("右");
  Serial.print(line_right_val);
  Serial.print("\t");
  Serial.print("左");
  Serial.print(line_left_val);
  Serial.print("\t");
  Serial.print("後");
  Serial.print(line_back_val);
  Serial.print("\t");
  Serial.print("右上");
  Serial.print(right_center_val);
  Serial.print("\t");
  Serial.print("左上");
  Serial.print(left_center_val);
  Serial.print("\t");
  Serial.print("右下");
  Serial.print(right_back_val);
  Serial.print("\t");
  Serial.print("左下");
  Serial.print(left_back_val);
  Serial.print("\t");

  Serial.println("\t");

  // delay(15);
}
