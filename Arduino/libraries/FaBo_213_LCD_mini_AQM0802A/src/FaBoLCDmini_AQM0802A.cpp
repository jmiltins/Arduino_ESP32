/**
 @file FaBoLCDmini_AQM0802A.cpp
 @brief This is a library for the FaBo LCD mini I2C Brick.

   http://fabo.io/213.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include "FaBoLCDmini_AQM0802A.h"

/**
 @brief Constructor
*/
FaBoLCDmini_AQM0802A::FaBoLCDmini_AQM0802A(uint8_t addr)
{
  _i2caddr = addr;
  Wire.begin();
}

/**
 @brief init
*/
void FaBoLCDmini_AQM0802A::init()
{
  begin();
}

/**
 @brief begin
*/
void FaBoLCDmini_AQM0802A::begin()
{
  _numcols = 8; // AQM0802A: 8
  _numlines = 2; // AQM0802A: 2
  uint8_t value = 0x00;

  setRowOffsets(0x00, 0x40, 0x00 + _numcols, 0x40 + _numcols);

  // Initializing

  // ￼Wait time >40mS After VDD stable
  delay(40);

  //Function Set: Normal mode:
  //  Dl: interface data 8/4bits
  //  N: number of line 2/1
  //  001D Nxxx
  //  0011 1000 : 0x38
  value = 0x38; // 0x20|LCD_8BITMODE|LCD_2LINE
  // write value
  command(value);
  // Wait time >26.3uS

  //Function Set: Extension mode:
  //  Dl: interface data 8/4bits
  //  N: number of line 2/1
  //  dH: double height font
  //  Is: instruction table select
  //  001D NH0I
  //  0011 1001 : 0x39
  value = 0x39; // 0x20|LCD_8BITMODE|LCD_2LINE|0x1
  // write value
  command(value);
  // Wait time >26.3uS

  //Internal OSC frequency:
  //  Bs: 1=1/4 bias 0=1/5 bias
  //  F2-0: adjust internal OSC frequency for FR frequency.
  //  0001 BFFF
  //  0001 0100 : 0x14
  value = 0x14;
  // write value
  command(value);
  // Wait time >26.3uS

  //Contrast set:
  //  C3-0: Contrast set for internal follower mode.
  //  0111 CCCC
  //  0111 0000 : 0x70
  value = 0x70;
  // write value
  command(value);
  // Wait time >26.3uS

  //Power/ICON control/Contrast set:
  //  Ion: ICON display on/off
  //  Bon: set booster circuit on/off
  //  C5-4: Contrast set for internal follower mode.
  //  0101 IBCC
  //  0101 0110 : 0x56
  value = 0x56;
  // write value
  command(value);
  // Wait time >26.3uS

  // Contrast initialize: xx10 0000(32d)

  //Follower control:
  //  Fon: set follower circuit on/off
  //  Rab2-0: select follower amplified ratio.
  //  0110 FRRR
  //  0101 1100 : 0x6C
  value = 0x6C;
  // write value
  command(value);
  // Wait time >26.3uS

  //Function Set: Normal mode:
  //  Dl: interface data 8/4bits
  //  N: number of line 2/1
  //  001D Nxxx
  //  0011 1000 : 0x38
  value = 0x38;
  // write value
  command(value);
  // Wait time >26.3uS

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

  display();
  clear();
  home();
}

/**
 @brief setContrast(1-63)
*/
void FaBoLCDmini_AQM0802A::setContrast(uint8_t contrast)
{
  if ( contrast>63 ) contrast = 63;
  if ( contrast<1 ) contrast = 1;
  command(0x39); // set Extension mode
  command(0x70|(contrast&0x0F)); // Contrast set
  command(0x54|((contrast&0x30) >>4) ); // Power/ICON control/Contrast set
  command(0x38); // set Normal mode
}

/**
 @brief setRowOffsets
*/
void FaBoLCDmini_AQM0802A::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/**
 @brief clear
*/
void FaBoLCDmini_AQM0802A::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

/**
 @brief home
*/
void FaBoLCDmini_AQM0802A::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

/**
 @brief setCursor
*/
void FaBoLCDmini_AQM0802A::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }

  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

/**
 @brief noDisplay
*/
void FaBoLCDmini_AQM0802A::noDisplay()
{
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief display
*/
void FaBoLCDmini_AQM0802A::display()
{
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief noCursor
*/
void FaBoLCDmini_AQM0802A::noCursor()
{
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief cursor
*/
void FaBoLCDmini_AQM0802A::cursor()
{
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief noBlink
*/
void FaBoLCDmini_AQM0802A::noBlink()
{
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief blink
*/
void FaBoLCDmini_AQM0802A::blink()
{
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 @brief scrollDisplayLeft
*/
void FaBoLCDmini_AQM0802A::scrollDisplayLeft(void)
{
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/**
 @brief scrollDisplayRight
*/
void FaBoLCDmini_AQM0802A::scrollDisplayRight(void)
{
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/**
 @brief leftToRight
*/
void FaBoLCDmini_AQM0802A::leftToRight(void)
{
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief rightToLeft
*/
void FaBoLCDmini_AQM0802A::rightToLeft(void)
{
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief autoscroll
*/
void FaBoLCDmini_AQM0802A::autoscroll(void)
{
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief noAutoscroll
*/
void FaBoLCDmini_AQM0802A::noAutoscroll(void)
{
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

/**
 @brief createChar
*/
void FaBoLCDmini_AQM0802A::createChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/**
 @brief command
*/
void FaBoLCDmini_AQM0802A::command(uint8_t value)
{
  send(value, (uint8_t)0x00);
}

/**
 @brief write
*/
size_t FaBoLCDmini_AQM0802A::write(uint8_t value)
{
  send(value, (uint8_t)0x40);
  return 1; // assume sucess
}

/**
 @brief send
*/
void FaBoLCDmini_AQM0802A::send(uint8_t value, uint8_t mode)
{
  Wire.beginTransmission(_i2caddr);
  Wire.write(mode); // control byte
  Wire.write(value);
  Wire.endTransmission();
  delayMicroseconds(27); // wait execution time
}
