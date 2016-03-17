/***************************************************************************************
** Function name:           drawRAW
** Descriptions:            draw a 565 format 16 bit raw image file
***************************************************************************************/

// This function opens a ".raw" image file and displays it at the given coordinates.
// It is faster than plotting BMP images as the file is already in the correct
// format to pipe directly to the display.
// The width and height of the image in pixels must be passed to the function
// as these parameters are not in the file

// Set buffer size, 2 x this size of RAM bytes will be used in the array.
// Reduce if RAM is short with some performance penalty
// Powers of 2 work well

#define RBUFF_SIZE 256

void drawRAW(char *filename, int16_t x, int16_t y, int16_t rawWidth, int16_t rawHeight) {
  File     rawFile;
  uint8_t  sdbuffer[2 * RBUFF_SIZE];   // SD read pixel buffer (16 bits per pixel)
  Serial.println(filename);

  // Check file exists and open it
  if ((rawFile = SD.open(filename)) == NULL) {
    Serial.println(F(" File not found"));
    return;
  }

  // Prepare the TFT screen area to receive the data
  tft.setWindow(x, y, x + rawWidth - 1, y + rawHeight - 1);
 
  // Work out how many whole buffers to send
  uint16_t nr = ((long)rawHeight * rawWidth)/RBUFF_SIZE;
  while(nr--) {
    rawFile.read(sdbuffer, sizeof(sdbuffer));
    tft.pushColors(sdbuffer, RBUFF_SIZE);
  }
  
  // Send any partial buffer
  nr = ((long)rawHeight * rawWidth)%RBUFF_SIZE;
  if (nr) {
    rawFile.read(sdbuffer, nr<<1); // We load  2 x nr bytes
    tft.pushColors(sdbuffer, nr);  // We send nr 16 bit pixels
  }

  // Close the file
  rawFile.close();
}


/***************************************************************************************
** Function name:           drawBMP
** Descriptions:            draw a BMP format bitmap to the screen
***************************************************************************************/

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size makes loading a little faster but the law of
// rapidly diminishing speed improvements applies.
// Suggest 8 minimum and 85 maximum (3 x this value is
// stored in a byte = 255/3 max!)
// A value of 8 is only ~20% slower than 24 or 48!
// Note that 5 x this value of RAM bytes will be needed
// Increasing beyond 48 gives little benefit.
// Use integral division of TFT (or typical often used image)
// width for slightly better speed to avoid short buffer purging

// Maximum of 85
#define BUFF_SIZE 80

