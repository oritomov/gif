/**
 * @(#)gif89a.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef GIF89aH
#define GIF89aH

#include "GifCommon.h"

#pragma pack (push)
#pragma pack (1)
//---------------------------------------------------------------------------
/*
- GIF89a HEADER 
- LOGICAL SCREEN DESCRIPTOR BLOCK 
  - may include an optional GLOBAL COLOR TABLE (99.5% of the time this will be present) 
- optional NETSCAPE APPLICATION EXTENSION BLOCK(:-> surprise) 
- a stream of graphics (each graphic being composed of the following) 
  - an optional GRAPHIC CONTROL BLOCK (one preceding each IMAGE) 
  - a single IMAGE DESCRIPTOR or PLAIN TEXT BLOCK 
    - which can include an optional LOCAL COLOR TABLE for an image 
    - and the actual IMAGE or TEXT data table 
- GIF TRAILER ends the series of images 
*/

#define GRAPHIC_CONTROL_EXTENSION_IDENTIFIER 0x21
#define GRAPHIC_CONTROL_APPLICATION_IDENTIFIER 0xF9
#define GRAPHIC_CONTROL_APPLICATION_SIZE 0x04

/**
  A GIF Extension Block may immediately preceed any Image  Descriptor
  or occur before the GIF Terminator.
  
 
byte   1       : 33 (hex 0x21) GIF Extension code

then (by GIF89a Specification)

byte   2       : 249 (hex 0xF9) Application Extension Label

means 

byte   3       : 4 (hex (0x04) Length of Application Block 
byte   4       :   Reserved                      3 Bits
                   Disposal Method               3 Bits
                   User Input Flag               1 Bit
                   Transparent Color Flag        1 Bit
                 1 there is a transparent background color 
                 (bit field; the lowest bit signifies transparency)
bytes  5 & 6   : 0 (hex (0x0000) delay for animation in hundredths of a second: not used
byte   7       : 255 (hex 0xFF) transparent color index

byte   8       : 0 (hex 0x00) a Data Sub-block Terminator. 

or (dont know why)

byte   2       : 255 (hex 0xFF) Application Extension Label

means 

byte   3       : 11 (hex (0x0B) Length of Application Block 
                 (eleven bytes of data to follow)
bytes  4 to 11 : "NETSCAPE"
bytes 12 to 14 : "2.0"

and also 

byte  15       : 3 (hex 0x03) Length of Data Sub-Block 
                 (three bytes of data to follow)
byte  16       : 1 (hex 0x01)
bytes 17 to 18 : 0 to 65535, an unsigned integer in 
                 lo-hi byte format. This indicate the 
                 number of iterations the loop should 
                 be executed.
                 
byte  19       : 0 (hex 0x00) a Data Sub-block Terminator. 
*/
struct GraphicControlExtension {
  byte identifier;          /** 0x21 */
  byte applicationId;       /** 0xF9 */
  byte blockSize;           /** 0x04*/
  byte packed;
  word delayTime;
  byte transparentColorIndex;
  byte blockTerminator;
  GraphicControlExtension(byte identifier = GRAPHIC_CONTROL_EXTENSION_IDENTIFIER, byte applicationId = GRAPHIC_CONTROL_APPLICATION_IDENTIFIER, 
    byte blockSize = GRAPHIC_CONTROL_APPLICATION_SIZE, byte packed = 0, word delayTime = 0, byte transparentColorIndex = 0) {
    this->identifier = identifier; this->applicationId = applicationId; this->blockSize = blockSize; this->packed = packed; 
    this->delayTime = delayTime; this->transparentColorIndex = transparentColorIndex; blockTerminator = 0;};
  bool operator==(GraphicControlExtension &other) {
    return ((identifier == other.identifier) && (applicationId == other.applicationId) && (blockSize == other.blockSize)
      && (packed == other.packed) && (delayTime == other.delayTime) && (transparentColorIndex == other.transparentColorIndex));
  };
};

//---------------------------------------------------------------------------
#pragma pack (pop)
#endif
