#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_PCF8574.h>

#include <Servo.h>   //載入函式庫，這是內建的，不用安裝

#define RST_PIN         9          
#define SS_PIN          10 //RC522卡上的SDA
Servo myservo;  // 建立SERVO物件
MFRC522 mfrc522;   // 建立MFRC522實體
LiquidCrystal_PCF8574 lcd(0x27);  // 設定i2c位址，一般情況就是0x27和0x3F兩種
char *reference;
int SensorPin = 4;
int OutputPin = 5;
//byte uid[]={0x01, 0x3F, 0xDC, 0x03};  //磁扣
//0xE1,0x1A,0x6F,0x21,//卡片

byte uid[2][4]={{0x01, 0x3F, 0xDC, 0x03},{0xE1,0x1A,0x6F,0x22}};  //磁扣
//0xE1,0x1A,0x6F,0x21    0x01, 0x3F, 0xDC, 0x03//卡片

void setup()
{   
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SensorPin, INPUT);
  lcd.begin(16, 2); // 初始化LCD
  lcd.setBacklight(125);
  lcd.clear();
  lcd.setCursor(0, 0);  //設定游標位置 (字,行)
  lcd.print(F("    Car  Park"));
  lcd.setCursor(0, 1);
  lcd.print(F("      open"));
  Serial.begin(9600);
  SPI.begin();
  myservo.attach(3);
  myservo.write(180);  //旋轉到0度，就是一般所說的歸零
  
  mfrc522.PCD_Init(SS_PIN, RST_PIN); // 初始化MFRC522卡
  // Serial.print(F("Reader "));
  // Serial.print(F(": "));
  mfrc522.PCD_DumpVersionToSerial(); // 顯示讀卡設備的版本
}

void loop() {
  // 檢查是不是偵測到新的卡
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // 顯示卡片的UID
      //Serial.print(F("Card UID:"));
      //dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // 顯示卡片的UID
      //Serial.println();
      //Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      //Serial.println(mfrc522.PICC_GetTypeName(piccType));  //顯示卡片的類型
      
      bool they_match = true; // 初始值是假設為真 
      for ( int i = 0; i < 2; i++ ) { // 卡片UID為4段，分別做比對
          they_match = true; // 初始值是假設為真 
          for(int j = 0 ; j < 4 ; j++ ){
            if ( uid[i][j] != mfrc522.uid.uidByte[j] ) { 
              they_match = false; // 如果任何一個比對不正確，they_match就為false，然後就結束比對
              break; 
            }
          } 
          if(they_match) break;
      }
      //在監控視窗中顯示比對的結果
      if(they_match){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print(F("    Welcome"));
        lcd.setCursor(0, 1);
        lcd.print(F("  door open"));
        //Serial.println(F("歡迎進入"));
        myservo.write(90); 
        delay(1000);
        int SensorValue = digitalRead(SensorPin);
        while(SensorValue ==0){
          SensorValue = digitalRead(SensorPin);
          if(SensorValue==1){
            for(int h=0;h<2000;h++){
              delay(1);
              SensorValue = digitalRead(SensorPin);
              if(SensorValue==0){
                break;
              }
            }         
          }
        }

        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print(F("  door closeing"));
        delay(1000);
        myservo.write(180);
      }else{
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print(F("     stop!"));
        lcd.setCursor(0, 1);
        lcd.print(F("  door close"));
        //Serial.println(F("禁止進入!"));
        delay(2000);
      }
      mfrc522.PICC_HaltA();  // 卡片進入停止模式
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);  //設定游標位置 (字,行)
      lcd.print(F("    Car  Park"));
      lcd.setCursor(0, 1);
      lcd.print(F("      open"));
    }
}

/**
 * 這個副程式把讀取到的UID，用16進位顯示出來
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
