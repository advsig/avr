#include <I2C.h>
#include <LiquidCrystal.h>

// Sample using LiquidCrystal library 

// Ambient Light Sensor (ALS)
const uint8_t kAlsAddr = 0X4A;

enum ALS_REG : uint8_t {
 ALS_STAT,
 ALS_INT_EN,
 ALS_CONFIG,
 ALS_LUX_H,
 ALS_LUX_L,
 ALS_TESH_H,
 ALS_TESH_L,
 ALS_TESH_T
};

const uint8_t ACCELLADDR = 0X1C;

enum kACCEL_REG : uint8_t {
 A_STATUS,
 A_X,
 A_Y = 0X03,
 A_Z = 0X05,
 A_FIFO_SETUP= 0X09,
 A_TRIG_CFG,
 A_SYSMOD,
 A_INT_SRC,
 A_WHO,
 A_XYZ_D_CFG,
 A_HP_FILTER,
 A_PL_STATUS,
 A_PL_CFG,
 A_PL_COUNT,
 A_BF_THRESH,
 A_PL_TRIP,
 A_FF_CFG,
 A_FF_SRC,
 A_FF_TRESH,
 A_FF_COUNT,
 A_TR_CFG = 0X1D,
 A_TR_SRC,
 A_TR_TRESH,
 A_TR_COUNT,
 A_PUL_CFG,
 A_PUL_SRC,
 A_PUL_TRESHX,
 A_PUL_TRESHY,
 A_PUL_TRESHZ,
 A_PUL_TMLT,
 A_PUL_LTCY,
 A_PUL_WIND,
 A_AS_COUNT,
 A_CTRL1,
 A_CTRL2,
 A_CTRL3,
 A_CTRL4,
 A_CTRL5,
 A_OFF_X,
 A_OFF_Y,
 A_OFF_Z
};

typedef struct AccelFifoStatus_t {
   uint8_t fifoCnt  : 6;
   uint8_t wmrkFlag : 1;
   uint8_t overflow : 1;
} AccelFifoStatus;

typedef struct AccelFifoSetup_t { // Reg 0x9, F_SETUP
   uint8_t wmark    : 6;
   uint8_t mode	    : 2;
} AccelFifoSetup;

enum kAccelTrig : uint8_t {  // Reg 0xA, TRIG_CFG
   kTrigFFMT 	= (1 << 2),
   kTrigPulse 	= (1 << 3),
   kTrigLandPrt	= (1 << 4),
   kTrigTrans	= (1 << 5)
};

typedef struct AccelSysMode_t {  // Reg 0xB SYSMOD
  uint8_t fgMode    : 2;
  uint8_t fgGate    : 5;
  uint8_t fgErr     : 1;
} AccelSysMode;

enum kAccelIntSrc : uint8_t {  // Reg 0xC IntSrc
  kSrcDrdy      = (1 << 0),
  kSrcFfMt      = (1 << 2),
  kSrcPulse     = (1 << 3),
  kSrcLandPrt   = (1 << 4),
  kSrcTrans     = (1 << 5),
  kSrcFifo      = (1 << 6),
  kSrcAsleep    = (1 << 7)
};

typedef struct AccelCtrlReg1_t {      // Reg 0x2A ControlReg1
  uint8_t active            : 1;
  uint8_t fRead             : 1;
  uint8_t lnoise            : 1;
  uint8_t dataRate          : 3;
  uint8_t asleepRate        : 2;
} AccelCtrlReg1;

typedef struct AccelCtrlReg3_t {    // Reg 0x2C ControlReg3
  uint8_t ppOpenDrain       : 1;
  uint8_t intPol            : 1;
  uint8_t rsvd1             : 1;
  uint8_t wakeFfMotion      : 1;
  uint8_t wakePulse         : 1;
  uint8_t wakeLandPrt       : 1;
  uint8_t wakeTransient     : 1;
  uint8_t FifoGate          : 1;
} AccelCtrlReg3;

typedef struct AccelCtrlReg4_t {  // Reg 0x2D Interrupt Enable Register
  uint8_t intEnDrdy         : 1;
  uint8_t rsvd1             : 1;
  uint8_t intEnFFMt         : 1;
  uint8_t intEnPulse        : 1;
  uint8_t intEnLandPrt      : 1;
  uint8_t intEnTrans        : 1;
  uint8_t intEnFifo         : 1;
  uint8_t intEnAsleep       : 1;
} AccelCtrlReg4;

typedef struct AccelCtrlReg5_t {  // Reg 0x2E Interrupt Config Register
  uint8_t intCfgDrdy        : 1;
  uint8_t rsvd1             : 1;
  uint8_t intCfgFFMt        : 1;
  uint8_t intCfgPulse       : 1;
  uint8_t intCfgLandPrt     : 1;
  uint8_t intCfgTrans       : 1;
  uint8_t intCfgFifo        : 1;
  uint8_t intCfgAsleep      : 1;
} AccelCtrlReg5;

