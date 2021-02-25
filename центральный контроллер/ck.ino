#define B1 24 //выбор 1
#define B2 23 //выбор 2
#define B3 25 //выбор 3
#define B4 22 //старт
#define G1 26 //выход на гаргулью 1
#define G2 27 //выход на гаргулью 2
#define G3 28 //выход на гаргулью 3
#define T1 29 //выход на табличку 1
#define T2 30 //выход на табличку 2
#define PR 31 //выход на предметы
#define K1 32 //выход на картину 1
#define K2 33 //выход на картину 2
#define K3 34 //выход на картину 3
#define K4 35 //выход на картину 4
#define DR 36 //выход на дракулу
#define SM 37 //выход на дым машину
#define FK 38 //выход на факела
#define DR_END 39 //выход на дракулу (конец)
#define G_I 2 //вход с гаргульи
#define T1_I 3 //вход с таблички 1
#define T2_I 4 //вход с таблички 2
#define PR_I 5 //вход с предметов
#define K1_I 6 //вход с картины 1
#define K2_I 7 //вход с картины 2
#define K3_I 8 //вход с картины 3
#define K4_I 9 //вход с картины 4
#define DR_I 10 //вход с дракулы

#include <DFPlayer_Mini_Mp3.h> 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(18, 19); // плеер rx tx
int players = 0;
unsigned long timer = 0;
int time_seq = 0;
bool smoke_check = 0;

void time_check(){
    if (millis() - timer > 3000000 && time_seq < 1 && smoke_check == 0){
    smoke_check++;
    digitalWrite(SM, HIGH);
    time_seq = 1;
  }
  if (millis() - timer > 3600000 && time_seq < 2){
    fakel();
    mp3_play(2);
    time_seq = 2;
  }
}

void gargulya(int f1, int f2, int f3){ //отправка кода в гаргулью
  digitalWrite(G1, f1);
  digitalWrite(G2, f2);
  digitalWrite(G3, f3);
}

void fakel(){
  digitalWrite(FK, HIGH);
  delay(1000);
  digitalWrite(FK, LOW);
}

void setup() {
  mySerial.begin(9600);
  mp3_set_serial(mySerial); 
  delay(1);
  mp3_set_volume(30);
  Serial.begin(9600);
  pinMode(B1, INPUT);
  pinMode(B2, INPUT); 
  pinMode(B3, INPUT);
  pinMode(B4, INPUT); 
  pinMode(G1, OUTPUT); 
  pinMode(G2, OUTPUT);
  pinMode(G3, OUTPUT); 
  pinMode(T1, OUTPUT);
  pinMode(T2, OUTPUT);
  pinMode(PR, OUTPUT);
  pinMode(K1, OUTPUT);
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);
  pinMode(K4, OUTPUT);
  pinMode(DR, OUTPUT);
  pinMode(SM, OUTPUT);
  pinMode(G_I, INPUT);
  pinMode(T1_I, INPUT);
  pinMode(T2_I, INPUT);
  pinMode(PR_I, INPUT);
  pinMode(K1_I, INPUT);
  pinMode(K2_I, INPUT);
  pinMode(K3_I, INPUT);
  pinMode(K4_I, INPUT);
  pinMode(DR_I, INPUT);
  gargulya(0, 0, 0);
  while(digitalRead(B4) == 1 || players == 0){
    if (digitalRead(B1) == 1) players = 2;
    else if (digitalRead(B2) == 1) players = 3;
    else if (digitalRead(B3) == 1) players = 4;
    Serial.println(players);
  }
  timer = millis();
  mp3_play(1);
  if (players == 2) gargulya(1, 0, 1);
  if (players == 3) gargulya(1, 1, 0);
  if (players == 4) gargulya(0, 1, 1);
  while (digitalRead(G_I) == 0){ //ждем ответа от гаргульи
    time_check();
    if (digitalRead(B1) == 1){ //запрос подсказки
      gargulya(0, 1, 0);
      delay(1000);
      gargulya(0, 0, 0);
    }
  }
  digitalWrite(T1, HIGH); //включаем табло 1
  Serial.println("T1");
  delay(1000);
  while (digitalRead(T1_I) == 0){//ждем прохождение табло 1
    time_check();
  }
  gargulya(1, 1, 1); //отправляем сигнал о прохождении табло 1
  delay(1000);
  while (digitalRead(G_I) == 0){ //ждем ответа от гаргульи
    time_check();
  }
  digitalWrite(T2, HIGH); //включаем табло 2
  delay(1000);
  while (digitalRead(T2_I) == 0){//ждем прохождение табло 2
    time_check();
  }
  gargulya(0, 1, 0); //отправляем сигнал о прохождении табло 2
  fakel();
  digitalWrite(DR, HIGH);
  delay(1000);
  digitalWrite(DR, LOW);
  digitalWrite(PR, HIGH); //включаем предметы
  while (digitalRead(PR_I) == 0){//ждем прохождение предметов
    time_check();
  }
  gargulya(1, 0, 1); //отправляем сигнал о прохождении предметов
  digitalWrite(K1, HIGH); //включаем картину 1
  delay(1000);
  while (digitalRead(K1_I) == 0){//ждем прохождение картины
    time_check();
  }
  gargulya(0, 1, 1); //отправляем сигнал о прохождении картины 1
  digitalWrite(K2, HIGH); //включаем картину 2
  delay(1000);
  while (digitalRead(K2_I) == 0){//ждем прохождение картины 2
    time_check();
  }
  gargulya(1, 0, 0); //отправляем сигнал о прохождении картины 2
  digitalWrite(K3, HIGH); //включаем картину 3
  if (smoke_check == 0){//включаем дым машину
    smoke_check++;
    digitalWrite(SM, HIGH);
  }
  delay(1000);
  while (digitalRead(K3_I) == 0){//ждем прохождение картины 3
    time_check();
  }
  gargulya(0, 0, 1); //отправляем сигнал о прохождении картины 3
  digitalWrite(K4, HIGH);
  while (digitalRead(K4_I) == 0){//ждем прохождение картины 3
    time_check();
  }
  gargulya(1, 1, 0);//отправляем сигнал о прохождении картины 4
  digitalWrite(DR, HIGH);
  while (digitalRead(DR_I) == 0){//ждем дракулы
    time_check();
  }
  fakel();
  gargulya(1, 0, 1);
}



void loop() {
  
}
