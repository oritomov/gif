/**
 * @(#)gifreader.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef GifReaderH
#define GifReaderH

#include "gif87a.h"
#include "gif89a.h"
#include "table.h"
#include "lzwreader.h"
#include "GifFile.h"

//---------------------------------------------------------------------------
class GifReader : public LzwReader {

  public:
    GifReader(const char*);
    ~GifReader();

    virtual int read();

    int getColors();
    GifColorTable* getColor(int);

    word getWidth();
    word getHeight();

  protected:
    char* version;       /* GIF format version("87a" or "89a") */
//    // Logical Screen Descriptor
//    //bool hasSorted;
//    //byte resolution;
//    //bool hasGlobalColorTable;
//    //byte background;  /* Background Color Index */
//    //byte aspectRatio;      /* Pixel Aspect Ratio */
    GifColorTable* color;
    void open();
    void readHeader();
    void readScreen();
    void readColors();
    int readExtension();
    void readDescriptor(int);
    virtual int readBufferSize();
    virtual void readBuffer(byte[], int);

  private:
    size_t fread(void*, size_t, size_t);
    
    GifFile file;
    word width;       /* Width of Display Screen in Pixels */
    word height;      /* Height of Display Screen in Pixels */
    bool interlaced;
    bool globalColors; /* Global color map follows Descriptor */
    GifImageDescriptor* descr;

    int line;
    int code;
};
//---------------------------------------------------------------------------
#endif
