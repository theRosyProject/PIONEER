
#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "RTClib.h"
#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#elif defined(ARDUINO_ARCH_SAMD)
#elif defined(ARDUINO_SAM_DUE)
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define Wire Wire1
#endif

#if (ARDUINO >= 100)
#include <Arduino.h> 

#define _I2C_WRITE write 
#define _I2C_READ read   
#else
#include <WProgram.h>
#define _I2C_WRITE send  
#define _I2C_READ receive 
#endif

static uint8_t read_i2c_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire._I2C_WRITE((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, (byte)1);
  return Wire._I2C_READ();
}

static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire._I2C_WRITE((byte)reg);
  Wire._I2C_WRITE((byte)val);
  Wire.endTransmission();
}

const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
  if (y >= 2000)
    y -= 2000;
  uint16_t days = d;
  for (uint8_t i = 1; i < m; ++i)
    days += pgm_read_byte(daysInMonth + i - 1);
  if (m > 2 && y % 4 == 0)
    ++days;
  return days + 365 * y + (y + 3) / 4 - 1;
}


static uint32_t time2ulong(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
  return ((days * 24UL + h) * 60 + m) * 60 + s;
}

DateTime::DateTime(uint32_t t) {
  t -= SECONDS_FROM_1970_TO_2000;

  ss = t % 60;
  t /= 60;
  mm = t % 60;
  t /= 60;
  hh = t % 24;
  uint16_t days = t / 24;
  uint8_t leap;
  for (yOff = 0;; ++yOff) {
    leap = yOff % 4 == 0;
    if (days < 365U + leap)
      break;
    days -= 365 + leap;
  }
  for (m = 1; m < 12; ++m) {
    uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
    if (leap && m == 2)
      ++daysPerMonth;
    if (days < daysPerMonth)
      break;
    days -= daysPerMonth;
  }
  d = days + 1;
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                   uint8_t min, uint8_t sec) {
  if (year >= 2000)
    year -= 2000;
  yOff = year;
  m = month;
  d = day;
  hh = hour;
  mm = min;
  ss = sec;
}

DateTime::DateTime(const DateTime &copy)
    : yOff(copy.yOff), m(copy.m), d(copy.d), hh(copy.hh), mm(copy.mm),
      ss(copy.ss) {}

static uint8_t conv2d(const char *p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

DateTime::DateTime(const char *date, const char *time) {
  yOff = conv2d(date + 9);
  switch (date[0]) {
  case 'J':
    m = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7);
    break;
  case 'F':
    m = 2;
    break;
  case 'A':
    m = date[2] == 'r' ? 4 : 8;
    break;
  case 'M':
    m = date[2] == 'r' ? 3 : 5;
    break;
  case 'S':
    m = 9;
    break;
  case 'O':
    m = 10;
    break;
  case 'N':
    m = 11;
    break;
  case 'D':
    m = 12;
    break;
  }
  d = conv2d(date + 4);
  hh = conv2d(time);
  mm = conv2d(time + 3);
  ss = conv2d(time + 6);
}

DateTime::DateTime(const __FlashStringHelper *date,
                   const __FlashStringHelper *time) {
  char buff[11];
  memcpy_P(buff, date, 11);
  yOff = conv2d(buff + 9);
  switch (buff[0]) {
  case 'J':
    m = (buff[1] == 'a') ? 1 : ((buff[2] == 'n') ? 6 : 7);
    break;
  case 'F':
    m = 2;
    break;
  case 'A':
    m = buff[2] == 'r' ? 4 : 8;
    break;
  case 'M':
    m = buff[2] == 'r' ? 3 : 5;
    break;
  case 'S':
    m = 9;
    break;
  case 'O':
    m = 10;
    break;
  case 'N':
    m = 11;
    break;
  case 'D':
    m = 12;
    break;
  }
  d = conv2d(buff + 4);
  memcpy_P(buff, time, 8);
  hh = conv2d(buff);
  mm = conv2d(buff + 3);
  ss = conv2d(buff + 6);
}

bool DateTime::isValid() const {
  if (yOff >= 100)
    return false;
  DateTime other(unixtime());
  return yOff == other.yOff && m == other.m && d == other.d && hh == other.hh &&
         mm == other.mm && ss == other.ss;
}

