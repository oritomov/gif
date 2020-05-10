/**
 * @(#)gifwriter.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "GifWriter.h"

//---------------------------------------------------------------------------
GifWriter::GifWriter(char* filename)  : LzwWriter(), file(GifFile(filename, FILE_WRITE)) {
  color = NULL;
  header=NULL;
  screen=NULL;
  descr=NULL;
  interlaced = false;
}

//---------------------------------------------------------------------------
GifWriter::~GifWriter() {
  flush();
  if(color)
    free(color);
  if(header)
    free(header);
  if(screen)
    free(screen);
  if(descr)
    free(descr);
}

//---------------------------------------------------------------------------
void GifWriter::flush() {
//  if (file) {
    write(_init + 1);
    byte end = 0;
    if (!fwrite(&end, 1, 1))
#if defined(ARDUINO)
      GifCommon::runtime_error("err: writing end!");
#elif defined(__BORLANDC__)
      throw std::runtime_error("err: writing end!");
#endif // defined
    end = GifEof;
    if (!fwrite(&end, 1, 1))
#if defined(ARDUINO)
      GifCommon::runtime_error("err: writing end!");
#elif defined(__BORLANDC__)
      throw std::runtime_error("err: writing end!");
#endif // defined
    file.flush();
//    file.close();
//  }
}

//---------------------------------------------------------------------------
size_t GifWriter::fwrite(void* ptr, size_t size, size_t nmemb) {
  return file.write(ptr, size, nmemb);
}

//---------------------------------------------------------------------------
void GifWriter::writeHeader(GifHeader* header=NULL) {
  GifHeader temp;
  if (!header)
    header = this->header;           // use this* when no parameter
  if (!header)
    header = (GifHeader*)&temp;      // use temp when no other way
  if (!fwrite(&header, sizeof(GifHeader), 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: writing header!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: writing header!");
#endif // defined
}

//---------------------------------------------------------------------------
void GifWriter::writeScreen(GifLogicalScreenDescriptor* screen=NULL) {
  GifLogicalScreenDescriptor temp(
  width,
  height,
  0xf0 |                             // Global Color Table and max Resolution
  (      (_init^0x02)==0?0:
   (     (_init^0x04)==0?1:
    (    (_init^0x08)==0?2:
     (   (_init^0x10)==0?3:
      (  (_init^0x20)==0?4:
       ( (_init^0x40)==0?5:
        ((_init^0x80)==0?6:7))))))),
  0,
  0);
  if (!screen)
    screen = this->screen;                       // use this* when no parameter
  if (!screen)
    screen = (GifLogicalScreenDescriptor*)&temp; // use temp when no other way
  //PixelAspectRatio = (AspectRatio + 15) / 64;
  //printf("Packed: %x\n", header.Packed);
  if (!fwrite(&screen, sizeof(GifLogicalScreenDescriptor), 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: writing screen!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: writing screen!");
#endif // defined
}

//---------------------------------------------------------------------------
void GifWriter::writeColors() {
  if (fwrite(color, sizeof(GifColorTable), _init) != (unsigned)_init)
#if defined(ARDUINO)
    GifCommon::runtime_error("err: writing colors!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: writing colors!");
#endif // defined
}

//---------------------------------------------------------------------------
void GifWriter::writeDescriptor(GifImageDescriptor* descr=NULL) {
  GifImageDescriptor temp(IMAGE_DECRIPTOR_IDENTIFIER, 0, 0, width, height, (interlaced?64:0));
  if (!descr)
    descr = this->descr;                 // use this* when no parameter
  if (!descr)
    descr = (GifImageDescriptor*)&temp;  // use temp when no other way
  if (!fwrite(&descr, sizeof(GifImageDescriptor), 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: writing descriptor!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: writing descriptor!");
#endif // defined
}

//---------------------------------------------------------------------------
void GifWriter::writeAll() {
  writeHeader(header);
  writeScreen(screen);
  writeColors();
  writeDescriptor(descr);
  byte bits = (      (_init^0x02)==0?1:
               (     (_init^0x04)==0?2:
                (    (_init^0x08)==0?3:
                 (   (_init^0x10)==0?4:
                  (  (_init^0x20)==0?5:
                   ( (_init^0x40)==0?6:
                    ((_init^0x80)==0?7:8)))))));
  if (!fwrite(&bits, 1, 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: writing bits!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: writing bits!");
#endif // defined
}

//---------------------------------------------------------------------------
void GifWriter::writeBufferSize(byte buffSize) {
  if (!fwrite(&buffSize, 1, 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: writing buffer size!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: writing buffer size!");
#endif // defined
}

//---------------------------------------------------------------------------
void GifWriter::writeBuffer(byte* buffer, int buffSize) {
  if (!fwrite(buffer, buffSize, 1))
#if defined(ARDUINO)
    GifCommon::runtime_error("err: writing buffer!");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: writing buffer!");
#endif // defined
}

//---------------------------------------------------------------------------
void GifWriter::write(GifColorTable* color) {
  for (int index = 0; index < _init; index++) {
    if (*color == this->color[index]) {
      write(index);
      break;
    }
  }
}

//---------------------------------------------------------------------------
void GifWriter::write(int index) {
  LzwWriter::write(index);
}

//---------------------------------------------------------------------------
void GifWriter::setWidth(word width) {
  this->width = width;
}

//---------------------------------------------------------------------------
void GifWriter::setHeight(word height) {
  this->height = height;
}

//---------------------------------------------------------------------------
void GifWriter::setColorsCount(int count) {
  setInit(count);
  color  = (GifColorTable*)realloc(color, _init * sizeof(GifColorTable));
  memset(color, 0, _init * sizeof(GifColorTable)); // black 
  setBits((      (_init^0x02)==0?1:
           (     (_init^0x04)==0?2:
            (    (_init^0x08)==0?3:
             (   (_init^0x10)==0?4:
              (  (_init^0x20)==0?5:
               ( (_init^0x40)==0?6:
                ((_init^0x80)==0?7:8))))))));
  write(_init);
}

//---------------------------------------------------------------------------
GifColorTable* GifWriter::getColor(int index) {
  if (index >= _init) {
    char buff[64];
    sprintf(buff, "err3: %d index out of bounds!", index);
#if defined(ARDUINO)
    GifCommon::runtime_error((char*)buff);
#elif defined(__BORLANDC__)
    throw std::runtime_error(buff);
#endif // defined
  }
  return &color[index];
}

//---------------------------------------------------------------------------
void GifWriter::setColor(int index, GifColorTable* color) {
  if (index >= _init) {
    char buff[64];
    sprintf(buff, "err4: %d index out of bounds!", index);
#if defined(ARDUINO)
    GifCommon::runtime_error((char*)buff);
#elif defined(__BORLANDC__)
    throw std::runtime_error(buff);
#endif // defined
  }
  this->color[index].red = color->red;
  this->color[index].green = color->green;
  this->color[index].blue = color->blue;
}

//---------------------------------------------------------------------------
void GifWriter::setHeader(GifHeader* header){
  this->header = new GifHeader(header->signature, header->version);
}

//---------------------------------------------------------------------------
void GifWriter::setScreenDescriptor(GifLogicalScreenDescriptor* screen){
  this->screen = new GifLogicalScreenDescriptor(screen->screenWidth, screen->screenHeight, screen->packed, screen->backgroundColor, screen->aspectRatio);
}

//---------------------------------------------------------------------------
void GifWriter::setImageDescriptor(GifImageDescriptor* descr) {
  this->descr = new GifImageDescriptor(descr->identifier, descr->left, descr->top, descr->width, descr->height, descr->packed);
}
