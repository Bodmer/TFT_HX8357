# TFT_HX8357

An Arduino IDE compatible graphics and fonts library for Mega with a drivers for the HX8357B, HX8357C, ILI9481 and ILI9486 based TFT displays with a 16 bit parallel interface. This library will not run on an UNO and it does not support 8 bit UNO shields.

The library contains proportional fonts, different sizes can be enabled/disabled at compile time to optimise the use of FLASH memory.  The library has been tested with the Mega.

In addition to the default fonts (1, 2, 4, 6, 7 and 8) the library includes and supports the encoded Free Fonts from the new Adafruit_GFX library.  Unlike the Adafruit_GFX library these fonts can be automatically rendered with background and padding to over-write and erase old text, see the examples.

The library is designed to be slim and fast. Typically a clear screen for a 320 x 480 TFT will complete in only 12ms.

The library is based on the Adafruit GFX library and the aim is to retain compatibility. Significant additions have been made to the library to boost the speed for the Mega processor and to add new features. The new graphics functions include different size proportional fonts and formatting features. There are a significant number of example sketches to demonstrate the different features.

Configuration of the library font selections and other features is made by editting the User_Setup.h file in the library folder.  Fonts and features can easily be disabled by commenting out lines.

The 3.2" HX8357B display I use can be found here:

http://www.banggood.com/3_2-Inch-320-X-480-TFT-LCD-Display-Module-Support-Arduino-Mega2560-p-963574.html

This HX8357B based display does appear to have a bug in the silicon of the driver chip as it sometimes generates spurious pixels on the display. The only way around this is to redraw the whole screen occasionally to wipe out the duff ones, this is most noticeable for intensive updates in small screen areas (e.g. in ring meter sketch). So my suggestion is to go for the next revision driver, the HX8357C with 3" TFT which does not exhibit the same problem:

http://www.banggood.com/3_0-Inch-320-X-480-TFT-LCD-Display-Module-Support-Arduino-Mega2560-p-963573.html

Have fun!