char *DateTime::toString(char *buffer) {
  uint8_t apTag =
      (strstr(buffer, "ap") != nullptr) || (strstr(buffer, "AP") != nullptr);
  uint8_t hourReformatted, isPM;
  if (apTag) {     
    if (hh == 0) { 
      isPM = false;
      hourReformatted = 12;
    } else if (hh == 12) {
      isPM = true;
      hourReformatted = 12;
    } else if (hh < 12) {
      isPM = false;
      hourReformatted = hh;
    } else {
      isPM = true;
      hourReformatted = hh - 12;
    }
  }

  for (size_t i = 0; i < strlen(buffer) - 1; i++) {
    if (buffer[i] == 'h' && buffer[i + 1] == 'h') {
      if (!apTag) {
        buffer[i] = '0' + hh / 10;
        buffer[i + 1] = '0' + hh % 10;
      } else {
        buffer[i] = '0' + hourReformatted / 10;
        buffer[i + 1] = '0' + hourReformatted % 10;
      }
    }
    if (buffer[i] == 'm' && buffer[i + 1] == 'm') {
      buffer[i] = '0' + mm / 10;
      buffer[i + 1] = '0' + mm % 10;
    }
    if (buffer[i] == 's' && buffer[i + 1] == 's') {
      buffer[i] = '0' + ss / 10;
      buffer[i + 1] = '0' + ss % 10;
    }
    if (buffer[i] == 'D' && buffer[i + 1] == 'D' && buffer[i + 2] == 'D') {
      static PROGMEM const char day_names[] = "SunMonTueWedThuFriSat";
      const char *p = &day_names[3 * dayOfTheWeek()];
      buffer[i] = pgm_read_byte(p);
      buffer[i + 1] = pgm_read_byte(p + 1);
      buffer[i + 2] = pgm_read_byte(p + 2);
    } else if (buffer[i] == 'D' && buffer[i + 1] == 'D') {
      buffer[i] = '0' + d / 10;
      buffer[i + 1] = '0' + d % 10;
    }
    if (buffer[i] == 'M' && buffer[i + 1] == 'M' && buffer[i + 2] == 'M') {
      static PROGMEM const char month_names[] =
          "JanFebMarAprMayJunJulAugSepOctNovDec";
      const char *p = &month_names[3 * (m - 1)];
      buffer[i] = pgm_read_byte(p);
      buffer[i + 1] = pgm_read_byte(p + 1);
      buffer[i + 2] = pgm_read_byte(p + 2);
    } else if (buffer[i] == 'M' && buffer[i + 1] == 'M') {
      buffer[i] = '0' + m / 10;
      buffer[i + 1] = '0' + m % 10;
    }
    if (buffer[i] == 'Y' && buffer[i + 1] == 'Y' && buffer[i + 2] == 'Y' &&
        buffer[i + 3] == 'Y') {
      buffer[i] = '2';
      buffer[i + 1] = '0';
      buffer[i + 2] = '0' + (yOff / 10) % 10;
      buffer[i + 3] = '0' + yOff % 10;
    } else if (buffer[i] == 'Y' && buffer[i + 1] == 'Y') {
      buffer[i] = '0' + (yOff / 10) % 10;
      buffer[i + 1] = '0' + yOff % 10;
    }
    if (buffer[i] == 'A' && buffer[i + 1] == 'P') {
      if (isPM) {
        buffer[i] = 'P';
        buffer[i + 1] = 'M';
      } else {
        buffer[i] = 'A';
        buffer[i + 1] = 'M';
      }
    } else if (buffer[i] == 'a' && buffer[i + 1] == 'p') {
      if (isPM) {
        buffer[i] = 'p';
        buffer[i + 1] = 'm';
      } else {
        buffer[i] = 'a';
        buffer[i + 1] = 'm';
      }
    }
  }
  return buffer;
}

uint8_t DateTime::twelveHour() const {
  if (hh == 0 || hh == 12) { 
    return 12;
  } else if (hh > 12) { 
    return hh - 12;
  } else { 
    return hh;
  }
}

uint8_t DateTime::dayOfTheWeek() const {
  uint16_t day = date2days(yOff, m, d);
  return (day + 6) % 7;
}

uint32_t DateTime::unixtime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2ulong(days, hh, mm, ss);
  t += SECONDS_FROM_1970_TO_2000; 

  return t;
}

uint32_t DateTime::secondstime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2ulong(days, hh, mm, ss);
  return t;
}

DateTime DateTime::operator+(const TimeSpan &span) {
  return DateTime(unixtime() + span.totalseconds());
}

