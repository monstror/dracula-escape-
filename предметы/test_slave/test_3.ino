#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // создаем экземпляр MFRC522 instance.
unsigned int timer = 0;
bool flag_1 = 0; //сигнал для плеера локальный
bool flag_2 = 0; //сигнал для плеера приходящий
bool check = 0;
 
void setup()
{
  //Serial.begin(9600);
  SPI.begin();          // инициализируем шину SPI
  mfrc522.PCD_Init();   // инициализируем MFRC522
  pinMode(5, OUTPUT); //порт сигнала плеера
  pinMode(4, OUTPUT); //порт сигнала чека 
  pinMode(6, INPUT);  //порт входа сигнала чека
  pinMode(A0, INPUT); //порт входа сигнала плеера
  digitalWrite(4, HIGH); //даем сигнал об отсутсвии 
  digitalWrite(5, LOW);  
}
/*
void serial_print(){
  Serial.print(" Флаг плеера локал. ");
  Serial.print(flag_1);
  Serial.print(" Локал. сигнал плеера ");
  Serial.print(digitalRead(5));
  Serial.print(" Флаг плеера приход. ");
  Serial.print(flag_2);
  Serial.print(" Чек ");
  Serial.print(check);
  Serial.print(" Локал. сигнал чека ");
  Serial.print(digitalRead(4));
  Serial.print(" A0 ");
  Serial.print(digitalRead(A0));
  Serial.print(" Сигнал чека ");
  Serial.print(digitalRead(6));
  Serial.println();
}
*/
void loop()
{
  //serial_print(); //вывод данных в серийный порт

  if (flag_1 == 1 || flag_2 == 1) {digitalWrite(5, HIGH); digitalWrite(4, HIGH);} else digitalWrite(5, LOW); //если флаги есть, значит даем сигнал о запуске плеера

  if (digitalRead(A0) == HIGH) flag_2 = 1; else flag_2 = 0; //если внешний сигнал есть, то региструем это

  if (digitalRead(6) == HIGH || check == 0) digitalWrite(4, HIGH); //если датчик не совпадает и с предыдущего контроллера сигнала есть, то тоже включаем сигнал

  if ( ! mfrc522.PICC_IsNewCardPresent()) //проверка наличия метки
  {
    delay(100);
    if ( ! mfrc522.PICC_IsNewCardPresent()){
      check = 0; //если карты нет, то регистрируем отсутствие соответствующей метки
      flag_1 = 0; //отключаем локальный флаг плеера
      return;
    }
    
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) //считывание метки
  {
    return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) //считываем UID метки
  {
    /* Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);*/
     
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  content.toUpperCase();
  if (content.substring(1) == "4D 39 1D D9" || content.substring(1) == "13 02 2B 1B") //(B3 D0 96 1A - 1) (E9 7F C2 56 - 2) (70 98 1A 7C - 3) (4D 39 1D D9 - 4)
  {                                                                                   //(1F C2 4F 39 - 1) (9B A5 64 0D - 2) (4B C8 D8 0D - 3) (13 02 2B 1B - 4)
    check = 1; //если совпало, то регистрируем это
    flag_1 = 0; //отключаем локальный флаг об несоответствии метки
    if (digitalRead(6) == LOW && digitalRead(A0) == LOW) digitalWrite(4, LOW); else digitalWrite(4, HIGH);
  }
 else   {
    check = 0; //если не совпало, то регистрируем это
    digitalWrite(4, HIGH); //даем сигнал об отсутсвии правильной метки
    flag_1 = 1; //включаем локальный флаг об несоответствии метки
  }
}
