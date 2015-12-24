/* clock with 4-digit MAX7219 LED display */
#include <Time.h>
#include <TimeAlarms.h>
#include <cRGB.h>
#include <WS2812.h>
#include <avr/pgmspace.h>
#include <ctype.h>
#include <TM1637Display.h>
/*
#include <IRLib.h>
*/
TM1637Display disp(5,6);

enum colmap {
  colMax = 6
};

enum kp {
  kpClk = 7,
  kpDat = 2
};

void setColor(byte c);
void ledInit(void);
void ledDisp(uint16_t val, bool dp);
void kpIrq(void);

WS2812 sled(1);
String inpMsg;
volatile uint8_t kpInt = 0;

static uint8_t minTask = 0;
static AlarmId taskTimer;

void setup() {
  ledInit();
  sled.setOutput(9);
  Serial.begin(115200);
  setColor(2);
  inpMsg.reserve(25);
  disp.setBrightness(0xf);

  /* Keypad Clock pin */
  pinMode(kpClk, OUTPUT);
  digitalWrite(kpClk, LOW);

  /* Keypad Data Pin */
  pinMode(kpDat, INPUT);
  digitalWrite(kpDat, LOW);

  /* presents pin change interrupt on key press */
  attachInterrupt(digitalPinToInterrupt(kpDat), kpIrq, RISING);

  /* kick off minute repeat timer at the top of the minute */
  Alarm.timerOnce(60 - second(), setUpTimer);
  taskTimer = Alarm.timerRepeat(10, taskTick);
  Alarm.disable(taskTimer);
  disp.showNumberDec(0, true);
}

void loop() {

  uint8_t keyHit = 0xff;    /* 0xff is an invalid value, is when the key is released */

  /* check for keypad interrupt */
  if (kpInt) {
    detachInterrupt(digitalPinToInterrupt(kpDat));
    kpInt = 0;
    keyHit = readKey();
    Alarm.delay(160);
    attachInterrupt(digitalPinToInterrupt(kpDat), kpIrq, RISING);
  }

  /* test for valid key read */
  if (keyHit != 0xff) {
    minTask = keyHit * 6;
    /* start countdown time */
    Alarm.enable(taskTimer);
    taskTick();
  }

  /* this is needed to fire off Timer Alarm chain */
  Alarm.delay(0);
}

/* called by setup, instantiante minute timer */
void setUpTimer()
{
  Alarm.timerRepeat(60, minuteTick);
  minuteTick();
}

void minuteTick()
{
  static uint8_t i = 1;
  
  unsigned t = hour() * 100 + minute();
  ledDisp(t, true);
  setColor(colMax);
  delay(100);
  setColor(i);
  i++;
  i %= colMax;
}

/* when task is enabled, update display */
void taskTick()
{
    if (!minTask) {
      Alarm.disable(taskTimer);
      /* insert beep/ tone */
   }
   
   /* convert minTask (quanta) to MM:SS */
   uint16_t dispS = (minTask / 6) * 100 + (minTask % 6) * 10;
   disp.showNumberDec(dispS, true);
   
   if (minTask)
    minTask--;
}

/* called when during serial input */
void serialEvent()
{
  char c = Serial.read();
  if (iscntrl(c)) {
    /* end of message */
    switch (inpMsg[0]) {

      case 'T':
        processSyncMessage(inpMsg);
      break;

      default:
        Serial.println("\bErr");
      break;
    }
    inpMsg = "";
  }
  else {
    inpMsg += c;
  }
}

void kpIrq()
{
  kpInt++;
}

const cRGB ctable[] PROGMEM = {
  {0,   0x40, 0},
  {0x40,   0, 0},
  {0, 0,   0x40},
  {0x40, 0x40, 0},
  {0, 0x40, 0x40},
  {0x40, 0, 0x40},
  {0x40, 0x40, 0x40}
};

void setColor(uint8_t c)
{
  char fmt[30];
  cRGB col;
  uint32_t tmp;
  /* read 32-bit dword from flash */
  tmp = pgm_read_dword_near(&ctable[c]);
  /* cast to rgb structure (works if packed */
  memcpy(&col, &tmp, sizeof(col));
  sled.set_crgb_at(0, col);
  sled.sync();
}
