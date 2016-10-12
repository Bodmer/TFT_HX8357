// On the SD card use 24 bit color BMP files (be sure they are 24-bit!)
// There are examples images in the sketch folder.

// MS Paint can be used to crop, resize and flip and save images in 24bit BMP format

// Updated 31/3/15 to include new drawRAW() function that draws raw 16 bit images.
// Raw images are created with UTFT library tool (libraries\UTFT\Tools\ImageConverter565.exe)
// Save as .raw file as this can be easily piped to the TFT pushColors() function
// without any tedious byte swapping etc
 
#include <TFT_HX8357.h>        // Hardware-specific library
TFT_HX8357 tft = TFT_HX8357(); // Invoke custom library

// SD Card library, comment out next line if the SdFat library is used
#include <SD.h>

// We can use the SdFat library instead and it will be a bit faster, uncomment next 2 lines
//#include <SdFat.h>           // Use the SdFat library for the Due
//SdFat SD;                    // Permit SD function call compatibility with no sketch changes :-)

// SD chip select
#define SDC_CS 53

// These are used when calling drawBMP() function
#define BU_BMP 1 // Temporarily flip the TFT coords for standard Bottom-Up bit maps
#define TD_BMP 0 // Draw inverted Top-Down bitmaps in standard coord frame

/***************************************************************************************
** Function name:           setup
** Descriptions:            To set things up
***************************************************************************************/

void setup()
{
  Serial.begin(38400); // For debug messages
  delay(4000);

  tft.init(); // Initialise the display (various parameters configured)
  
  Serial.print(F("Initialising SD card..."));
  if (!SD.begin(SDC_CS)) {
    Serial.println(F("failed!"));
    //return;
  }

  Serial.println(F("Here we go..."));

}

/***************************************************************************************
** Function name:           loop
** Descriptions:            Infinite loop
***************************************************************************************/

void loop()
{
  // Standard BMP files are usually stored with a raster scan of pixel values from bottom up
  // File names MUST use the 8.3 file name format (8 chars + . + any 3 extension letters)
  // By convention bitmap files end in .bmp
  // Bit maps MUST be in 24 bit colout (not 16 or 8 bit greyscale)
  
  // Landscape mode
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  // Draw bmp image top left corner at x,y = 0,0
  // Image must fit (one day I will add clipping... but it will slow things down)
  drawBMP("480Mouse.bmp", 0, 0, BU_BMP);

  delay(2000);

  tft.fillScreen(TFT_BLUE);
  
  // Draw bmp image top left corner at x,y = 0,0
  // Image must fit (one day I will add clipping... but it will slow things down)
  drawRAW("480Mouse.raw", 0, 0, 480, 320);

  delay(2000);
}

