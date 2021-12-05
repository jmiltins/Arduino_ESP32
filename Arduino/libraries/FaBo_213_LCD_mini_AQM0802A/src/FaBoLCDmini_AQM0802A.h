/**
 @file FaBoLCDmini_AQM0802A.h
 @brief This is a library for the FaBo LCD mini I2C Brick.

   http://fabo.io/213.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#ifndef FABOLCDMINI_AQM0802A_H
#define FABOLCDMINI_AQM0802A_H

#include <Arduino.h>
#include <Wire.h>
#include "Print.h"

#define AQM0802A_SLAVE_ADDRESS 0x3E ///< AQM0802A I2C Slave Address

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/**
 @class FaBoLCDmini_AQM0802A
 @brief FaBo LCD mini I2C Controll class
*/
class FaBoLCDmini_AQM0802A : public Print {
  public:
    FaBoLCDmini_AQM0802A(uint8_t addr = AQM0802A_SLAVE_ADDRESS);
    void init(void);
    void begin(void);
    void setRowOffsets(int row0, int row1, int row2, int row3);
/********** high level commands, for the user! */
    void clear(void);
    void home(void);
    void setCursor(uint8_t col, uint8_t row);
    void noDisplay(void);
    void display(void);
    void noCursor(void);
    void cursor(void);
    void noBlink(void);
    void blink(void);
    void scrollDisplayLeft(void);
    void scrollDisplayRight(void);
    void leftToRight(void);
    void rightToLeft(void);
    void autoscroll(void);
    void noAutoscroll(void);
    void createChar(uint8_t location, uint8_t charmap[]);
    void setContrast(uint8_t contrast);
/*********** mid level commands, for sending data/cmds */
    void command(uint8_t value);
    virtual size_t write(uint8_t value);
  private:
    uint8_t _i2caddr;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _numlines;
    uint8_t _numcols;
    uint8_t _row_offsets[4];
/************ low level data pushing commands **********/
    void send(uint8_t value, uint8_t mode);
};

#endif // FABOLCDMINI_AQM0802A_H
