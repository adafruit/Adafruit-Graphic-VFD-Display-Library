#ifndef _ADAFRUIT_GP9002_H
#define _ADAFRUIT_GP9002_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Adafruit_MonoGfx.h"

#define BLACK 0
#define WHITE 1

#define GP9002_DISPLAYSOFF 0x00
#define GP9002_DISPLAY1ON 0x01
#define GP9002_DISPLAY2ON 0x02
#define GP9002_ADDRINCR 0x04
#define GP9002_ADDRHELD 0x05
#define GP9002_CLEARSCREEN 0x06
#define GP9002_CONTROLPOWER 0x07
#define GP9002_DATAWRITE 0x08
#define GP9002_DATAREAD 0x09
#define GP9002_LOWERADDR1 0x0A
#define GP9002_HIGHERADDR1 0x0B
#define GP9002_LOWERADDR2 0x0C
#define GP9002_HIGHERADDR1 0x1D
#define GP9002_ADDRL 0x0E
#define GP9002_ADDRH 0x0F
#define GP9002_OR 0x10
#define GP9002_XOR 0x11
#define GP9002_AND 0x12
#define GP9002_BRIGHT 0x13
#define GP9002_DISPLAY 0x14
#define GP9002_DISPLAY_MONOCHROME 0x10
#define GP9002_DISPLAY_GRAYSCALE 0x14
#define GP9002_INTMODE 0x15
#define GP9002_DRAWCHAR 0x20
#define GP9002_CHARRAM 0x21
#define GP9002_CHARSIZE 0x22
#define GP9002_CHARBRIGHT 0x24


class Adafruit_GP9002 : public Adafruit_MonoGfx {
 public:
  Adafruit_GP9002(int8_t SCLK, int8_t MISO, int8_t MOSI, 
		  int8_t CS, int8_t DC);

  // particular to this display
  void begin(void);
  uint8_t spiTransfer(uint8_t d, uint8_t datacommand);
  void command(uint8_t c);
  uint8_t data(uint8_t c);
  void setBrightness(uint8_t val);
  void invert(uint8_t i);

  void displayOn();
  void displayOff();
  void clearDisplay(void);

  void drawPixel(uint8_t x, uint8_t y, uint8_t color);
  void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color);

  void drawChar(uint8_t x, uint8_t line, uint8_t c);
  void drawString(uint8_t x, uint8_t line, char *c);

 private:
  int8_t _miso, _mosi, _sclk, _dc, _cs;
  void spiwrite(uint8_t c);
};

#endif
