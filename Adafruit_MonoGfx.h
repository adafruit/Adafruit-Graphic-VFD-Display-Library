#ifndef _ADAFRUIT_MONOGFX_H
#define _ADAFRUIT_MONOGFX_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

class Adafruit_MonoGfx {
 public:
  // this must be defined by the subclass
  virtual void drawPixel(uint8_t x, uint8_t y, uint8_t color);

  // these are 'generic' drawing functions, so we can share them!
  void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
		uint8_t color);
  virtual void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color);
  void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		uint8_t color);
  void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		uint8_t color);

  void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, 
		  uint8_t color);
  void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, 
		  uint8_t color);

  void drawBitmap(uint8_t x, uint8_t y, 
		  const uint8_t *bitmap, uint8_t w, uint8_t h,
		  uint8_t color);


  // return the size of the display
  uint8_t width() { return WIDTH; }
  uint8_t height() { return HEIGHT; }

 protected:
  uint8_t WIDTH, HEIGHT;


};

#endif