DateTime DateTime::operator-(const TimeSpan &span) {
  return DateTime(unixtime() - span.totalseconds());
}

TimeSpan DateTime::operator-(const DateTime &right) {
  return TimeSpan(unixtime() - right.unixtime());
}

bool DateTime::operator<(const DateTime &right) const {
  return unixtime() < right.unixtime();
}

bool DateTime::operator==(const DateTime &right) const {
  return unixtime() == right.unixtime();
}

String DateTime::timestamp(timestampOpt opt) {
  char buffer[20];

  switch (opt) {
  case TIMESTAMP_TIME:
    sprintf(buffer, "%02d:%02d:%02d", hh, mm, ss);
    break;
  case TIMESTAMP_DATE:
    sprintf(buffer, "%d-%02d-%02d", 2000 + yOff, m, d);
    break;
  default:
    sprintf(buffer, "%d-%02d-%02dT%02d:%02d:%02d", 2000 + yOff, m, d, hh, mm,
            ss);
  }
  return String(buffer);
}

TimeSpan::TimeSpan(int32_t seconds) : _seconds(seconds) {}

TimeSpan::TimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds)
    : _seconds((int32_t)days * 86400L + (int32_t)hours * 3600 +
               (int32_t)minutes * 60 + seconds) {}

TimeSpan::TimeSpan(const TimeSpan &copy) : _seconds(copy._seconds) {}

TimeSpan TimeSpan::operator+(const TimeSpan &right) {
  return TimeSpan(_seconds + right._seconds);
}

TimeSpan TimeSpan::operator-(const TimeSpan &right) {
  return TimeSpan(_seconds - right._seconds);
}

static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }

static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }

boolean RTC_DS1307::begin(void) {
  Wire.begin();
  Wire.beginTransmission(DS1307_ADDRESS);
  if (Wire.endTransmission() == 0)
    return true;
  return false;
}

uint8_t RTC_DS1307::isrunning(void) {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire._I2C_WRITE((byte)0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 1);
  uint8_t ss = Wire._I2C_READ();
  return !(ss >> 7);
}

void RTC_DS1307::adjust(const DateTime &dt) {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire._I2C_WRITE((byte)0); 
  Wire._I2C_WRITE(bin2bcd(dt.second()));
  Wire._I2C_WRITE(bin2bcd(dt.minute()));
  Wire._I2C_WRITE(bin2bcd(dt.hour()));
  Wire._I2C_WRITE(bin2bcd(0));
  Wire._I2C_WRITE(bin2bcd(dt.day()));
  Wire._I2C_WRITE(bin2bcd(dt.month()));
  Wire._I2C_WRITE(bin2bcd(dt.year() - 2000));
  Wire.endTransmission();
}

DateTime RTC_DS1307::now() {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire._I2C_WRITE((byte)0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);
  uint8_t ss = bcd2bin(Wire._I2C_READ() & 0x7F);
  uint8_t mm = bcd2bin(Wire._I2C_READ());
  uint8_t hh = bcd2bin(Wire._I2C_READ());
  Wire._I2C_READ();
  uint8_t d = bcd2bin(Wire._I2C_READ());
  uint8_t m = bcd2bin(Wire._I2C_READ());
  uint16_t y = bcd2bin(Wire._I2C_READ()) + 2000;

  return DateTime(y, m, d, hh, mm, ss);
}

Ds1307SqwPinMode RTC_DS1307::readSqwPinMode() {
  int mode;

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire._I2C_WRITE(DS1307_CONTROL);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)DS1307_ADDRESS, (uint8_t)1);
  mode = Wire._I2C_READ();

  mode &= 0x93;
  return static_cast<Ds1307SqwPinMode>(mode);
}

void RTC_DS1307::writeSqwPinMode(Ds1307SqwPinMode mode) {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire._I2C_WRITE(DS1307_CONTROL);
  Wire._I2C_WRITE(mode);
  Wire.endTransmission();
}

void RTC_DS1307::readnvram(uint8_t *buf, uint8_t size, uint8_t address) {
  int addrByte = DS1307_NVRAM + address;
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire._I2C_WRITE(addrByte);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)DS1307_ADDRESS, size);
  for (uint8_t pos = 0; pos < size; ++pos) {
    buf[pos] = Wire._I2C_READ();
  }
}

