/**
 * @(#)lzwreader.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef LzwReaderH
#define LzwReaderH
//---------------------------------------------------------------------------
#include "lzwio.h"

#define MAX_SIZE 4096

//---------------------------------------------------------------------------
class LzwReader: public LzwIn {

  public:
    LzwReader();
    virtual ~LzwReader();

    virtual int read();

  protected:
    virtual void init();
    virtual void readBuffer(byte*, int) = 0;
    virtual int readBufferSize() = 0;
    void setBits(byte);

  private:
    byte buffer[256];
    byte bits;
    int buffSize;
    int pos;
    int shift;
    int mask;
    void readBuffer();
};
//---------------------------------------------------------------------------
#endif
