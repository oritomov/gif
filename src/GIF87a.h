/**
 * @(#)gif87a.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef GIF87aH
#define GIF87aH

#include "GifCommon.h"

#pragma pack (push)
#pragma pack (1)

#define _GIF_ "GIF"
#define _89a_ "89a"
#define GifEof 0x3B

//---------------------------------------------------------------------------
struct GifHeader {
  byte signature[3];     /* Header Signature (always "GIF") */
  byte version[3];       /* GIF format version("87a" or "89a") */
  GifHeader(byte* signature = (byte*)_GIF_, byte* version = (byte*)_89a_) {
    for (int i = 0; i < sizeof(this->signature); i++) this->signature[i] = signature[i];
    for (int i = 0; i < sizeof(this->version); i++) this->version[i] = version[i];
  };
  bool operator==(GifHeader &other) {
    for (int i = 0; i < sizeof(this->signature); i++) if (this->signature[i] != signature[i]) return false;
    for (int i = 0; i < sizeof(this->version); i++) if (this->version[i] != version[i]) return false;
    return true;
  };
};

//---------------------------------------------------------------------------
struct GifLogicalScreenDescriptor {
  word screenWidth;      /* Width of Display Screen in Pixels */
  word screenHeight;     /* Height of Display Screen in Pixels */
  byte packed;           /* Screen and Color Map Information */
  byte backgroundColor;  /* Background Color Index */
  byte aspectRatio;      /* Pixel Aspect Ratio */
  GifLogicalScreenDescriptor(word screenWidth = 0, word screenHeight = 0, byte packed = 0, byte backgroundColor = 0, byte aspectRatio = 0) {
    this->screenWidth = screenWidth; this->screenHeight = screenHeight; this->packed = packed; this->backgroundColor = backgroundColor; this->aspectRatio = aspectRatio;
  };
  bool operator==(GifLogicalScreenDescriptor &other) {
    return ((screenWidth == other.screenWidth) && (screenHeight == other.screenHeight) && (packed  == other.packed)
      && (backgroundColor == other.backgroundColor) && (aspectRatio == other.aspectRatio));
  };
};

/*
Packed contains the following four subfields of data (bit 0 is the least significant bit, or LSB): 

Bits 0-2 Size of the Global Color Table 
Bit 3 Color Table Sort Flag 
Bits 4-6 Color Resolution 
Bit 7 Global Color Table Flag 
*/
//---------------------------------------------------------------------------

struct GifColorTable{
  byte red;          /* Red Color Element */
  byte green;        /* Green Color Element */
  byte blue;         /* Blue Color Element */
  GifColorTable(byte r, byte g, byte b) {red = r; green = g; blue = b;};
  bool operator==(GifColorTable &other) {return ((red == other.red) && (green == other.green) && (blue  == other.blue));};
};
//---------------------------------------------------------------------------

#define IMAGE_DECRIPTOR_IDENTIFIER 0x2C

struct GifImageDescriptor {
  byte identifier;   /* Image Descriptor identifier 0x2C */
  word left;         /* X position of image on the display */
  word top;          /* Y position of image on the display */
  word width;        /* Width of the image in pixels */
  word height;       /* Height of the image in pixels */
  byte packed;       /* Image and Color Table Data Information */
  GifImageDescriptor(byte id = IMAGE_DECRIPTOR_IDENTIFIER, word left = 0, word top = 0, word width = 0, word height = 0, byte packed = 0) {
    this->identifier = id; this->left = left; this->top = top; this->width = width; this->height = height; this->packed = packed;
  };
  bool operator==(GifImageDescriptor &other) {
    return ((identifier == other.identifier) && (left == other.left) && (top  == other.top)
      && (width == other.width) && (height == other.height) && (packed  == other.packed));
  };
};

/*
Packed contains the following five subfields of data (bit 0 is the LSB): 

Bit 0 Local Color Table Flag 
Bit 1 Interlace Flag 
Bit 2 Sort Flag 
Bits 3-4 Reserved 
Bits 5-7 Size of Local Color Table Entry 
*/
//---------------------------------------------------------------------------
#pragma pack (pop)
#endif
