#include "Adafruit_MonoGfx.h"

// draw a circle outline
void Adafruit_MonoGfx::drawCircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
    
  }
}

void Adafruit_MonoGfx::fillCircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  drawFastVLine(x0, y0-r, r*2+1, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawFastVLine(x0+x, y0-y, y*2+1, color);
    drawFastVLine(x0-x, y0-y, y*2+1, color);

    drawFastVLine(x0+y, y0-x, x*2+1, color);
    drawFastVLine(x0-y, y0-x, x*2+1, color);
  }
}

// bresenham's algorithm - thx wikpedia
void Adafruit_MonoGfx::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
		      uint8_t color) {
  uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  uint8_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}


// draw a rectangle
void Adafruit_MonoGfx::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {
  // very stupid version, but works!
  for (uint8_t i=x; i<x+w; i++) {
    drawPixel(i, y, color);
    drawPixel(i, y+h-1, color);
  }
  for (uint8_t i=y; i<y+h; i++) {
    drawPixel(x, i, color);
    drawPixel(x+w-1, i, color);
  } 
}

void Adafruit_MonoGfx::drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color) {
  // stupidest version
  for (uint8_t j=y; j<y+h; j++) {
    drawPixel(x, j, color);
  }
}

void Adafruit_MonoGfx::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {
  // stupidest version
  for (uint8_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color); 
  }
}

void Adafruit_MonoGfx::drawBitmap(uint8_t x, uint8_t y, 
			const uint8_t *bitmap, uint8_t w, uint8_t h,
			uint8_t color) {
  for (uint8_t j=0; j<h; j++) {
    for (uint8_t i=0; i<w; i++ ) {
      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
	drawPixel(x+i, y+j, color);
      }
    }
  }
}
