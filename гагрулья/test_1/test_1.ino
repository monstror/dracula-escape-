#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "r9hba"
#define STAPSK  "cqwwssb2016"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

SoftwareSerial mp3_player(0, 2); //порты rx tx для плеера

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("Gargulia Quest");
  ArduinoOTA.onStart([]() {
    Serial.println("Start");  //  "Начало OTA-апдейта"
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");  //  "Завершение OTA-апдейта"
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    //  "Ошибка при аутентификации"
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    //  "Ошибка при начале OTA-апдейта"
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    //  "Ошибка при подключении"
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    //  "Ошибка при получении данных"
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    //  "Ошибка при завершении OTA-апдейта"
  });
  ArduinoOTA.begin();
  Serial.println("Ready");  //  "Готово"
  Serial.print("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
}
  pinMode(16, INPUT); //первый порт приема с ЦК
  pinMode(5, INPUT); //второй порт приема с ЦК 
  pinMode(4, INPUT); //третий порт приема с ЦК
  pinMode(14, INPUT); //преступок 1
  pinMode(12, INPUT); //преступок 2
  pinMode(13, INPUT); //преступок 3
  pinMode(15, INPUT); //преступок 4
  pinMode(17, INPUT); //датчик движения
  pinMode(10, OUTPUT); //обратная связь с ЦК
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

void loop() {
  
  digital_pin();
  Serial.println("1");
  delay(1000);
  while (( f1 != 1 || f2 != 0 || f3 != 1) && ( f1 != 1 || f2 != 1 || f3 != 0) && ( f1 != 0 || f2 != 1 || f3 != 1) )  //Режим ожидания
  {
     Serial.println("Ожидание сигнала о количестве игроков");
     digital_pin();
     delay(1000);
     yield();
  };
  if (f1 == 1 && f2 == 0 && f3 == 1) pl_num = 2; //выбор количества игроков
  if (f1 == 1 && f2 == 1 && f3 == 0) pl_num = 3;
  if (f1 == 0 && f2 == 1 && f3 == 1) pl_num = 4;
  Serial.print("Игроков: ");
  Serial.print(pl_num);
  delay(1000);
  Serial.print("Начало! "); //Начало квеста
  delay(1000);
  digital_pin();
  while (k1 != 1) { //первый этап
     Serial.println("Часть первая: Приветсвие ");
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            Serial.print("Гаркулья говорит приветствие. ");
            /*if (k < 3) k++;
            Serial.print("Вызвана подсказка: "); Serial.print(k); Serial.println();*/
            mp3_play(25);
            delay(6000);
            k1 = 1;
          }
        } 
      }
     //if (pr1 + pr2 + pr3 + pr4 == 0) k1 = 0;
     delay(1000);
     yield();
  }
  digitalWrite(10, HIGH);
  delay(10000);
  digitalWrite(10, LOW);
  digital_pin();
  Serial.println("Ожидаю сигнала о решении загадки с преступками ");
  delay(1000);
  k1 = 0;
  k = 0;
  while ( f1 != 1 || f2 != 1 || f3 != 1) {
     Serial.print("Часть вторая: Табличка 1 ");
     digital_pin();
     if ( (pr1 + pr2 + pr3 + pr4) >= pl_num && k1 == 0 ) //вызов подсказки
      {
        if (analogRead(17) < 900) { //проверка дачтика движения
          delay(2000);                //ждем две секунды
          if (analogRead(17) < 900) { //все норм? значит можно дать подсказку
            if (k < 3) k++;
            Serial.print("Вызвана подсказка: "); Serial.print(k); Serial.println();
            mp3_play(k);
            delay(6000);
            k1 = 1;
          }
        } 
      }
     if (pr1 + pr2 + pr3 + pr4 < pl_num) k1 = 0;
     delay(1000);
     yield();
  }
}
