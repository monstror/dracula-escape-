#include <Adafruit_NeoPixel.h>
#include <Encoder.h>
#define LED_COUNT 20
#define LED_PIN 15
#define RELE_1 0
#define RELE_2 12
#define IN_PUT 16
#define OUT_PUT 2

Encoder Enc1(5, 4); //CLK DT
Encoder Enc2(3, 14);
Encoder Enc3(1, 13);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //Serial.begin(9600);
  strip.begin();
  strip.clear();
  strip.show();
  pinMode(OUT_PUT, OUTPUT); //выход на цк
  pinMode(IN_PUT, INPUT); //вход с цк
  pinMode(RELE_1, OUTPUT); //реле 1
  pinMode(RELE_2, OUTPUT); //реле 2
  digitalWrite(RELE_2, LOW);
  digitalWrite(RELE_1, LOW);
  while (digitalRead(IN_PUT) == LOW){
    //Serial.println("Жду сигнала от ЦК");
    delay(1000);
  }
  digitalWrite(RELE_1, HIGH);
  //Serial.println("СИГНАЛ ПРИШЕЛ!");
  Enc1.write(0);
  Enc2.write(40);
  Enc3.write(-20);
}

long oldPosition_1  = -999;
long oldPosition_2  = -999;
long oldPosition_3  = -999;
int led_pos_1 = 0;
int led_pos_2 = 0;
int led_pos_3 = 0;
int old_led_pos_1 = 0;
int old_led_pos_2 = 0;
int old_led_pos_3 = 0; 
unsigned long first_pos = 0;
unsigned long second_pos = 0;
unsigned long third_pos = 0;
unsigned long final_pos = 0;
bool k_1 = 0;
bool k_2 = 0;
bool k_3 = 0;
bool k_final = 0;
double r = 0;
double g = 0;

void pos_deter (long *newPosition, long *oldPosition, int *led_pos, unsigned long *time_pos){
    if ((*newPosition > - 10 && *newPosition < 10) || (*newPosition < -70) || (*newPosition > 70)) *led_pos = 0;
    if ((*newPosition > 10 && *newPosition < 30) || (*newPosition < -50 && *newPosition > -70)) *led_pos = 1;
    if ((*newPosition > 30 && *newPosition < 50) || (*newPosition < -30 && *newPosition > -50)) *led_pos = 2;
    if ((*newPosition > 50 && *newPosition < 70) || (*newPosition < -10 && *newPosition > -30)) *led_pos = 3;  
    *oldPosition = *newPosition;
    *time_pos = millis();
}

void strip_on(int a){
  r = 0;
  g = 0;
  for (int i = 0; i < 120; i++){
           r = r + 1.875;
           g = g + 0.175;
           strip.setPixelColor(a, strip.Color(r, g, 0)); //зажигаем светодиод
           strip.show();
           delay(15);
        }
}

void loop() {
  
  long newPosition_1 = Enc1.read();
  long newPosition_2 = Enc2.read();
  long newPosition_3 = Enc3.read();
  if (newPosition_1 != oldPosition_1) {
    if (newPosition_1 > 79) {
      newPosition_1 = 0;
      Enc1.write(0);
    }
    if (newPosition_1 < -79) {
      newPosition_1 = 0;
      Enc1.write(0);
    }
    pos_deter(&newPosition_1, &oldPosition_1, &led_pos_1, &first_pos);
  }
  
  if (newPosition_2 != oldPosition_2) {
    if (newPosition_2 > 79) {
      newPosition_2 = 0;
      Enc2.write(0);
    }
    if (newPosition_2 < -79) {
      newPosition_2 = 0;
      Enc2.write(0);
    }
    pos_deter(&newPosition_2, &oldPosition_2, &led_pos_2, &second_pos);
  }

  if (newPosition_3 != oldPosition_3) {
    if (newPosition_3 > 79) {
      newPosition_3 = 0;
      Enc3.write(0);
    }
    if (newPosition_3 < -79) {
      newPosition_3 = 0;
      Enc3.write(0);
    }
    pos_deter(&newPosition_3, &oldPosition_3, &led_pos_3, &third_pos);
  }
  
  if (millis() - first_pos > 500 && k_1 == 0){ //если энкодер в нужном положении
        strip_on(led_pos_1); //зажигаем светодиод 0
        k_1 = 1; //и не впадаем в цикл
        old_led_pos_1 = led_pos_1;
  }
  if (led_pos_1 != old_led_pos_1) { //если энкодер не в нужном положении, тушим светодиод
    k_1 = 0; 
    strip.setPixelColor(old_led_pos_1, strip.Color(0, 0, 0)); //тушим светодиод 0
    strip.show();
  }

  if (millis() - second_pos > 500 && k_2 == 0){ //если энкодер в нужном положении
        strip_on(led_pos_2 + 4); //зажигаем светодиод
        k_2 = 1; //и не впадаем в цикл
        old_led_pos_2 = led_pos_2;
  }
  if (led_pos_2 != old_led_pos_2) { //если энкодер не в нужном положении, тушим светодиод
    k_2 = 0; 
    strip.setPixelColor((old_led_pos_2 + 4), strip.Color(0, 0, 0)); //тушим светодиод
    strip.show();
  }

  if (millis() - third_pos > 500 && k_3 == 0){ //если энкодер в нужном положении
        strip_on(led_pos_3 + 8); //зажигаем светодиод
        k_3 = 1; //и не впадаем в цикл
        old_led_pos_3 = led_pos_3;
  }
  if (led_pos_3 != old_led_pos_3) { //если энкодер не в нужном положении, тушим светодиод
    k_3 = 0; 
    strip.setPixelColor((old_led_pos_3 + 8), strip.Color(0, 0, 0)); //тушим светодиод
    strip.show();
  }
  if (led_pos_1 == 3 && led_pos_2 == 0 && led_pos_3 == 1 && k_final == 0){ // если все энкодеры стоят в нужном положении
      final_pos = millis(); //засекаем время
      k_final = 1;     //ставим флаг
  }
  if (led_pos_1 != 3 || led_pos_2 != 0 || led_pos_3 != 1) k_final = 0; //если нет, то флаг снимаем
  if (k_final == 1 && millis() - final_pos > 1500){ //если усё норм, то
      strip.clear();
      int i;
      for (i = 0; i < 120; i++){
           if (r - 1 > 0) r = r - 1; else r = 0;
           if (g + 1 < 255) g = g + 1; else g = 255;
           strip.setPixelColor(3, strip.Color(r, g, 0)); //рисуем светодиоды зеленые
           strip.setPixelColor(4, strip.Color(r, g, 0));
           strip.setPixelColor(9, strip.Color(r, g, 0));
           strip.show();
           if (i < 20) delay(30); else delay(10);
        }
      digitalWrite(OUT_PUT, HIGH);
      digitalWrite(RELE_2, HIGH);
      delay(2000);
      digitalWrite(OUT_PUT, LOW);  
      while(true){yield();};
  }
}
