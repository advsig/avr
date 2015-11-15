#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x22, 16, 2);  // set the LCD address to 0x21 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.print("We Love Tracy");  
  lcd.setCursor(0,1);
  lcd.print("24/7/365-She's never");
  lcd.backlight();
}
void loop()
{
}