void RTC_DS1307::writenvram(uint8_t address, uint8_t *buf, uint8_t size) {
  int addrByte = DS1307_NVRAM + address;
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire._I2C_WRITE(addrByte);
  for (uint8_t pos = 0; pos < size; ++pos) {
    Wire._I2C_WRITE(buf[pos]);
  }
  Wire.endTransmission();
}

uint8_t RTC_DS1307::readnvram(uint8_t address) {
  uint8_t data;
  readnvram(&data, 1, address);
  return data;
}

void RTC_DS1307::writenvram(uint8_t address, uint8_t data) {
  writenvram(address, &data, 1);
}

uint32_t RTC_Millis::lastMillis;
uint32_t RTC_Millis::lastUnix;

void RTC_Millis::adjust(const DateTime &dt) {
  lastMillis = millis();
  lastUnix = dt.unixtime();
}

DateTime RTC_Millis::now() {
  uint32_t elapsedSeconds = (millis() - lastMillis) / 1000;
  lastMillis += elapsedSeconds * 1000;
  lastUnix += elapsedSeconds;
  return lastUnix;
}


uint32_t RTC_Micros::microsPerSecond = 1000000;

uint32_t RTC_Micros::lastMicros;
uint32_t RTC_Micros::lastUnix;

void RTC_Micros::adjust(const DateTime &dt) {
  lastMicros = micros();
  lastUnix = dt.unixtime();
}

void RTC_Micros::adjustDrift(int ppm) { microsPerSecond = 1000000 - ppm; }

DateTime RTC_Micros::now() {
  uint32_t elapsedSeconds = (micros() - lastMicros) / microsPerSecond;
  lastMicros += elapsedSeconds * microsPerSecond;
  lastUnix += elapsedSeconds;
  return lastUnix;
}

boolean RTC_PCF8523::begin(void) {
  Wire.begin();
  Wire.beginTransmission(PCF8523_ADDRESS);
  if (Wire.endTransmission() == 0)
    return true;
  return false;
}

boolean RTC_PCF8523::lostPower(void) {
  return (read_i2c_register(PCF8523_ADDRESS, PCF8523_STATUSREG) >> 7);
}

boolean RTC_PCF8523::initialized(void) {
  Wire.beginTransmission(PCF8523_ADDRESS);
  Wire._I2C_WRITE((byte)PCF8523_CONTROL_3);
  Wire.endTransmission();

  Wire.requestFrom(PCF8523_ADDRESS, 1);
  uint8_t ss = Wire._I2C_READ();
  return ((ss & 0xE0) != 0xE0);
}

void RTC_PCF8523::adjust(const DateTime &dt) {
  Wire.beginTransmission(PCF8523_ADDRESS);
  Wire._I2C_WRITE((byte)3);
  Wire._I2C_WRITE(bin2bcd(dt.second()));
  Wire._I2C_WRITE(bin2bcd(dt.minute()));
  Wire._I2C_WRITE(bin2bcd(dt.hour()));
  Wire._I2C_WRITE(bin2bcd(dt.day()));
  Wire._I2C_WRITE(bin2bcd(0));
  Wire._I2C_WRITE(bin2bcd(dt.month()));
  Wire._I2C_WRITE(bin2bcd(dt.year() - 2000));
  Wire.endTransmission();

  Wire.beginTransmission(PCF8523_ADDRESS);
  Wire._I2C_WRITE((byte)PCF8523_CONTROL_3);
  Wire._I2C_WRITE((byte)0x00);
  Wire.endTransmission();
}

DateTime RTC_PCF8523::now() {
  Wire.beginTransmission(PCF8523_ADDRESS);
  Wire._I2C_WRITE((byte)3);
  Wire.endTransmission();

  Wire.requestFrom(PCF8523_ADDRESS, 7);
  uint8_t ss = bcd2bin(Wire._I2C_READ() & 0x7F);
  uint8_t mm = bcd2bin(Wire._I2C_READ());
  uint8_t hh = bcd2bin(Wire._I2C_READ());
  uint8_t d = bcd2bin(Wire._I2C_READ());
  Wire._I2C_READ();
  uint8_t m = bcd2bin(Wire._I2C_READ());
  uint16_t y = bcd2bin(Wire._I2C_READ()) + 2000;

  return DateTime(y, m, d, hh, mm, ss);
}

