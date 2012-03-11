#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "Adafruit_GP9002.h"
#include "glcdfont.c"

static uint8_t is_reversed = 0;

// a 5x7 font table
extern uint8_t PROGMEM font[];


void Adafruit_GP9002::drawbitmap(uint8_t x, uint8_t y, 
			const uint8_t *bitmap, uint8_t w, uint8_t h,
			uint8_t color) {
  for (uint8_t j=0; j<h; j++) {
    for (uint8_t i=0; i<w; i++ ) {
      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
	setpixel(x+i, y+j, color);
      }
    }
  }
}

void Adafruit_GP9002::drawstring(uint8_t x, uint8_t line, char *c) {
  while (c[0] != 0) {
    drawchar(x, line, c[0]);
    c++;
    x += 6; // 6 pixels wide
    if (x + 6 >= Adafruit_GP9002_LCDWIDTH) {
      x = 0;    // ran out of this line
      line++;
    }
    if (line >= (Adafruit_GP9002_LCDHEIGHT/8))
      return;        // ran out of space :(
  }

}

void  Adafruit_GP9002::drawchar(uint8_t x, uint8_t line, uint8_t c) {
  if ((line >= Adafruit_GP9002_LCDHEIGHT/8) || (x >= (Adafruit_GP9002_LCDWIDTH - 6)))
      return;
  for (uint8_t i =0; i<5; i++ ) {
    buffer[x + (line*128) ] = pgm_read_byte(font+(c*5)+i);
    x++;
  }
}


// bresenham's algorithm - thx wikpedia
void Adafruit_GP9002::drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
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
      setpixel(y0, x0, color);
    } else {
      setpixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// filled rectangle
void Adafruit_GP9002::fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {

  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    for (uint8_t j=y; j<y+h; j++) {
      setpixel(i, j, color);
    }
  }
}

// draw a rectangle
void Adafruit_GP9002::drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint8_t color) {
  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    setpixel(i, y, color);
    setpixel(i, y+h-1, color);
  }
  for (uint8_t i=y; i<y+h; i++) {
    setpixel(x, i, color);
    setpixel(x+w-1, i, color);
  } 
}

// draw a circle outline
void Adafruit_GP9002::drawcircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  setpixel(x0, y0+r, color);
  setpixel(x0, y0-r, color);
  setpixel(x0+r, y0, color);
  setpixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    setpixel(x0 + x, y0 + y, color);
    setpixel(x0 - x, y0 + y, color);
    setpixel(x0 + x, y0 - y, color);
    setpixel(x0 - x, y0 - y, color);
    
    setpixel(x0 + y, y0 + x, color);
    setpixel(x0 - y, y0 + x, color);
    setpixel(x0 + y, y0 - x, color);
    setpixel(x0 - y, y0 - x, color);
    
  }
}

void Adafruit_GP9002::fillcircle(uint8_t x0, uint8_t y0, uint8_t r, 
			uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  for (uint8_t i=y0-r; i<=y0+r; i++) {
    setpixel(x0, i, color);
  }

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    for (uint8_t i=y0-y; i<=y0+y; i++) {
      setpixel(x0+x, i, color);
      setpixel(x0-x, i, color);
    } 
    for (uint8_t i=y0-x; i<=y0+x; i++) {
      setpixel(x0+y, i, color);
      setpixel(x0-y, i, color);
    }    
  }
}

// the most basic function, set a single pixel
void Adafruit_GP9002::setpixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= Adafruit_GP9002_LCDWIDTH) || (y >= Adafruit_GP9002_LCDHEIGHT))
    return;

  // x is which column
  if (color == WHITE) 
    buffer[x+ (y/8)*Adafruit_GP9002_LCDWIDTH] |= _BV((y%8));  
  else
    buffer[x+ (y/8)*Adafruit_GP9002_LCDWIDTH] &= ~_BV((y%8)); 
}

