#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "Adafruit_GP9002.h"
#include "glcdfont.c"

static uint8_t is_reversed = 0;

// a 5x7 font table
extern uint8_t PROGMEM font[];

Adafruit_GP9002::Adafruit_GP9002(int8_t SCLK, int8_t MISO, int8_t MOSI, 
				 int8_t CS, int8_t DC) {
  _sclk = SCLK;
  _miso = MISO;
  _mosi = MOSI;
  _cs = CS;
  _dc = DC;

  WIDTH = 128;
  HEIGHT = 64;
}


void Adafruit_GP9002::drawString(uint8_t x, uint8_t line, char *c) {
  while (c[0] != 0) {
    drawChar(x, line, c[0]);
    c++;
    x += 6; // 6 pixels wide
    if (x + 6 >= this->WIDTH) {
      x = 0;    // ran out of this line
      line++;
    }
    if (line >= (this->HEIGHT/8))
      return;        // ran out of space :(
  }

}

void  Adafruit_GP9002::drawChar(uint8_t x, uint8_t line, uint8_t c) {
  if ((line >= this->HEIGHT/8) || (x >= (this->WIDTH - 6)))
      return;
  for (uint8_t i =0; i<5; i++ ) {
    //buffer[x + (line*128) ] = pgm_read_byte(font+(c*5)+i);
    x++;
  }
}


// updated high speed drawing!
void Adafruit_GP9002::drawFastVLine(uint8_t x, uint8_t orig_y, uint8_t h, uint8_t color) {
  
  while (orig_y % 8) {
    drawPixel(x, orig_y, color);
    orig_y++;
    h--;
  }    
  
  // calculate addr
  uint16_t addr = 0;
  addr = x*8;
  addr += orig_y/8;
  
  command(GP9002_ADDRINCR);
  command(GP9002_ADDRL);
  data(addr & 0xFF);
  command(GP9002_ADDRH);
  data(addr >> 8);
  command(GP9002_DATAWRITE);
  while (h >= 8) {
    // draw 8 pixels at once!
    if (color) 
      data(0xFF);
    else 
      data(0x00);
    h -= 8;
    orig_y += 8;
  }
  command(GP9002_ADDRHELD);

  while (h) {
    drawPixel(x, orig_y, color);
    orig_y++;
    h--;
  }
}






// the most basic function, set a single pixel
void Adafruit_GP9002::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= this->WIDTH) || (y >= this->HEIGHT))
    return;

  uint8_t p;
  
  // calculate addr
  uint16_t addr = 0;
  addr = x*8;
  addr += y/8;

  // hold the address so we can read and then write
  command(GP9002_ADDRHELD);
  
  command(GP9002_ADDRL);
  data(addr & 0xFF);
  command(GP9002_ADDRH);
  data(addr >> 8);
  
  command(GP9002_DATAREAD);
  data(0x0);
  p = data(0x0);
  
  p |= (1 << (7-(y % 8)));
  command(GP9002_DATAWRITE);
  data(p);
}

  
void Adafruit_GP9002::begin(void) {
  // set pin directions
  pinMode(_mosi, OUTPUT);
  pinMode(_miso, INPUT);
  pinMode(_sclk, OUTPUT);
  pinMode(_dc, OUTPUT);
  pinMode(_cs, OUTPUT);

  command(GP9002_DISPLAY);
  data(GP9002_DISPLAY_MONOCHROME);
  command(GP9002_OR);
  command(GP9002_CLEARSCREEN);
  command(GP9002_DISPLAY1ON);
}


void Adafruit_GP9002::invert(uint8_t i) {
 // command(GP9002_XOR);
}

uint8_t Adafruit_GP9002::spiTransfer(uint8_t d, uint8_t datacommand) {
 uint8_t reply = 0;

 volatile uint8_t *sclkportreg, *mosiportreg, *misoportreg;
  uint8_t sclkpin, mosipin, misopin;

 digitalWrite(_dc, datacommand);
 digitalWrite(_cs, LOW);

  sclkportreg = portOutputRegister(digitalPinToPort(_sclk));
  sclkpin = digitalPinToBitMask(_sclk);
  mosiportreg = portOutputRegister(digitalPinToPort(_mosi));
  mosipin = digitalPinToBitMask(_mosi);
  misoportreg = portInputRegister(digitalPinToPort(_miso));
  misopin = digitalPinToBitMask(_miso);

 for (uint8_t i=0; i<8; i++) {
   //digitalWrite(_sclk, LOW);
   *sclkportreg &= ~sclkpin;
   if (d & _BV(i)) {
     //digitalWrite(_mosi, HIGH);
     *mosiportreg |= mosipin;
   } else {
     //digitalWrite(_mosi, LOW);
     *mosiportreg &= ~mosipin;
   }
   //delayMicroseconds(10);
   //digitalWrite(_sclk, HIGH);
   *sclkportreg |= sclkpin;

   if (digitalRead(_miso)) 
   //if ((*misoportreg) & misopin)
     reply |= _BV(i);
   delayMicroseconds(5);
 }
 digitalWrite(_cs, HIGH);
  
  return reply;
}


void Adafruit_GP9002::command(uint8_t d) { 
 volatile uint8_t *sclkportreg, *mosiportreg;
  uint8_t sclkpin, mosipin;

 digitalWrite(_dc, HIGH);
 digitalWrite(_cs, LOW);

 /*
  sclkportreg = portOutputRegister(digitalPinToPort(_sclk));
  sclkpin = digitalPinToBitMask(_sclk);
  
  mosiportreg = portOutputRegister(digitalPinToPort(_mosi));
  mosipin = digitalPinToBitMask(_mosi);
  */
 for (uint8_t i=0; i<8; i++) {

   digitalWrite(_sclk, LOW);
   //*sclkportreg &= ~sclkpin;
   if (d & _BV(i)) {
     digitalWrite(_mosi, HIGH);
     //*mosiportreg |= mosipin;
   } else {
     digitalWrite(_mosi, LOW);
     //*mosiportreg &= ~mosipin;
   }   
   digitalWrite(_sclk, HIGH);
   //*sclkportreg |= sclkpin;
 }
 digitalWrite(_cs, HIGH);
}

uint8_t Adafruit_GP9002::data(uint8_t d) {
 return spiTransfer(d, LOW);
}

void Adafruit_GP9002::setBrightness(uint8_t val) {
  
}

// clear everything
void Adafruit_GP9002::clearDisplay(void) {
  command(GP9002_CLEARSCREEN);
}

void Adafruit_GP9002::displayOff(void) {
  command(GP9002_DISPLAYSOFF); 
}
void Adafruit_GP9002::displayOn(void) {
   command(GP9002_DISPLAY1ON);
}


