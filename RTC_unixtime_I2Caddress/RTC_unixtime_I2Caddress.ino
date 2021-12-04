#include<Wire.h>
#define TIME_MSG_LEN 11
#define TIME_HEADER 'T'
#include<Time.h>
#include<DS1307RTC.h>
#include <LiquidCrystal_I2C.h> 

#define I2C_ADDR 0x27
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
const byte DS1307_CTRL_ID = 0x68;
const byte NumberofFields=7;

int Second;
int Minute;
int Hour;
int Day;
int Wday;
int Month;
int Year;

void setup()
  { 
    lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
    Serial.begin(9600);
    Wire.begin();
    setSyncProvider(RTC.get);
   
   
  }

void loop()
  { 
    if(Serial.available())
    {
      time_t t=processSyncMessage();
      if(t>0)
        {
          RTC.set(t);
          setTime(t); 
        }
     
    }
    
    Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write(0x00);
    Wire.endTransmission();
     // request the 7 data fields (secs,min,hr,dow,date,mth,yr)
    Wire.requestFrom(DS1307_CTRL_ID,NumberofFields);

    Second = bcd2dec(Wire.read()&0x7f);
    Minute = bcd2dec(Wire.read());
    Hour   = bcd2dec(Wire.read()& 0x3f);
    Wday   = bcd2dec(Wire.read());
    Day    = bcd2dec(Wire.read());
    Month  = bcd2dec(Wire.read());
    Year   = bcd2dec(Wire.read());
    Year   = Year+2000; // RTC year 0 is year 2000

     if(timeStatus()!=timeSet)
      lcd.print("unable to sync with RTC");
    else
    digitalClockDisplay();
    delay(1000);
  }

// Convert Binary Coded Decimal (BCD) to Decimal 
byte bcd2dec(byte num)
  {
    return((num/16*10)+(num%16));
  }

void digitalClockDisplay()
  {

 lcd.setCursor(0,0);
 lcd.print("Time:");
 lcd.setCursor(7,0);
 lcd.print(Hour);
 printDigits(Minute);
 printDigits(Second);
 lcd.print(" ");

 lcd.setCursor(0,1);
 lcd.print("Date:");
 lcd.setCursor(6,1);
 lcd.print(Day);
 lcd.print(".");
 lcd.print(Month);
 lcd.print(".");
 lcd.print(Year);



  }

void printDigits(int digits)
  {
    
   lcd.print(":");
    if(digits<10)

    lcd.print('0');
   lcd.print(digits);
  }

time_t processSyncMessage(){
while(Serial.available()>= TIME_MSG_LEN)
    {
      char c=Serial.read();
      Serial.print(c);
      if(c==TIME_HEADER)
      {
        time_t pctime=0;
        for(int i=0;i<TIME_MSG_LEN-1;i++)
          {
            c=Serial.read();
            if(c>='0' && c<='9')
              {
                pctime=(10*pctime)+(c-'0');
              }
          }
          return pctime;
      }
    }
    return 0;
}
