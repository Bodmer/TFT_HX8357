# TFT_HX8357

An Arduino IDE compatible graphics and fonts library for Mega with a drivers for the HX8357B, HX8357C and ILI9481 based TFT displays.

The library contains proportional fonts, different sizes can be enabled/disabled at compile time to optimise the use of FLASH memory.  The library has been tested with the Mega.

In addition to the default fonts (1, 2, 4, 6, 7 and 8) the library includes and supports the encoded Free Fonts from the new Adafruit_GFX library.  Unlike the Adafruit_GFX library these faults can be automatically rendered with background and padding to over-write and erase old text, see the examples.

The library is designed to be slim and fast. Typically a clear screen for a 320 x 480 TFT will complete in only 12ms.

The library is based on the Adafruit GFX library and the aim is to retain compatibility. Significant additions have been made to the library to boost the speed for the Due processor and to add new features. The new graphics functions include different size proportional fonts and formatting features. There are a significant number of example sketches to demonstrate the different features.

Configuration of the library font selections and other features is made by editting the User_Setup.h file in the library folder.  Fonts and features can easily be disabled by commenting out lines.

The 3.2" HX8357B display I use can be found here:

http://www.banggood.com/3_2-Inch-320-X-480-TFT-LCD-Display-Module-Support-Arduino-Mega2560-p-963574.html

or here for the HX8357C based 3.0" display:

http://www.banggood.com/3_0-Inch-320-X-480-TFT-LCD-Display-Module-Support-Arduino-Mega2560-p-963573.html

Soon a dedicated Instructable will be available to document the library features, until then do explore the example sketches.

Have fun!
