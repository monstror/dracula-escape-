#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial mp3_player(0, 2); //порты rx tx для плеера D4

void setup() {
  pinMode(16, INPUT); //первый порт приема с ЦК D0
  pinMode(5, INPUT); //второй порт приема с ЦК D1
  pinMode(4, INPUT); //третий порт приема с ЦК D2
  pinMode(14, INPUT); //преступок 1 D5
  pinMode(12, INPUT); //преступок 2 D6
  pinMode(13, INPUT); //преступок 3 D7
  pinMode(15, INPUT); //преступок 4 D8 D3
  pinMode(17, INPUT); //датчик движения A0
  pinMode(10, OUTPUT); //обратная связь с ЦК SD3
  digitalWrite(10, LOW);
  Serial.begin(9600);
  mp3_player.begin(9600);
  mp3_set_serial(mp3_player);
  delay (100);
  mp3_set_volume(30);
}
  
  int k = 0;
  bool k1 = 0;
  int f1 = digitalRead(16); //первый порт приема с ЦК
  int f2 = digitalRead(5);  //второй порт приема с ЦК 
  int f3 = digitalRead(4);  //третий порт приема с ЦК
  int pr1 = digitalRead(14); //преступок 1
  int pr2 = digitalRead(12); //преступок 2
  int pr3 = digitalRead(13); //преступок 3
  int pr4 = digitalRead(15); //преступок 4
  int pl_num = 0;           //количество игроков

void digital_pin(){ //считывание значений с пинов и вывод данных в монитор порта
  f1 = digitalRead(16);
  f2 = digitalRead(5);
  f3 = digitalRead(4);
  pr1 = digitalRead(14);
  pr2 = digitalRead(12);
  pr3 = digitalRead(13);
  pr4 = digitalRead(15);
  Serial.print(f1);
  Serial.print(f2);
  Serial.print(f3);
  Serial.print(" ");
  Serial.print(pl_num);
  Serial.print(" ");
  Serial.print(pr1 + pr2 + pr3 + pr4);
  Serial.print(" ");
  Serial.print(k1);
  Serial.print(" ");
  Serial.print(analogRead(17));
  Serial.println();
}

void signal_ck(){
  digitalWrite(10, HIGH);
  delay(5000);
  digitalWrite(10, LOW);
}

void loop() {
  digital_pin();
  delay(1000);
  while (( f1 != 1 || f2 != 0 || f3 != 1) && ( f1 != 1 || f2 != 1 || f3 != 0) && ( f1 != 0 || f2 != 1 || f3 != 1) )  //Режим ожидания
    {
      if (f1 == 0 && f2 == 1 && f3 == 0){
        mp3_play(1);
        delay(2000);
      }
      digital_pin();
      yield();
    };
  if (f1 == 1 && f2 == 0 && f3 == 1) pl_num = 2; //выбор количества игроков
  if (f1 == 1 && f2 == 1 && f3 == 0) pl_num = 3;
  if (f1 == 0 && f2 == 1 && f3 == 1) pl_num = 4;
  delay(1000);
  digital_pin();
  while (k1 != 1) //начало игры
    { 
      digital_pin();
      if ( (pr1 + pr2 + pr3 + pr4) >= pl_num) //вызов преступков
        {
          if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно говорить
            mp3_play(2);
            delay(6000);
            k1 = 1;
            }
          } 
        }
     delay(1000);
     yield();
  }
  signal_ck();
  digital_pin();
  delay(1000);
  k1 = 0;
  k = 0;
  while ( f1 != 1 || f2 != 1 || f3 != 1) { //ждем сигнал о прохождении таблички 1
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(3);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  mp3_play(4);
  delay(8000);
  while ((pr1 + pr2 + pr3 + pr4) < pl_num){
      yield();
  }
  signal_ck();
  while ( f1 != 0 || f2 != 1 || f3 != 0) { //ждем сигнал о прохождении таблички 2
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(3);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  mp3_play(12);
  while ( f1 != 1 || f2 != 0 || f3 != 1) { //ждем сигнал о прохождении предметов
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(5);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  while ( f1 != 0 || f2 != 1 || f3 != 1) { //ждем сигнал о прохождении к1
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(6);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  while ( f1 != 1 || f2 != 0 || f3 != 0) { //ждем сигнал о прохождении к2
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(7);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  while ( f1 != 0 || f2 != 0 || f3 != 1) { //ждем сигнал о прохождении к3
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(8);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  while ( f1 != 1 || f2 != 1 || f3 != 0) { //ждем сигнал о прохождении к4
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(9);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  while ( f1 != 1 || f2 != 0 || f3 != 1) { //ждем сигнал об открытии гроба
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            mp3_play(10);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
  delay(2000);
  mp3_play(11);
}
