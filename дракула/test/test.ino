#define FASTLED_ESP8266_NODEMCU_PIN_ORDER 
#include <FastLED.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#define NUM_LEDS 30
#define CK_INPUT 16
#define CK_INPUT_END 15
#define SOUND_PIN 5
#define LIGHT_PIN 0
#define LED 2 //GPIO4
#define CK_OUTPUT 2
#define RELE 14
#define RX 12
#define TX 13

SoftwareSerial mp3_player(RX, TX); //порты rx tx для плеера D4

unsigned long timer_sleep = 0;
int timer_step = 2000;
bool sound = 0;
int old = 0;
int new_ = 0;
int dracula_level = 1;
unsigned long timer = 0;
bool sun = 0;
int bright = 0;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, LED, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(0);
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Red;
  }
  Serial.begin(9600);
  mp3_player.begin(9600);
  mp3_set_serial(mp3_player);
  delay (100);
  mp3_set_volume(30);
  pinMode(SOUND_PIN, INPUT); //звук
  pinMode(CK_INPUT, INPUT); //вход с цк
  pinMode(CK_OUTPUT, OUTPUT); //выход на цк
  pinMode(LIGHT_PIN, INPUT); //свет
  pinMode(RELE, OUTPUT); //реле гроба
  pinMode(CK_INPUT_END, INPUT);
  digitalWrite(RELE, HIGH);
  digitalWrite(CK_OUTPUT, LOW);
  Serial.begin(9600);
  while (digitalRead(CK_INPUT) == 0){
    if (millis() - timer_sleep > timer_step){
      timer_sleep = millis();
      timer_step = random(2000, 15000);
      mp3_play(4);
      Serial.println(" Звук ");
    }
    yield();
  }
  while (digitalRead(CK_INPUT) == 1){
    digitalWrite(CK_OUTPUT, HIGH);
    Serial.println(" Пришел сигнал ");
    yield();
  }
  digitalWrite(CK_OUTPUT, LOW);
  Serial.println(" Cигнал пропал ");
}


void loop() {
  /*new_ = digitalRead(16);
  if (new_ != old){ //если было изменение
      old = new_;
      timer = millis(); //засекаем время изменения пина
      digitalWrite(4, HIGH);
  }
  if (millis() - timer > 50){ //если долго не было изменений
    digitalWrite(4, LOW); //то звука нет
  }*/
  if (digitalRead(CK_INPUT_END) == 1){ //пришел сигнал об окончании игры
    Serial.println(" Свет ");
      mp3_stop();
      delay(50);
      mp3_play(5);
      while (bright < 255){
        FastLED.setBrightness(bright);
        bright++;
        FastLED.show();
        delay(30);
      }
      digitalWrite(CK_OUTPUT, HIGH);
      digitalWrite(RELE, LOW);
      while (true){yield();};
  }
  if (digitalRead(CK_INPUT) == 1) {sun = 1; Serial.println(" Пришел сигнал ");}
  if (digitalRead(SOUND_PIN) == 1 && millis() - timer > 10000 ){
    mp3_stop();
    delay(50);
    if (dracula_level == 2) mp3_play(2);
    else if (dracula_level == 3) mp3_play(3);
    else mp3_play(1);
    dracula_level++;
    timer = millis();
    Serial.println(" Разбудили Дракулу ");
  }
  if (digitalRead(LIGHT_PIN) == 0 && sun == 1){
    delay(2000);
    if (digitalRead(LIGHT_PIN) == 0){
      Serial.println(" Свет ");
      mp3_stop();
      delay(50);
      mp3_play(5);
      while (bright < 255){
        FastLED.setBrightness(bright);
        bright++;
        FastLED.show();
        delay(30);
      }
      digitalWrite(CK_OUTPUT, HIGH);
      digitalWrite(RELE, LOW);
      while (true){yield();};
    }
  }
}
