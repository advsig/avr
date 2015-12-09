#include <SPI.h>
#define CS 10
/* initialize and display a max7219-based display */
static SPISettings spiSet(250000, MSBFIRST, SPI_MODE0);
void ledInit(void)
{
  /* init registers 0x9-0xc */
  uint8_t maxInit[] = {0xff, 2, 3, 1};
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

  /* set display to blank */
  reg = 1;
  for (i = 0; i < 4; i++) {
    trx(reg++, 0xff);
  }
  SPI.endTransaction();
}

void ledDisp(uint16_t val, bool dp)
{
  uint8_t i, dig, lim;
  SPI.beginTransaction(spiSet);
  lim = 4;
  for (i = 0; i < 4 ; i++) {
    dig = (val) ? val % 10 : 0xf ;
    if (dp && i == 2) {
      dig |= 0x80;
    }
    trx(lim--, dig);
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
