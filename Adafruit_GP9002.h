#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 0
#define WHITE 1

  #define GP9002_LCDWIDTH                  128
  #define GP9002_LCDHEIGHT                 64

#define GP9002_SETCONTRAST 0x81
#define GP9002_DISPLAYALLON_RESUME 0xA4
#define GP9002_DISPLAYALLON 0xA5
#define GP9002_NORMALDISPLAY 0xA6
#define GP9002_INVERTDISPLAY 0xA7
#define GP9002_DISPLAYOFF 0xAE
#define GP9002_DISPLAYON 0xAF

#define GP9002_SETDISPLAYOFFSET 0xD3
#define GP9002_SETCOMPINS 0xDA

#define GP9002_SETVCOMDETECT 0xDB

#define GP9002_SETDISPLAYCLOCKDIV 0xD5
#define GP9002_SETPRECHARGE 0xD9

#define GP9002_SETMULTIPLEX 0xA8

#define GP9002_SETLOWCOLUMN 0x00
#define GP9002_SETHIGHCOLUMN 0x10

#define GP9002_SETSTARTLINE 0x40

#define GP9002_MEMORYMODE 0x20

#define GP9002_COMSCANINC 0xC0
#define GP9002_COMSCANDEC 0xC8

#define GP9002_SEGREMAP 0xA0

#define GP9002_CHARGEPUMP 0x8D

#define GP9002_EXTERNALVCC 0x1
#define GP9002_SWITCHCAPVCC 0x2

class Adafruit_GP9002 {
 public:
  Adafruit_GP9002(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS) :sid(SID), sclk(SCLK), dc(DC), rst(RST), cs(CS) {}
  Adafruit_GP9002(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST) :sid(SID), sclk(SCLK), dc(DC), rst(RST), cs(-1) {}


  void init(uint8_t switchvcc);
  void command(uint8_t c);
  void data(uint8_t c);
  void setBrightness(uint8_t val);
  void clearDisplay(void);
  void clear();
  void invert(uint8_t i);
  void display();

  void setpixel(uint8_t x, uint8_t y, uint8_t color);
  void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, 
		  uint8_t color);
  void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, 
		  uint8_t color);
  void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		uint8_t color);
  void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		uint8_t color);
  void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
		uint8_t color);
  void drawChar(uint8_t x, uint8_t line, uint8_t c);
  void drawString(uint8_t x, uint8_t line, char *c);

  void drawBitmap(uint8_t x, uint8_t y, 
		  const uint8_t *bitmap, uint8_t w, uint8_t h,
		  uint8_t color);

 private:
  int8_t sid, sclk, dc, rst, cs;
  void spiwrite(uint8_t c);

};