void drawBMP(char *filename, int x, int y, boolean flip) {
  if ((x >= tft.width()) || (y >= tft.height())) return;
  File     bmpFile;
  int16_t  bmpWidth, bmpHeight;   // Image W+H in pixels
  //uint8_t  bmpDepth;            // Bit depth (must be 24) but we dont use this
  uint32_t bmpImageoffset;        // Start address of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFF_SIZE];    // SD read pixel buffer (8 bits each R+G+B per pixel)
  uint16_t tftbuffer[BUFF_SIZE];       // TFT pixel out buffer (16-bit per pixel)
  uint8_t  sd_ptr = sizeof(sdbuffer); // sdbuffer pointer (so BUFF_SIZE must be less than 86)
  boolean  goodBmp = false;            // Flag set to true on valid header parse
  int16_t  w, h, row, col;             // to store width, height, row and column
  //uint8_t  r, g, b;   // brg encoding line concatenated for speed so not used
  uint8_t rotation;     // to restore rotation
  uint8_t  tft_ptr = 0;  // buffer pointer

  // Check file exists and open it
  Serial.println(filename);
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.println(F(" File not found")); // Can comment out if not needed
    return;
  }

  // Parse BMP header to get the information we need
  if (read16(bmpFile) == 0x4D42) { // BMP file start signature check
    read32(bmpFile);       // Dummy read to throw away and move on
    read32(bmpFile);       // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    read32(bmpFile);       // Dummy read to throw away and move on
    bmpWidth  = read32(bmpFile);  // Image width
    bmpHeight = read32(bmpFile);  // Image height

    //if (read16(bmpFile) == 1) { 
    // Only proceed if we pass a bitmap file check
    // Number of image planes -- must be '1', depth 24 and 0 (uncompressed format)
    if ((read16(bmpFile) == 1) && (read16(bmpFile) == 24) && (read32(bmpFile) == 0)) {
      //goodBmp = true; // Supported BMP format -- proceed!
      // BMP rows are padded (if needed) to 4-byte boundary
      rowSize = (bmpWidth * 3 + 3) & ~3;
      // Crop area to be loaded
      w = bmpWidth;
      h = bmpHeight;

      // We might need to alter rotation to avoid tedious pointer manipulation
      // Save the current value so we can restore it later
      rotation = tft.getRotation();
      // Use TFT SGRAM coord rotation if flip is set for 25% faster rendering
      if (flip) tft.setRotation((rotation + (flip<<2)) % 8); // Value 0-3 mapped to 4-7

      // We might need to flip and calculate new y plot coordinate
      // relative to top left corner as well...
      switch (rotation) {
        case 0:
          if (flip) y = tft.height() - y - h; break;
        case 1:
          y = tft.height() - y - h; break;
          break;
        case 2:
          if (flip) y = tft.height() - y - h; break;
          break;
        case 3:
          y = tft.height() - y - h; break;
          break;
      }

      // Set TFT address window to image bounds
      // Currently, image will not draw or will be corrputed if it does not fit
      // TODO -> efficient clipping, but I don't need it to be idiot proof ;-)
      tft.setWindow(x, y, x + w - 1, y + h - 1);

      // Finally we are ready to send rows of pixels, writing like this avoids slow 32 bit multiply
      for (uint32_t pos = bmpImageoffset; pos < bmpImageoffset + h * rowSize ; pos += rowSize) {
        // Seek if we need to on boundaries and arrange to dump buffer and start again
        if (bmpFile.position() != pos) {
          bmpFile.seek(pos);
          sd_ptr = sizeof(sdbuffer);
        }

        // Fill the pixel buffer and plot
        for (col = 0; col < w; col++) { // For each column...
          // Time to read more pixel data?
          if (sd_ptr >= sizeof(sdbuffer)) {
            // Push tft buffer to the display
            if (tft_ptr) {
              // Here we are sending a uint16_t array to the function
              tft.pushColors(tftbuffer, tft_ptr);
              tft_ptr = 0; // tft_ptr and sd_ptr are not always in sync...
            }
            // Finally reading bytes from SD Card
            bmpFile.read(sdbuffer, sizeof(sdbuffer));
            sd_ptr = 0; // Set buffer index to start
          }
          // Convert pixel from BMP 8+8+8 format to TFT compatible 16 bit word
          // Blue 5 bits, green 6 bits and red 5 bits (16 bits total)
          // Is is a long line but it is faster than calling a library fn for this
          tftbuffer[tft_ptr] = (sdbuffer[sd_ptr++] >> 3) ;
          tftbuffer[tft_ptr] |= ((sdbuffer[sd_ptr++] & 0xFC) << 3);
          tftbuffer[tft_ptr] |= ((sdbuffer[sd_ptr++] & 0xF8) << 8);
          tft_ptr++;
        } // Next row
      }   // All rows done

      // Write any partially full buffer to TFT
      if (tft_ptr) tft.pushColors(tftbuffer, tft_ptr);

    } // End of bitmap access
  }   // End of bitmap file check
  //}     // We can close the file now

  bmpFile.close();
  //if(!goodBmp) Serial.println(F("BMP format not recognized."));
  tft.setRotation(rotation); // Put back original rotation
}

/***************************************************************************************
** Function name:           Support functions for drawBMP()
** Descriptions:            Read 16 and 32-bit types from the SD card file
***************************************************************************************/

// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File& f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File& f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

