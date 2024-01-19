/*
本程式為LCD 1602的範例

首先要安裝LiquidCrystal_PCF8574的函式庫
接線請參考附圖

LCD 16x2 賣場：
https://www.jmaker.com.tw/products/lcd1602

LCD支架賣場：
https://www.jmaker.com.tw/products/product170

LCD 20x4 賣場：
https://www.jmaker.com.tw/products/lcd20x4



傑森創工賣場：https://www.jmaker.com.tw

傑森創工粉絲團：https://www.facebook.com/jasonshow

 */

#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);  // 設定i2c位址，一般情況就是0x27和0x3F兩種

void setup()
{
  lcd.begin(16, 2); // 初始化LCD
  //lcd.begin(20, 4); // 如果是20x4的LCD就是用這行
  
  lcd.setBacklight(255);
  lcd.clear();
  lcd.setCursor(0, 0);  //設定游標位置 (字,行)
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("Thank you");
} // setup()

void loop()
{
 
} // loop()