#define T_SIGN_MASK		0X800
#define T_SIGN_MASK1	        0X7FF
#define	T_SIGN_NEG		0X8000
 	
const uint8_t TMP02ADDR = 0x48;
enum TEMP_R : uint8_t {
  T_TEMPERATURE_REG,
  T_CONFIG_REG,
  T_TLOW,
  T_HIGH
};

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
byte temperature[2] ;
byte A_axis[2] ;
byte ALS_lux[2] ;

enum btn {
 btnRIGHT,
 btnUP,
 btnDOWN,
 btnLEFT,
 btnSELECT,
 btnNONE
};
 
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT; 
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}

void read_temp( uint8_t T_register )
{
	I2c.read( (uint8_t) TMP02ADDR, (uint8_t) T_register, (uint8_t) 2 ); //receive 2 bytes
	temperature[0] = temperature[1] = 0 ;
	temperature[1] = I2c.receive() ;
	if( T_register == T_TEMPERATURE_REG )
		temperature[1] &= 0X0F ;
	temperature[0] = I2c.receive() ;
}

void read_axis( uint8_t T_axis )
{
	A_axis[0] = A_axis[1] = 0 ;	
	I2c.read( ACCELLADDR, T_axis,2, &A_axis[0] ) ;	
}

void read_lux( uint8_t T_lux, uint8_t num )
{
	ALS_lux[0] = ALS_lux[1] = 0 ;
	I2c.read( kAlsAddr, T_lux, 1, &ALS_lux[num] ) ;	
}
 
void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 I2c.begin();
 Serial.begin(9600);
 I2c.scan();
 if (I2c.write( ACCELLADDR, A_CTRL1, (uint8_t) 0X01) != 0) {
     Serial.println("error writing accell Ctl1");
 }
 if (I2c.write( ACCELLADDR, A_CTRL3, (uint8_t) 0X01) != 0) { // open drain on interrupt
     Serial.println("error writing accell Ctlt3");
 }
}
 
void loop()
{
 lcd.setCursor(0,0);			// move to the beginning of the second line
 
 lcd_key = read_LCD_buttons();  // read the buttons
 
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
	 lcd.clear() ;
	 lcd.setCursor(0,0);
	 read_temp(T_CONFIG_REG) ;
	 lcd.print("T_cfg = ");
	 lcd.print(temperature[1],HEX) ;
	 lcd.print(temperature[0],HEX) ;
	 lcd.setCursor(0,1);            // move to the beginning of the second line	 
     lcd.print("Temp   ") ;
	 read_temp(T_TEMPERATURE_REG) ;
	 lcd.print(temperature[1],HEX) ;
	 lcd.print(temperature[0],HEX) ;
     break;
     }
   case btnLEFT:
     {	 
	 lcd.clear() ;
	 lcd.setCursor(0,0);
	 read_lux(ALS_CONFIG, 0) ;
	 lcd.print("ALS_CFG = ");
	 lcd.print(ALS_lux[0],HEX) ;

	 lcd.setCursor(0,1);            // move to the beginning of the second line
	 lcd.print("LUX ") ;
	 read_lux(ALS_LUX_L,0) ;
	 read_lux(ALS_LUX_H,1) ;
	 lcd.print(ALS_lux[1],HEX) ;
	 lcd.print(ALS_lux[0],HEX) ;
     break;
     }
   case btnUP:
     {
	 lcd.clear() ;
	 lcd.setCursor(0,0);
	 read_axis( A_X ) ;
	 lcd.print("X ");
	 lcd.print(A_axis[1],HEX);	 
	 lcd.print(A_axis[0],HEX);
	 read_axis( A_Y ) ;
	 lcd.print(" Y ");
	 lcd.print(A_axis[1],HEX);
	 lcd.print(A_axis[0],HEX);
	 lcd.setCursor(0,1);            // move to the beginning of the second line	 	 	 
	 read_axis( A_Z ) ;
	 lcd.print("Z ");
	 lcd.print(A_axis[1],HEX);
	 lcd.print(A_axis[0],HEX);
	 I2c.read( ACCELLADDR, A_CTRL1,1, &A_axis[0] ) ;
	 read_axis(A_CTRL2 ) ;
	 lcd.print(" C ");
 	 lcd.print(A_axis[0],HEX);
	 lcd.print(" ") ;
	 I2c.read( ACCELLADDR, A_CTRL3,1, &A_axis[0] ) ;
 	 lcd.print(A_axis[0],HEX);
     break;
     }
   case btnDOWN:
     {
	 lcd.setCursor(0,1);            // move to the beginning of the second line	 	 
     lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
	 lcd.setCursor(0,1);            // move to the beginning of the second line	 	 
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     break;
     }
	 delay(1000) ;
 }
 
}

