/* Time convert website : http://www.onlineconversion.com/unix_time.htm
   Components -
   1.RTC 1307 or 3231
   2.LCD with I2C Module

*/
#include<Wire.h>
#include<Time.h>
#include<DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#define TIME_MSG_LEN 11
#define TIME_HEADER 'T'

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
const byte NumberofFields = 7;

int Second;
int Minute;
int Hour;
int Day;
int Wday;
int Month;
int Year;

byte bar1[8] = {
  B11100,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11100
};
byte bar2[8] = {
  B00111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B00111
};
byte bar3[8] = {
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte bar4[8] = {
  B11110,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,
  B11100
};
byte bar5[8] = {
  B01111,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00111
};
byte bar6[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte bar7[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
  B01111
};
byte bar8[8] = {
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void custom0(int col) {
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(8);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(2);
  lcd.write(6);
  lcd.write(1);
}

void custom1(int col) {
  lcd.setCursor(col, 0);
  lcd.write(8);
  lcd.write(1);
  lcd.setCursor(col + 1, 1);
  lcd.write(1);
}

void custom2(int col) {
  lcd.setCursor(col, 0);
  lcd.write(5);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(2);
  lcd.write(6);
  lcd.write(6);
}

void custom3(int col) {
  lcd.setCursor(col, 0);
  lcd.write(5);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(6);
  lcd.write(1);
}

void custom4(int col) {
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(6);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(32);
  lcd.write(32);
  lcd.write(1);
}

void custom5(int col) {
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(6);
  lcd.write(1);
}

void custom6(int col) {
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.setCursor(col, 1);
  lcd.write(2);
  lcd.write(6);
  lcd.write(1);
}

void custom7(int col) {
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(8);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(32);
  lcd.write(32);
  lcd.write(1);
}

void custom8(int col) {
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(2);
  lcd.write(6);
  lcd.write(1);
}

void custom9(int col)
{
  lcd.setCursor(col, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(1);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(6);
  lcd.write(1);
}

void setup() {
  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.createChar(1, bar1);
  lcd.createChar(2, bar2);
  lcd.createChar(3, bar3);
  lcd.createChar(4, bar4);
  lcd.createChar(5, bar5);
  lcd.createChar(6, bar6);
  lcd.createChar(7, bar7);
  lcd.createChar(8, bar8);
  Serial.begin(9600);
  Wire.begin();
  setSyncProvider(RTC.get);
}

void loop() {
  if (Serial.available()) {
    time_t t = processSyncMessage();
    if (t > 0) {
      RTC.set(t);
      setTime(t);
    }
  }
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write(0x00);
  Wire.endTransmission();
  // request the 7 data fields (secs,min,hr,dow,date,mth,yr)
  Wire.requestFrom(DS1307_CTRL_ID, NumberofFields);

  Second = bcd2dec(Wire.read() & 0x7f);
  Minute = bcd2dec(Wire.read());
  Hour   = bcd2dec(Wire.read() & 0x3f);
  Wday   = bcd2dec(Wire.read());
  Day    = bcd2dec(Wire.read());
  Month  = bcd2dec(Wire.read());
  Year   = bcd2dec(Wire.read());
  Year   = Year + 2000; // RTC year 0 is year 2000

  if (timeStatus() != timeSet) {
    lcd.print("unable to sync with RTC");
  }
  else {
    digitalClockDisplay();
    delay(10000);
  }
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }

  lcd.clear();
  updateDisplaydate();
  delay(5000);

  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    lcd.scrollDisplayRight();
    delay(300);
  }
  lcd.clear();
}

// Convert Binary Coded Decimal (BCD) to Decimal
byte bcd2dec(byte num) {
  return ((num / 16 * 10) + (num % 16));
}

void zero(int n, int c) {
  custom0(c);
}
void one(int c) {
  custom1(c);
}
void two(int c) {
  custom2(c);
}
void three(int c) {
  custom3(c);
}
void four(int c) {
  custom4(c);
}
void five(int c) {
  custom5(c);
}

void zeroto9(int n, int c, int o) {
  switch (n) {
    case 0:
      custom0(c);
      break;
    case 1:
      custom1(c + o);
      break;
    case 2:
      custom2(c);
      break;
    case 3:
      custom3(c);
      break;
    case 4:
      custom4(c);
      break;
    case 5:
      custom5(c);
      break;
    case 6:
      custom6(c);
      break;
    case 7:
      custom7(c);
      break;
    case 8:
      custom8(c);
      break;
    case 9:
      custom9(c);
      break;
  }
}

void tento11(int n, int c, int o) {
  switch (n) {
    case 10:
      custom1(c);
      custom0(c + o);
      break;
    case 11:
      custom1(c);
      custom1(c + o);
      break;
  }
}

void digitalClockDisplay() {
  int h = Hour;
  int m = Minute;
  int s = Second;

  ////////// for hour display //////////

  if (h == 0) {
    one(0);
    two(2);
    lcd.setCursor(5, 0);
    lcd.print("PM");
    lcd.setCursor(5, 1);
    lcd.print("Hr");
  }
  if (h > 12 && h < 25) { ///// for 13 to 24 hour /////
    int hr = h - 12;
    int n = hr;
    if (n > 0 && n < 10) { ///// 1 to 9 /////
      zeroto9(n, 1, 1);
    }
    else if (n > 9 && n < 12) { //// 10 to 11 /////
      tento11(n, 0, 2);
    }
    lcd.setCursor(5, 0);
    lcd.print("PM");
  }

  if (h > 0 && h < 13) { ///// 1hr to 12hr /////
    int n = h;
    if (n > 0 && n < 10) {
      zeroto9(n, 1, 1);
    }
    else if (n > 9 && n < 13) {
      if (n == 10) {
        custom1(0);
        custom0(2);
      }
      if (n == 11) {
        custom1(0);
        custom1(2);
      }
      if (n == 12) {
        custom1(0);
        custom2(2);
      }
    }
    lcd.setCursor(5, 0);
    lcd.print("AM");
  }
  lcd.setCursor(5, 1);
  lcd.print("Hr");

  ////////// For Minute Display //////////

  int n = m;
  int a = n - 10;
  if (n == 0) {
    zero(0, 8);
    zero(0, 11);
    lcd.setCursor(14, 1);
    lcd.print("Mn");
  }
  if (n > 0 && n < 10) { //1 to 9
    zero(0, 8);
    zeroto9(n, 11, 0);
    lcd.setCursor(14, 1);
    lcd.print("Mn");
  }
  if (n > 9 && n < 20) { //10 to 19
    one(8);
    zeroto9(a, 10, 1);
    lcd.setCursor(14, 1);
    lcd.print("Mn");
  }
  if (n > 19 && n < 30) { //20 to 29
    a = a - 10;
    two(8);
    zeroto9(a, 11, 1);
    lcd.setCursor(14, 1);
    lcd.print("Mn");
  }
  if (n > 29 && n < 40) { //30 to 39
    a = a - 20;
    three(8);
    zeroto9(a, 11, 1);
    lcd.setCursor(14, 1);
    lcd.print("Mn");
  }
  if (n > 39 && n < 50) { //40 to 49
    a = a - 30;
    four(8);
    zeroto9(a, 11, 1);
    lcd.setCursor(14, 1);
    lcd.print("Mn");
  }
  if (n > 49 && n < 60) { //// 50 to 59 /////
    a = a - 40;
    five(8);
    zeroto9(a, 11, 1);
    lcd.setCursor(14, 1);
    lcd.print("Mn");
  }
}

void updateDisplaydate() {
  int days = Day;
  int months = Month;
  int years = Year;
  int dayofweek = Wday;

  ///// for dayofweek display /////

  if (dayofweek == 0) {
    lcd.setCursor(4, 0);
    lcd.print("Saturday");
  }
  if (dayofweek == 1) {
    lcd.setCursor(4, 0);
    lcd.print("Sunday");
  }
  if (dayofweek == 2) {
    lcd.setCursor(4, 0);
    lcd.print("Monday");
  }
  if (dayofweek == 3) {
    lcd.setCursor(4, 0);
    lcd.print("Tuesday");
  }
  if (dayofweek == 4) {
    lcd.setCursor(4, 0);
    lcd.print("Wednesday");
  }
  if (dayofweek == 5) {
    lcd.setCursor(4, 0);
    lcd.print("Thursday");
  }
  if (dayofweek == 6) {
    lcd.setCursor(4, 0);
    lcd.print("Friday");
  }
  
  ///// for date display /////
  
  if (days > 0 && days < 10) {
    lcd.setCursor(4, 1);
    lcd.print(days);
  }
  else if (days > 9 && days < 32) {
    lcd.setCursor(3, 1);
    lcd.print(days);
  }
  lcd.setCursor(5, 1);
  lcd.print(".");
  if (months > 0 && months < 10) {
    lcd.setCursor(6, 1);
    lcd.print("0");
    lcd.setCursor(7, 1);
    lcd.print(months);
  }
  if (months > 9 && months < 13) {
    lcd.setCursor(6, 1);
    lcd.print(months);
  }
  lcd.setCursor(8, 1);
  lcd.print(".");
  lcd.setCursor(9, 1);
  lcd.print(years);
}

time_t processSyncMessage() {
  while (Serial.available() >= TIME_MSG_LEN) {
    char c = Serial.read();
    Serial.print(c);
    if (c == TIME_HEADER) {
      time_t pctime = 0;
      for (int i = 0; i < TIME_MSG_LEN - 1; i++) {
        c = Serial.read();
        if (c >= '0' && c <= '9') {
          pctime = (10 * pctime) + (c - '0');
        }
      }
      return pctime;
    }
  }
  return 0;
}
