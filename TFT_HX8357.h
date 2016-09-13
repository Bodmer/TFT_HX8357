/***************************************************
  Arduino TFT graphics library targetted at the
  Mega board, HX8357B/C and ILI9481 display drivers.

  This library has been derived from the Adafruit_GFX
  library and the associated driver library. See text
  at the end of this file.

  This is a standalone library that contains the
  hardware driver, the graphics functions and the
  proportional fonts.

  The larger fonts are Run Length Encoded to reduce
  their FLASH footprint and speed rendering.

 ****************************************************/

// Include header file that defines the fonts loaded and the pins to be used
#include <User_Setup.h>

// Stop fonts etc being loaded multiple times
#ifndef _TFT_HX8357H_
#define _TFT_HX8357H_

// Only load the fonts defined in User_Setup.h (to save space)
// Sets flag so RLE rendering code is optionally compiled

#ifdef LOAD_GLCD
  #include <Fonts/glcdfont.c>
#endif

#ifdef LOAD_FONT2
  #include <Fonts/Font16.h>
#endif

#ifdef LOAD_FONT4
  #include <Fonts/Font32rle.h>
  #define LOAD_RLE
#endif

#ifdef LOAD_FONT6
  #include <Fonts/Font64rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#ifdef LOAD_FONT7
  #include <Fonts/Font7srle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#ifdef LOAD_FONT8
  #include <Fonts/Font72rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Print.h>

#include <Fonts/GFXFF/gfxfont.h>

// Swap any type
template <typename T> static inline void
swap(T& a, T& b) { T t = a; a = b; b = t; }


//These define the ports and port bits used for the write, chip select (CS) and data/command (RS) lines
#define WR_L PORTG&=~_BV(2);

// We need a slower write strobe for the ILI9488
#ifdef ILI9486
  #define ILI9481
  #define WR_H PORTG&=~_BV(2);PORTG|=_BV(2);
  #define WR_STB PORTG&=~_BV(2);PORTG&=~_BV(2);PORTG|=_BV(2);
#else
  #define WR_H PORTG|=_BV(2);
  #define WR_STB PORTG&=~_BV(2);PORTG|=_BV(2);
#endif

// Chip select must be toggled during setup
#define SETUP_CS_H PORTG|=_BV(1)
#define SETUP_CS_L PORTG&= ~_BV(1)

// Chip select can optionally be kept low after setup
#ifndef KEEP_CS_LOW
  #define CS_H PORTG|=_BV(1)
  #define CS_L PORTG&= ~_BV(1)
#else
  #define CS_H // We do not define this so CS will not be set high
  #define CS_L PORTG&= ~_BV(1)
#endif

// If pin 4 is hard wired to pin 38 we benefit from all controls on PORTG
#ifdef FAST_RS
  #define RS_L PORTG&= ~_BV(5)
  #define RS_H PORTG|=_BV(5)
#else
  #define RS_L PORTD&=~_BV(7)
  #define RS_H PORTD|=_BV(7)
#endif

// These are the port settings if the Mega Pin 4 is wired direct to pin 38,
// this hardware hack puts all important control pins on one port (PORTG)
// Conveniently other PORTG pins are not used on the Mega so we can set
// other bits with impunity!
//                          CS-----.
//                          WR----.|
//                          RS-.  ||
//                             |  ||
#define CSL_RSL_WRL PORTG = B11011001
#define CSL_RSL_WRH PORTG = B11011101
#define CSL_RSH_WRL PORTG = B11111001
#define CSL_RSH_WRH PORTG = B11111101


// These register enumerations are not all used, but kept for possible future use
#define HX8357D 0xD
#define HX8357B 0xB

#define HX8357_NOP     0x00
#define HX8357_SWRESET 0x01
#define HX8357_RDDID   0x04
#define HX8357_RDDST   0x09

#define HX8357_RDPOWMODE  0x0A
#define HX8357_RDMADCTL  0x0B
#define HX8357_RDCOLMOD  0x0C
#define HX8357_RDDIM  0x0D
#define HX8357_RDDSDR  0x0F

#define HX8357_SLPIN   0x10
#define HX8357_SLPOUT  0x11
#define HX8357B_PTLON   0x12
#define HX8357B_NORON   0x13

#define HX8357_INVOFF  0x20
#define HX8357_INVON   0x21
#define HX8357_DISPOFF 0x28
#define HX8357_DISPON  0x29

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E

#define HX8357B_PTLAR   0x30
#define HX8357_TEON  0x35
#define HX8357_TEARLINE  0x44
#define HX8357_MADCTL  0x36
#define HX8357_COLMOD  0x3A

#define HX8357_SETOSC 0xB0
#define HX8357_SETPWR1 0xB1
#define HX8357B_SETDISPLAY 0xB2
#define HX8357_SETRGB 0xB3
#define HX8357D_SETCOM  0xB6

#define HX8357B_SETDISPMODE  0xB4
#define HX8357D_SETCYC  0xB4
#define HX8357B_SETOTP 0xB7
#define HX8357D_SETC 0xB9

#define HX8357B_SET_PANEL_DRIVING 0xC0
#define HX8357D_SETSTBA 0xC0
#define HX8357B_SETDGC  0xC1
#define HX8357B_SETID  0xC3
#define HX8357B_SETDDB  0xC4
#define HX8357B_SETDISPLAYFRAME 0xC5
#define HX8357B_GAMMASET 0xC8
#define HX8357B_SETCABC  0xC9
#define HX8357_SETPANEL  0xCC


#define HX8357B_SETPOWER 0xD0
#define HX8357B_SETVCOM 0xD1
#define HX8357B_SETPWRNORMAL 0xD2

