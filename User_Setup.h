//                            USER DEFINED SETTINGS V6
//  This library supports the Mega and HX8357B/C, ILI9481 and ILI9486 display drivers
//
//                 Set fonts to be loaded and speed up options below


// ##################################################################################
//
// Define ONE driver, either HX8357B, HX8357C, ILI9481 or ILI9486
//
// ##################################################################################

#define HX8357B
//#define HX8357C
//#define ILI9481
//#define ILI9486

// ##################################################################################
//
// Define the display width and height in pixels in portrait orientation
//
// ##################################################################################

#define HX8357_TFTWIDTH  320
#define HX8357_TFTHEIGHT 480

// ##################################################################################
//
// Define the fonts that are to be used here
//
// ##################################################################################

// Comment out the #defines below with // to stop that font being loaded
//
// If all fonts are loaded the extra FLASH space required is about 17000 bytes...
// To save FLASH space only enable the fonts you need!
// In practice a Mega has plenty of FLASH to hold all fonts, so the main gain is
// faster sketch upload due to smaller size
// If free fonts are not needed then there is some performance gain if LOAD_GFXFF
// is commented out

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48

// ##################################################################################
//
// Define the character to be used to detemine the text bounding box for datum changes
//
// ##################################################################################

#define FF_HEIGHT '/'  // '/' character used to set free font height above the baseline
#define FF_BOTTOM 'y'  // 'y' character used to set free font height below baseline


// ##################################################################################
//
// Speed up options (hardly worth changing when using a Due!)
//
// ##################################################################################

// Run faster by stopping most screen boundary limit checks
// Comment out the following #define to stop boundary checking and clipping
// for fillRectangle()and fastH/V lines. This speeds up other funtions such as text
// rendering where size>1. Sketch then must not draw graphics/text outside screen
// boundary. Code saving for no bounds check (i.e. commented out) is small.

#define CLIP_CHECK


// Render fonts slightly faster, only enable if you DO NOT use setTextSize(n)
// with values of 'n' greater than 1, when enabled the code that alows the
// text size to be multiplied is not included.

//#define FIXED_SIZE 


// Uncomment next #define to run faster by keeping CS low
// This only speeds things up a little (0 - 2%)
// We can do this safely as the CS line is not shared with other functions

//#define KEEP_CS_LOW


// ##################################################################################
//
// These defines are used to set different datums for drawing text and numbers
//
// ##################################################################################

// These enumerate the text plotting alignment (reference datum point)
// use in the setTextDatum() function

#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

// ##################################################################################
//
// These are the default colour definitions, others can be added here
//
// ##################################################################################

// New color definitions, used for all my TFT libraries
#define TFT_BLACK       0x0000
#define TFT_NAVY        0x000F
#define TFT_DARKGREEN   0x03E0
#define TFT_DARKCYAN    0x03EF
#define TFT_MAROON      0x7800
#define TFT_PURPLE      0x780F
#define TFT_OLIVE       0x7BE0
#define TFT_LIGHTGREY   0xC618
#define TFT_DARKGREY    0x7BEF
#define TFT_BLUE        0x001F
#define TFT_GREEN       0x07E0
#define TFT_CYAN        0x07FF
#define TFT_RED         0xF800
#define TFT_MAGENTA     0xF81F
#define TFT_YELLOW      0xFFE0
#define TFT_WHITE       0xFFFF
#define TFT_ORANGE      0xFD20
#define TFT_GREENYELLOW 0xAFE5
#define TFT_PINK        0xF81F

// Color definitions for backwards compatibility
#define HX8357_BLACK       0x0000
#define HX8357_NAVY        0x000F
#define HX8357_DARKGREEN   0x03E0
#define HX8357_DARKCYAN    0x03EF
#define HX8357_MAROON      0x7800
#define HX8357_PURPLE      0x780F
#define HX8357_OLIVE       0x7BE0
#define HX8357_LIGHTGREY   0xC618
#define HX8357_DARKGREY    0x7BEF
#define HX8357_BLUE        0x001F
#define HX8357_GREEN       0x07E0
#define HX8357_CYAN        0x07FF
#define HX8357_RED         0xF800
#define HX8357_MAGENTA     0xF81F
#define HX8357_YELLOW      0xFFE0
#define HX8357_WHITE       0xFFFF
#define HX8357_ORANGE      0xFD20
#define HX8357_GREENYELLOW 0xAFE5
#define HX8357_PINK        0xF81F


// ##################################################################################
//
// End of User_Setup.h
//
// ##################################################################################
