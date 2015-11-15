/* code to read events from LIS3DH accellerometer */
#include <SPI.h>

String inputString = "";         // a string to hold incoming data
String Ctrl = "\x00";
String Click = "\x00";
const int ACS = 10;  // accelleromenter Chip select
const int RCS = 9;   // GS2050 Chip select
const long SPISPD = 1000000*4;  // 4 MHZ SPI Speed
SPISettings spiset(SPISPD, MSBFIRST, SPI_MODE0);

void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(38400);
  SPI.begin();
  // reserve 100 bytes for the inputString:
  inputString.reserve(100);
  // init accelerometer
  pinMode(ACS, OUTPUT);
  pinMode(RCS, OUTPUT);
  digitalWrite(ACS, HIGH);
  digitalWrite(RCS, HIGH);
  
  // Accelerometer click event
  attachInterrupt(0, aclick, RISING);
  spitrx(ACS, Ctrl);
  spitrx(ACS, Click);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void spitrx(int cs, String str)
{
    // Initialize Accelerometer
  SPI.beginTransaction(spiset);
  // write string
  digitalWrite(cs, LOW);
  int i;
  for (i = 0; i < str.length(); i++)
  {
    SPI.transfer(str[i]);
  }
  digitalWrite(cs, HIGH);
  SPI.endTransaction();

}

void aclick()
{
  Serial.println("ClK");
}