Pcf8523SqwPinMode RTC_PCF8523::readSqwPinMode() {
  int mode;

  Wire.beginTransmission(PCF8523_ADDRESS);
  Wire._I2C_WRITE(PCF8523_CLKOUTCONTROL);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)PCF8523_ADDRESS, (uint8_t)1);
  mode = Wire._I2C_READ();

  mode >>= 3;
  mode &= 0x7;
  return static_cast<Pcf8523SqwPinMode>(mode);
}

void RTC_PCF8523::writeSqwPinMode(Pcf8523SqwPinMode mode) {
  Wire.beginTransmission(PCF8523_ADDRESS);
  Wire._I2C_WRITE(PCF8523_CLKOUTCONTROL);
  Wire._I2C_WRITE(mode << 3); 
  Wire.endTransmission();
}

void RTC_PCF8523::enableSecondTimer() {
  uint8_t ctlreg = read_i2c_register(PCF8523_ADDRESS, PCF8523_CONTROL_1);
  uint8_t clkreg = read_i2c_register(PCF8523_ADDRESS, PCF8523_CLKOUTCONTROL);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_CLKOUTCONTROL, clkreg | 0xB8);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_CONTROL_1, ctlreg | (1 << 2));
}

void RTC_PCF8523::disableSecondTimer() {
  uint8_t ctlreg = read_i2c_register(PCF8523_ADDRESS, PCF8523_CONTROL_1);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_CONTROL_1, ctlreg & ~(1 << 2));
}

void RTC_PCF8523::enableCountdownTimer(PCF8523TimerClockFreq clkFreq,
                                       uint8_t numPeriods,
                                       uint8_t lowPulseWidth) {
  disableCountdownTimer();

  uint8_t ctlreg = read_i2c_register(PCF8523_ADDRESS, PCF8523_CONTROL_2);
  uint8_t clkreg = read_i2c_register(PCF8523_ADDRESS, PCF8523_CLKOUTCONTROL);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_CONTROL_2, ctlreg |= 0x01);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_TIMER_B_FRCTL,
                     lowPulseWidth << 4 | clkFreq);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_TIMER_B_VALUE, numPeriods);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_CLKOUTCONTROL, clkreg | 0x79);
}

void RTC_PCF8523::enableCountdownTimer(PCF8523TimerClockFreq clkFreq,
                                       uint8_t numPeriods) {
  enableCountdownTimer(clkFreq, numPeriods, 0);
}

void RTC_PCF8523::disableCountdownTimer() {
  uint8_t clkreg = read_i2c_register(PCF8523_ADDRESS, PCF8523_CLKOUTCONTROL);

  write_i2c_register(PCF8523_ADDRESS, PCF8523_CLKOUTCONTROL, ~1 & clkreg);
}

void RTC_PCF8523::deconfigureAllTimers() {
  disableSecondTimer(); 
  write_i2c_register(PCF8523_ADDRESS, PCF8523_CONTROL_2, 0);
  write_i2c_register(PCF8523_ADDRESS, PCF8523_CLKOUTCONTROL, 0);
  write_i2c_register(PCF8523_ADDRESS, PCF8523_TIMER_B_FRCTL, 0);
  write_i2c_register(PCF8523_ADDRESS, PCF8523_TIMER_B_VALUE, 0);
}

void RTC_PCF8523::calibrate(Pcf8523OffsetMode mode, int8_t offset) {
  uint8_t reg = (uint8_t)offset & 0x7F;
  reg |= mode;

  Wire.beginTransmission(PCF8523_ADDRESS);
  Wire._I2C_WRITE(PCF8523_OFFSET);
  Wire._I2C_WRITE(reg);
  Wire.endTransmission();
}

boolean RTC_DS3231::begin(void) {
  Wire.begin();
  Wire.beginTransmission(DS3231_ADDRESS);
  if (Wire.endTransmission() == 0)
    return true;
  return false;
}

bool RTC_DS3231::lostPower(void) {
  return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG) >> 7);
}

void RTC_DS3231::adjust(const DateTime &dt) {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE((byte)DS3231_TIME);
  Wire._I2C_WRITE(bin2bcd(dt.second()));
  Wire._I2C_WRITE(bin2bcd(dt.minute()));
  Wire._I2C_WRITE(bin2bcd(dt.hour()));
  Wire._I2C_WRITE(bin2bcd(0));
  Wire._I2C_WRITE(bin2bcd(dt.day()));
  Wire._I2C_WRITE(bin2bcd(dt.month()));
  Wire._I2C_WRITE(bin2bcd(dt.year() - 2000));
  Wire.endTransmission();

  uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  statreg &= ~0x80;
  write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg);
}