void Adafruit_GP9002::ssd1306_init(uint8_t vccstate) {
  // set pin directions
  pinMode(sid, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(dc, OUTPUT);
  pinMode(rst, OUTPUT);
  pinMode(cs, OUTPUT);

  digitalWrite(rst, HIGH);
  // VDD (3.3V) goes high at start, lets just chill for a ms
  delay(1);
  // bring0xset low
  digitalWrite(rst, LOW);
  // wait 10ms
  delay(10);
  // bring out of reset
  digitalWrite(rst, HIGH);
  // turn on VCC (9V?)

   #if defined Adafruit_GP9002_128_32
    // Init sequence for 128x32 OLED module
    ssd1306_command(Adafruit_GP9002_DISPLAYOFF);                    // 0xAE
    ssd1306_command(Adafruit_GP9002_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(Adafruit_GP9002_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x1F);
    ssd1306_command(Adafruit_GP9002_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(Adafruit_GP9002_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(Adafruit_GP9002_CHARGEPUMP);                    // 0x8D
    if (vccstate == Adafruit_GP9002_EXTERNALVCC) 
      { ssd1306_command(0x10); }
    else 
      { ssd1306_command(0x14); }
    ssd1306_command(Adafruit_GP9002_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
	ssd1306_command(Adafruit_GP9002_SEGREMAP | 0x1);
    ssd1306_command(Adafruit_GP9002_COMSCANDEC);
    ssd1306_command(Adafruit_GP9002_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x02);
    ssd1306_command(Adafruit_GP9002_SETCONTRAST);                   // 0x81
    ssd1306_command(0x8F);
    ssd1306_command(Adafruit_GP9002_SETPRECHARGE);                  // 0xd9
    if (vccstate == Adafruit_GP9002_EXTERNALVCC) 
      { ssd1306_command(0x22); }
    else 
      { ssd1306_command(0xF1); }
    ssd1306_command(Adafruit_GP9002_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(Adafruit_GP9002_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(Adafruit_GP9002_NORMALDISPLAY);                 // 0xA6
  #endif

  #if defined Adafruit_GP9002_128_64
    // Init sequence for 128x64 OLED module
    ssd1306_command(Adafruit_GP9002_DISPLAYOFF);                    // 0xAE
    ssd1306_command(Adafruit_GP9002_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(Adafruit_GP9002_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x3F);
    ssd1306_command(Adafruit_GP9002_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(Adafruit_GP9002_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(Adafruit_GP9002_CHARGEPUMP);                    // 0x8D
    if (vccstate == Adafruit_GP9002_EXTERNALVCC) 
      { ssd1306_command(0x10); }
    else 
      { ssd1306_command(0x14); }
    ssd1306_command(Adafruit_GP9002_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
    ssd1306_command(Adafruit_GP9002_SEGREMAP | 0x1);
    ssd1306_command(Adafruit_GP9002_COMSCANDEC);
    ssd1306_command(Adafruit_GP9002_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x12);
    ssd1306_command(Adafruit_GP9002_SETCONTRAST);                   // 0x81
    if (vccstate == Adafruit_GP9002_EXTERNALVCC) 
      { ssd1306_command(0x9F); }
    else 
      { ssd1306_command(0xCF); }
    ssd1306_command(Adafruit_GP9002_SETPRECHARGE);                  // 0xd9
    if (vccstate == Adafruit_GP9002_EXTERNALVCC) 
      { ssd1306_command(0x22); }
    else 
      { ssd1306_command(0xF1); }
    ssd1306_command(Adafruit_GP9002_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(Adafruit_GP9002_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(Adafruit_GP9002_NORMALDISPLAY);                 // 0xA6
  #endif
  
  ssd1306_command(Adafruit_GP9002_DISPLAYON);//--turn on oled panel
}


void Adafruit_GP9002::invert(uint8_t i) {
  if (i) {
    ssd1306_command(Adafruit_GP9002_INVERTDISPLAY);
  } else {
    ssd1306_command(Adafruit_GP9002_NORMALDISPLAY);
  }
}

inline void Adafruit_GP9002::spiwrite(uint8_t c) {
  shiftOut(sid, sclk, MSBFIRST, c);

}
void Adafruit_GP9002::ssd1306_command(uint8_t c) { 
  digitalWrite(cs, HIGH);
  digitalWrite(dc, LOW);
  digitalWrite(cs, LOW);
  spiwrite(c);
  digitalWrite(cs, HIGH);
}

void Adafruit_GP9002::ssd1306_data(uint8_t c) {
  digitalWrite(cs, HIGH);
  digitalWrite(dc, HIGH);
  digitalWrite(cs, LOW);
  spiwrite(c);
  digitalWrite(cs, HIGH);
}

void Adafruit_GP9002::ssd1306_set_brightness(uint8_t val) {
  
}


void Adafruit_GP9002::display(void) {
  ssd1306_command(Adafruit_GP9002_SETLOWCOLUMN | 0x0);  // low col = 0
  ssd1306_command(Adafruit_GP9002_SETHIGHCOLUMN | 0x0);  // hi col = 0
  ssd1306_command(Adafruit_GP9002_SETSTARTLINE | 0x0); // line #0

  for (uint16_t i=0; i<(Adafruit_GP9002_LCDWIDTH*Adafruit_GP9002_LCDHEIGHT/8); i++) {
    ssd1306_data(buffer[i]);
  }
  // i wonder why we have to do this (check datasheet)
  if (Adafruit_GP9002_LCDHEIGHT == 32) {
    for (uint16_t i=0; i<(Adafruit_GP9002_LCDWIDTH*Adafruit_GP9002_LCDHEIGHT/8); i++) {
      ssd1306_data(0);
    }
  }
}

// clear everything
void Adafruit_GP9002::clear(void) {
}

void Adafruit_GP9002::clear_display(void) {
 
}
