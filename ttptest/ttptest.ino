#include <SPI.h>
#include "C:/Users/pkerrigan/Documents/Arduino/libraries/Ttp229/Ttp229.cpp"
#define TTP16Button g_ttp229Button
#pragma pack(pop)
 

void setup()
{
  Serial.begin(115200);
  
  DEBUG_STATUS(Serial.println("===================================================================="));
  DEBUG_STATUS(Serial.println("Button Input tester started"));  
  DEBUG_STATUS(Serial.println("===================================================================="));
  
  TTP16Button.Configure(7, 2);
}
 
void TestStatus()
{
  uint8_t buttonNumber = TTP16Button.GetButtonStatus();
  
  if( 0 != buttonNumber )
  {
    DEBUG_STATUS(Serial.print("Button Pressed : "));
    DEBUG_STATUS(Serial.println(buttonNumber));
  }
 
  delayMicroseconds(2500); // TTP229 document says it will reset the output if 2ms idle + little bit safety
}
 
void TestEvent()
{
#if defined(INT0_PIN) || defined(INT1_PIN)
  if( TTP16Button.HasEvent())
#endif
  {
    CTtp229ButtonEvent buttonEvent = TTP16Button.GetButtonEvent();
    
    if( 0 != buttonEvent.ButtonNumber )
    {
      if( buttonEvent.IsButtonReleased )
      {
        DEBUG_STATUS(Serial.print("Button Released: "));
      }
      else
      {
        DEBUG_STATUS(Serial.print("Button Pressed : "));
      }
      DEBUG_STATUS(Serial.println(buttonEvent.ButtonNumber));
    }
    else
    {
#if defined(INT0_PIN) || defined(INT1_PIN)
        DEBUG_STATUS(Serial.println("Are you not using interrupt? Should never come here for interrupt based system."));
#endif
    }
    
    //Serial.print("CurrentTime : "); Serial.println(millis());
    delayMicroseconds(2500); // TTP229 document says it will reset the output if 2ms idle + little bit safety. Not required if using interrupts
  }
  
}
 
void loop()
{
  //TestStatus();
  TestEvent();
}
