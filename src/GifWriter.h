/**
 * @(#)gifwriter.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef GifWriterH
#define GifWriterH
//---------------------------------------------------------------------------
#include "gif87a.h"
#include "GifFile.h"
#include "lzwwriter.h"
//---------------------------------------------------------------------------
class GifWriter : public LzwWriter {

  public:
    GifWriter(char*);
    ~GifWriter();

    void write(GifColorTable*);
    virtual void write(int);

    void setWidth(word);
    void setHeight(word);
    void setColorsCount(int);
    GifColorTable* getColor(int);
    void setColor(int, GifColorTable*);
    
    void setHeader(GifHeader*);
    void setScreenDescriptor(GifLogicalScreenDescriptor*);
    void setImageDescriptor(GifImageDescriptor*);

  protected:
    void flush();
    void writeHeader(GifHeader*);
    void writeScreen(GifLogicalScreenDescriptor*);
    void writeColors();
    void writeDescriptor(GifImageDescriptor*);
    virtual void writeAll();
    virtual void writeBufferSize(byte);
    virtual void writeBuffer(byte*, int);

  private:
    GifFile file;
    word width;      /* Width of Display Screen in Pixels */
    word height;     /* Height of Display Screen in Pixels */
    bool interlaced;
    GifColorTable* color;
    GifHeader* header;
    GifLogicalScreenDescriptor* screen;
    GifImageDescriptor* descr;

    size_t fwrite(void*, size_t, size_t);
};
//---------------------------------------------------------------------------
#endif
