//                            USER DEFINED SETTINGS V1
//                    Set fonts to be loaded and speed upoptions

// ##################################################################################
//
// Define the driver as HX8357C if it is NOT a HX8357B
//
// ##################################################################################

//#define HX8357C

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

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.


// ##################################################################################
//
// Other speed up options
//
// ##################################################################################

// Run faster by stopping most screen boundarylimit checks
// Comment out the following #define to stop boundary checking and clipping
// for fillRectangle()and fastH/V lines. This speeds up other funtions such as text
// rendering where size>1. Sketch then must not draw graphics/text outside screen
// boundary. Code saving for no bounds check (i.e. commented out) is small.

#define CLIP_CHECK


// Uncomment next #define to run faster by keeping CS low
// This only speeds things up a little (0 - 2%)
// We can do this as the CS line is not shared with other functions

//#define KEEP_CS_LOW


// Uncomment next #define for faster RS line toggling
// ** MUST** comment this out unless Pin 4 is connected with a wire link to pin 38
// so the all controls CS, RS and WR are on PORTG. Then we can define all control
// line states with a single port write.
// Performance gain is small for functions that write large filled pixel blocks
// performance gain is ~16% for drawLines() and drawPixel() functions.

//#define FAST_RS
