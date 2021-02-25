#define FASTLED_ESP8266_NODEMCU_PIN_ORDER 

#include <FastLED.h>
#define LED_PIN     1 
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS    4
#define TIME_STEP 60000
#define FRAMES_PER_SECOND 60

int bright = 255;
int bright_old = 255;
int limit_up = 255;
int limit_down = 120;
int delay_time = 5;
int n_step = 15;
bool flag = 0;

unsigned long time_pos = 0;

CRGB leds[NUM_LEDS];

void setup() {
 Serial.begin(9600);
 FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
 pinMode(4, INPUT); //датчик
 pinMode(5, INPUT); //вход с ЦК
 leds[0] = CRGB::White;
 leds[1] = CRGB::White;
 leds[2] = CRGB::White;
 FastLED.setBrightness(300);
 FastLED.show();
 while(digitalRead(4) == HIGH){
  yield();
 }
 delay(1000);
 leds[0] = CRGB::OrangeRed;
 leds[1] = CRGB::OrangeRed;
 leds[2] = CRGB::DarkOrange;
 FastLED.setBrightness(bright);
 FastLED.show();
 delay(1000);
 bright = random(50, 255);
 time_pos = millis();
}

void step_edit(){
    time_pos = millis();
    n_step--;
    if (n_step < 0) n_step = 0;
    limit_up = 17 * n_step;
    limit_down = 8 * n_step;
    if (n_step > 11) delay_time = 5;
    if (n_step >= 7 && n_step <= 11) delay_time = delay_time + (12 - n_step);
    if (n_step < 7) delay_time = (7 - n_step) * 10;
}

void loop() {
  if (bright_old != bright){
    if (bright_old < bright) {
      bright_old++;
    }
    if (bright_old > bright){
      bright_old--;
    }
  } else {
    bright_old = bright;
    bright = random(limit_down, limit_up);
  }
  if (millis() - time_pos > TIME_STEP){
    step_edit();
  }
  FastLED.setBrightness(bright_old);
  FastLED.show();
  if (digitalRead(4) == LOW && n_step < 10 && flag == 1) {n_step = 16; step_edit(); flag = 0;}
  if (digitalRead(5) == HIGH && flag == 0) {
    bright = 0;
    bright_old = 0;
    n_step = 1;
    step_edit();
    FastLED.setBrightness(0);
    FastLED.show();
    flag = 1;
    delay(2000);
  }
 // Serial.print(n_step); Serial.print(" "); Serial.print(limit_up); Serial.print(" "); Serial.print(limit_down); Serial.print(" "); Serial.print(delay_time); Serial.print(" "); Serial.print(digitalRead(4)); Serial.println(); 
  delay(delay_time);
}
