#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // создаем экземпляр MFRC522 instance.

SoftwareSerial mp3Serial(3, 2);

unsigned long timer = 0;
bool check = 0;
bool flag = 0;
 
void setup()
{
  //Serial.begin(9600);
  mp3Serial.begin (9600);
  mp3_set_serial (mp3Serial);
  mp3_set_volume (30);
  SPI.begin();          // инициализируем шину SPI
  mfrc522.PCD_Init();   // инициализируем MFRC522
  pinMode(A2, INPUT); //порт входа с ЦК
  pinMode(5, OUTPUT); //порт выхода на ЦК
  pinMode(6, INPUT);  //порт входа сигнала чека
  pinMode(A0, INPUT); //порт входа сигнала плеера
  digitalWrite(5, LOW); //даем сигнал об отсутсвии 
  while(digitalRead(A2) != HIGH){
    //Serial.println(" ЖДУ ");
    yield();
  }
  //Serial.println(" НАЧАЛО ");
  delay(2000);
}

/*void serial_print(){
  Serial.print(" Чек ");
  Serial.print(check);
  Serial.print(" A0 ");
  Serial.print(digitalRead(A0));
  Serial.print(" Сигнал RFID ");
  Serial.print(digitalRead(6));
  Serial.print(" Сигнал на ЦК ");
  Serial.print(digitalRead(5));
  Serial.println();
}
*/
void loop()
{
 //serial_print(); //вывод данных в серийный порт

  if (digitalRead(A0) == HIGH || flag == 1){ //если внешний сигнал есть, то региструем это
    if (millis() - timer > 10000){
      timer = millis();
      digitalWrite(5, LOW);
      mp3_play(1);
      flag = 0;
    }
  }
  if (digitalRead(6) == HIGH || check == 0) digitalWrite(5, LOW); //если датчик не совпадает и с предыдущего контроллера сигнала есть, то не даем сигнал на ЦК

  if ( ! mfrc522.PICC_IsNewCardPresent()) //проверка наличия метки
  {
    delay(100);
    if ( ! mfrc522.PICC_IsNewCardPresent()){
      check = 0; //если карты нет, то регистрируем отсутствие соответствующей метки
      flag = 0;
      return;
    }
    
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) //считывание метки
  {
    check = 0; //если карты нет, то регистрируем отсутствие соответствующей метки
    flag = 0;
    return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) //считываем UID метки
  {
/*     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);*/
     
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
//  Serial.println();
  content.toUpperCase();
  if (content.substring(1) == "93 8C 9B 1A" || content.substring(1) == "6B F3 AF 0D") //(93 8C 9B 1A)
  {                                                                                   //(6B F3 AF 0D)
    check = 1; //если совпало, то регистрируем это
    if (digitalRead(6) == LOW && digitalRead(A0) == LOW) digitalWrite(5, HIGH); else digitalWrite(5, LOW);
  }
 else   {
    check = 0; //если не совпало, то регистрируем это
    digitalWrite(5, LOW); //даем сигнал об отсутсвии правильной метки
    flag = 1;
  }
}
