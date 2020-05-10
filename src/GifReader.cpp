/**
 * @(#)gifreader.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "GifReader.h"
//---------------------------------------------------------------------------

GifReader::GifReader(const char* filename) : LzwReader(), file(GifFile(filename, FILE_READ)) {
  version = NULL;
  color = NULL;
  descr = NULL;
  line = -1;
  code = -1;
  open();
}

//---------------------------------------------------------------------------
GifReader::~GifReader() {
  if(version)
    free(version);
  if(color)
    free(color);
  if(descr)
    free(descr);
}

//---------------------------------------------------------------------------
void GifReader::open() {
  readHeader();
  readScreen();
  color = (GifColorTable*)calloc(_init, sizeof(GifColorTable));
  if (globalColors)
    readColors();
  int offset = readExtension();
  descr = new (GifImageDescriptor);
  readDescriptor(offset);
  byte bits;
  if (!fread(&bits, 1, 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: reading bits!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: reading bits!");
#endif // defined
#ifdef DEBUG  
  printf("bits: %x\n", bits);
#endif //DEBUG
  setBits(bits);
}

//---------------------------------------------------------------------------
size_t GifReader::fread(void* ptr, size_t size, size_t nmemb) {
  return file.read(ptr, size, nmemb);
}

//---------------------------------------------------------------------------
void GifReader::readHeader() {
  GifHeader header;
  if (!fread(&header, sizeof(GifHeader), 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: reading header!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: reading header!");
#endif // defined
  version = (char*)malloc(sizeof(header.version) + 1);
  memcpy(version, header.version, sizeof(header.version));
  version[sizeof(header.version)] = 0;
#ifdef DEBUG  
  printf("version: %s\n", version);
#endif //DEBUG
}

//---------------------------------------------------------------------------
void GifReader::readScreen() {
  GifLogicalScreenDescriptor screen;
  if (!fread(&screen, sizeof(GifLogicalScreenDescriptor), 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: reading screen!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: reading screen!");
#endif // defined
  width = screen.screenWidth;
#ifdef DEBUG  
  printf("width: %i\n", getWidth());
#endif //DEBUG
  height = screen.screenHeight;
#ifdef DEBUG  
  printf("height: %i\n", getHeight());
#endif //DEBUG
  _init = (1L << ((screen.packed & 0x07) + 1));
#ifdef DEBUG  
  printf("Colors: %i\n", getColors());
#endif //DEBUG
  globalColors = screen.packed & 0x80;
#ifdef DEBUG  
  if (globalColors)
    printf("Global color map.\n");
#endif //DEBUG
  //pixelAspectRatio = (aspectRatio + 15) / 64;
}

//---------------------------------------------------------------------------
void GifReader::readColors() {
  if (fread(color, 3, _init) != (size_t)_init)
#if defined(ARDUINO)
    GifCommon::runtime_error("err: reading colors!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: reading colors!");
#endif // defined
}

//---------------------------------------------------------------------------
int GifReader::readExtension() {
  if (strcmp(version, _89a_)!=0)
    return 0;
  while (true) {
    byte identifier;
    if (!fread(&identifier, sizeof(byte), 1))
#if defined(ARDUINO)
      GifCommon::runtime_error("err: reading extension identifier!");
#elif defined(__BORLANDC__)
      throw std::runtime_error("err: reading extension identifier!");
#endif // defined
    if (identifier!=GRAPHIC_CONTROL_EXTENSION_IDENTIFIER)
      return 1;
#ifdef DEBUG  
    printf("Graphic control extension.\n");
#endif //DEBUG
    GraphicControlExtension extension;
    byte application;
    if (!fread(&application, sizeof(byte), 1))
#if defined(ARDUINO)
      GifCommon::runtime_error("err: reading extension application id!");
#elif defined(__BORLANDC__)
      throw std::runtime_error("err: reading extension application id!");
#endif // defined
#ifdef DEBUG  
    printf("Graphic control extension application %x.\n", application);
#endif //DEBUG
    extension.applicationId = application;
    if (application == GRAPHIC_CONTROL_APPLICATION_IDENTIFIER) {
      if (!fread(&extension+2*sizeof(byte), sizeof(GraphicControlExtension)-2*sizeof(byte), 1))
#if defined(ARDUINO)
        GifCommon::runtime_error("err: reading extension!");
#elif defined(__BORLANDC__)
        throw std::runtime_error("err: reading extension!");
#endif // defined
    } else 
    while (true) {
      byte blockSize;
      if (!fread(&blockSize, sizeof(byte), 1))
#if defined(ARDUINO)
        GifCommon::runtime_error("err: reading extension application blockSize!");
#elif defined(__BORLANDC__)
        throw std::runtime_error("err: reading extension application blockSize!");
#endif // defined
#ifdef DEBUG  
      printf("Graphic control extension application blockSize %x.\n", blockSize);
#endif //DEBUG
      if (blockSize == 0) 
        break;
      byte block[0x100];
      block[blockSize] = 0;
      if (!fread(block, sizeof(byte), blockSize))
#if defined(ARDUINO)
        GifCommon::runtime_error("err: reading extension application block!");
#elif defined(__BORLANDC__)
        throw std::runtime_error("err: reading extension application block!");
#endif // defined
#ifdef DEBUG  
      for (int i = 0; i < blockSize; i++)
        printf("%x ", block[i]);
      printf("  %.*s\n", (int)blockSize, block);
#endif //DEBUG
    }
  }
}

//---------------------------------------------------------------------------
void GifReader::readDescriptor(int offset) {
  if (!fread(((byte*)descr)+offset, sizeof(GifImageDescriptor)-offset, 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: reading descriptor!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: reading descriptor!");
#endif // defined
#ifdef DEBUG  
  //printf("Separator: %i\n", descr->separator);
  printf("Left: %i\n",      descr->left);
  printf("Top: %i\n",       descr->top);
  printf("Width: %i\n",     descr->width);
  printf("Height: %i\n",    descr->height);
  printf("Packed: %i\n",    descr->packed);
#endif //DEBUG
  interlaced = descr->packed & 0x40;
}

//---------------------------------------------------------------------------
int GifReader::readBufferSize() {
  byte bufferSize;
  if (!fread(&bufferSize, 1, 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: reading buffer size!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: reading buffer size!");
#endif // defined
  return bufferSize;
}

//---------------------------------------------------------------------------
void GifReader::readBuffer(byte* buffer, int buffSize) {
  if (!fread(buffer, buffSize, 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: reading buffer!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: reading buffer!");
#endif // defined
}

//---------------------------------------------------------------------------
int GifReader::read() {
  int result = -1;
  if (line < 0) {
    line = LzwReader::read();
    if (line < 0) {
      return result;
    }
    code = 0;
  }
  result = getLine(line)->getCode(code++);
  if (result < 0)
    printf("err\n");
  if (code >= getLine(line)->getSize()) {
    line = -1;
  }
  return result;
}

//---------------------------------------------------------------------------
word GifReader::getWidth() {
  return width;
}

//---------------------------------------------------------------------------
word GifReader::getHeight() {
  return height;
}

//---------------------------------------------------------------------------
int GifReader::getColors() {
  return _init;
}

//---------------------------------------------------------------------------
GifColorTable* GifReader::getColor(int index) {
  if (index >= _init) {
    char buff[64];
    sprintf(buff, "err2: %d index out of bounds!", index);
#if defined(ARDUINO)
    GifCommon::runtime_error((char*)buff);
#elif defined(__BORLANDC__)
    throw std::runtime_error(buff);
#endif // defined
  }
  return &color[index];
}
