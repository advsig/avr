#include <shine.h>
#include <SPI.h>
#define CS 10

static SPISettings spiSet(1000000, MSBFIRST, SPI_MODE0);
void ledInit(void)
{
  uint8_t maxInit[] = {0xff, 2, 7, 1};
  uint8_t reg = 9, i;
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  SPI.begin();
  SPI.beginTransaction(spiSet);
  for (i = 0; i < sizeof(maxInit); i++) {
    trx(reg++, maxInit[i]);
  }
  /* reset test mode */
  trx(0xf, 0);
  SPI.endTransaction();
}

void ledDisp(uint16_t val, uint8_t row, bool dp)
{
  uint8_t i, dig, lim;
  SPI.beginTransaction(spiSet);
  lim = (row) ? 1 : 5;
  for (i = 0; i < 4; i++) {
    dig = (val) ? val % 10 : 0xf ;
    if (dp && i == 2) {
      dig |= 0x80;
    }
    trx(lim++, dig);
    val /= 10;
  }
  SPI.endTransaction();
}

void trx(uint8_t reg, uint8_t val)
{
  digitalWrite(CS, LOW);
  SPI.transfer(reg);
  SPI.transfer(val);
  digitalWrite(CS, HIGH);
}

