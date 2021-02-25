#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // создаем экземпляр MFRC522 instance.
unsigned int timer = 0;
int flag_1 = 3; //сигнал для плеера локальный
int flag_2 = 0; //сигнал для плеера приходящий
bool check = 0;
 
void setup()
{
  SPI.begin();          // инициализируем шину SPI
  mfrc522.PCD_Init();   // инициализируем MFRC522
  pinMode(5, OUTPUT); //порт сигнала плеера
  pinMode(4, OUTPUT); //порт сигнала чека 
  pinMode(6, INPUT);  //порт входа сигнала чека
  pinMode(A0, INPUT); //порт входа сигнала плеера
  digitalWrite(4, HIGH); 
  digitalWrite(5, LOW);  
}
void loop()
{
  if (flag_1 == 1 && millis() - timer > 3000){ //после определенного времени перестаем давать сигнал плеера
    flag_1 = 2;
    timer = millis();
  }
  if (flag_1 == 2 && millis() - timer > 5000) flag_1 = 3; //снова готовы дать сигнал
  if (digitalRead(6) == HIGH) { //если на предыдушем контроллере несовпадение, то и на этом тоже
    digitalWrite(4, HIGH);
  }

  if (check = 1 && digitalRead(6) == LOW) {
    digitalWrite(4, LOW);
  }

  check = 0;
  
  if (digitalRead(A0) == HIGH) flag_2 = 1; else flag_2 = 0; //если внешний сигнал есть, то региструем это
  
  if (flag_1 = 1 || flag_2 == 1) digitalWrite(5, HIGH); else digitalWrite(5, LOW);
   
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "E9 7F C2 56") //  впишите здесь UID тега, которому вы хотите дать доступ (E9 7F C2 56 - 2) (70 98 1A 7C - 3)
  {
    check = 1;
  }
 else   {
    check = 0;
    digitalWrite(4, HIGH);
    if (flag_1 == 3) {flag_1 = 1; timer = millis();}
  }
}