#define HX8357B_RDID1   0xDA
#define HX8357B_RDID2   0xDB
#define HX8357B_RDID3   0xDC
#define HX8357B_RDID4   0xDD

#define HX8357D_SETGAMMA 0xE0

#define HX8357B_SETGAMMA 0xC8
#define HX8357B_SETPANELRELATED  0xE9


// This is a structure to conveniently hold infomation on the default fonts
// Stores pointer to font character image address table, width table and height

typedef struct {
	const uint8_t *chartbl;
	const uint8_t *widthtbl;
	uint8_t height;
	uint8_t baseline;
	} fontinfo;

// Now fill the structure
const PROGMEM fontinfo fontdata [] = {
   { 0, 0, 0, 0 },

   // GLCD font (Font 1) does not have all parameters
   { 0, 0, 8, 7 },

  #ifdef LOAD_FONT2
   { (const uint8_t *)chrtbl_f16, widtbl_f16, chr_hgt_f16, baseline_f16},
  #else
   { 0, 0, 0, 0 },
  #endif

   // Font 3 current unused
   { 0, 0, 0, 0 },

  #ifdef LOAD_FONT4
   { (const uint8_t *)chrtbl_f32, widtbl_f32, chr_hgt_f32, baseline_f32},
  #else
   { 0, 0, 0, 0 },
  #endif

   // Font 5 current unused
   { 0, 0, 0, 0 },

  #ifdef LOAD_FONT6
   { (const uint8_t *)chrtbl_f64, widtbl_f64, chr_hgt_f64, baseline_f64},
  #else
   { 0, 0, 0, 0 },
  #endif

  #ifdef LOAD_FONT7
   { (const uint8_t *)chrtbl_f7s, widtbl_f7s, chr_hgt_f7s, baseline_f7s},
  #else
   { 0, 0, 0, 0 },
  #endif

  #ifdef LOAD_FONT8
   { (const uint8_t *)chrtbl_f72, widtbl_f72, chr_hgt_f72, baseline_f72}
  #else
   { 0, 0, 0, 0 }
  #endif
};


// Class member functions and variables
class TFT_HX8357

//#ifdef PRINT_CLASS 
 : public Print
//#endif

{

 public:

  TFT_HX8357(int16_t _W = HX8357_TFTWIDTH, int16_t _H = HX8357_TFTHEIGHT);

  void     init(void), begin(void), // Same - begin included for backwards compatibility

           drawPixel(uint16_t x, uint16_t y, uint16_t color),

           drawChar(int16_t x, int16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t font),
           setWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1),

           pushColor(uint16_t color),
           pushColor(uint16_t color, uint16_t len),

           pushColors(uint16_t *data, uint8_t len),
           pushColors(uint8_t  *data, uint16_t len),

           fillScreen(uint16_t color),

           drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),

           drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
           drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),
           fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),

           setRotation(uint8_t r),
           invertDisplay(boolean i),

           drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
           drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color),
           fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
           fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color),

           drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color),
           fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color),

           drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
           fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),

           drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color),

           setCursor(int16_t x, int16_t y),
           setCursor(int16_t x, int16_t y, uint8_t font),
           setTextColor(uint16_t color),
           setTextColor(uint16_t fgcolor, uint16_t bgcolor),
           setTextSize(uint8_t size),
           setTextFont(uint8_t font),
           setTextWrap(boolean wrap),
           setTextDatum(uint8_t datum),
           setTextPadding(uint16_t x_width),

#ifdef LOAD_GFXFF
           setFreeFont(const GFXfont *f = NULL),
           setTextFont(const GFXfont *f = NULL),
#else
           setFreeFont(uint8_t font),
#endif

           writecommand(uint8_t c),
           writedata(uint8_t d),
           commandList(const uint8_t *addr);

  uint8_t  getRotation(void);

  uint16_t fontsLoaded(void),
           color565(uint8_t r, uint8_t g, uint8_t b);

  int16_t  drawChar(uint16_t uniCode, int16_t x, int16_t y, int16_t font),
           drawNumber(int32_t long_num,int16_t poX, int16_t poY, int16_t font),
           drawFloat(float floatNumber,int16_t decimal,int16_t poX, int16_t poY, int16_t font),

           drawString(char *string, int16_t poX, int16_t poY, int16_t font),
           drawCentreString(char *string, int16_t dX, int16_t poY, int16_t font),
           drawRightString(char *string, int16_t dX, int16_t poY, int16_t font),

           height(void),
           width(void),
           textWidth(char *string, int16_t font),
           fontHeight(int16_t font);

//#ifdef PRINT_CLASS 
  virtual  size_t write(uint8_t c);
//#endif

 private:
    // Sketches should use setWindow(...) instead of this one
    void  setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

  uint8_t  tabcolor;

 protected:

  int16_t  _width, _height,           // Display w/h as modified by current rotation
           cursor_x, cursor_y, padX;  // Text cursor position and width padding

  uint16_t textcolor, textbgcolor, fontsloaded;

  uint8_t  glyph_ab,  // glyph height above baseline
           glyph_bb,  // glyph height below baseline
           textfont,  // Current selected font
           textsize,  // Current font size multiplier
           textdatum, // Text reference datum
           rotation;  // Display rotation (0-3)

  int8_t   _cs, _rs, _rst, _wr, _fcs; // Control lines

  boolean  textwrap; // If set, 'wrap' text at right edge of display

#ifdef LOAD_GFXFF
  GFXfont
    *gfxFont;
#endif

};

#endif

/***************************************************

  ORIGINAL LIBRARY HEADER

  This is our library for the Adafruit  HX8357 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