DateTime RTC_DS3231::now() {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE((byte)0);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 7);
  uint8_t ss = bcd2bin(Wire._I2C_READ() & 0x7F);
  uint8_t mm = bcd2bin(Wire._I2C_READ());
  uint8_t hh = bcd2bin(Wire._I2C_READ());
  Wire._I2C_READ();
  uint8_t d = bcd2bin(Wire._I2C_READ());
  uint8_t m = bcd2bin(Wire._I2C_READ());
  uint16_t y = bcd2bin(Wire._I2C_READ()) + 2000;
  
  return DateTime(y, m, d, hh, mm, ss);
}

Ds3231SqwPinMode RTC_DS3231::readSqwPinMode() {
  int mode;

  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE(DS3231_CONTROL);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)DS3231_ADDRESS, (uint8_t)1);
  mode = Wire._I2C_READ();

  mode &= 0x93;
  return static_cast<Ds3231SqwPinMode>(mode);
}

void RTC_DS3231::writeSqwPinMode(Ds3231SqwPinMode mode) {
  uint8_t ctrl;
  ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);

  ctrl &= ~0x04; 
  ctrl &= ~0x18; 

  if (mode == DS3231_OFF) {
    ctrl |= 0x04; 
  } else {
    ctrl |= mode;
  }
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
}

float RTC_DS3231::getTemperature() {
  uint8_t lsb;
  int8_t msb;
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE(DS3231_TEMPERATUREREG);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 2);
  msb = Wire._I2C_READ();
  lsb = Wire._I2C_READ();

  return (float)msb + (lsb >> 6) * 0.25f;
}

bool RTC_DS3231::setAlarm1(const DateTime &dt, Ds3231Alarm1Mode alarm_mode) {
  uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
  if (!(ctrl & 0x04)) {
    return false;
  }

  uint8_t A1M1 = (alarm_mode & 0x01) << 7;
  uint8_t A1M2 = (alarm_mode & 0x02) << 6; 
  uint8_t A1M3 = (alarm_mode & 0x04) << 5; 
  uint8_t A1M4 = (alarm_mode & 0x08) << 4; 
  uint8_t DY_DT = (alarm_mode & 0x10) << 2; 

  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE(DS3231_ALARM1);
  Wire._I2C_WRITE(bin2bcd(dt.second()) | A1M1);
  Wire._I2C_WRITE(bin2bcd(dt.minute()) | A1M2);
  Wire._I2C_WRITE(bin2bcd(dt.hour()) | A1M3);
  if (DY_DT) {
    Wire._I2C_WRITE(bin2bcd(dt.dayOfTheWeek()) | A1M4 | DY_DT);
  } else {
    Wire._I2C_WRITE(bin2bcd(dt.day()) | A1M4 | DY_DT);
  }
  Wire.endTransmission();

  ctrl |= 0x01; 
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
  return true;
}

bool RTC_DS3231::setAlarm2(const DateTime &dt, Ds3231Alarm2Mode alarm_mode) {
  uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
  if (!(ctrl & 0x04)) {
    return false;
  }

  uint8_t A2M2 = (alarm_mode & 0x01) << 7; 
  uint8_t A2M3 = (alarm_mode & 0x02) << 6;
  uint8_t A2M4 = (alarm_mode & 0x04) << 5; 
  uint8_t DY_DT = (alarm_mode & 0x8) << 3;

  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE(DS3231_ALARM2);
  Wire._I2C_WRITE(bin2bcd(dt.minute()) | A2M2);
  Wire._I2C_WRITE(bin2bcd(dt.hour()) | A2M3);
  if (DY_DT) {
    Wire._I2C_WRITE(bin2bcd(dt.dayOfTheWeek()) | A2M4 | DY_DT);
  } else {
    Wire._I2C_WRITE(bin2bcd(dt.day()) | A2M4 | DY_DT);
  }
  Wire.endTransmission();
  ctrl |= 0x02;
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
  return true;
}

void RTC_DS3231::disableAlarm(uint8_t alarm_num) {
  uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
  ctrl &= ~(1 << (alarm_num - 1));
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
}

void RTC_DS3231::clearAlarm(uint8_t alarm_num) {
  uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  status &= ~(0x1 << (alarm_num - 1));
  write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status);
}

bool RTC_DS3231::alarmFired(uint8_t alarm_num) {
  uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  return (status >> (alarm_num - 1)) & 0x1;
}
