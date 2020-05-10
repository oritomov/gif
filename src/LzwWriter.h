/**
 * @(#)lzwwriter.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef LzwWriterH
#define LzwWriterH
//---------------------------------------------------------------------------
#include "lzwio.h"
//---------------------------------------------------------------------------
#define MAXTSIZE 0x500
//MAXTSIZE 0x1000
#define BUFFSIZE 0xfe
//---------------------------------------------------------------------------
class LzwWriter : public LzwOut {

  public:
    LzwWriter();

    virtual void write(int);

  protected:
    virtual void writeAll() = 0;
    virtual void writeBuffer(byte*, int) = 0;
    virtual void writeBufferSize(byte) = 0;
    void setBits(byte);

  private:
    bool firsttime;
    byte buffer[512];
    byte bits;
    int buffSize;
    int pos;
    int shift;
    int mask;
    void writeBuffer();
};
//---------------------------------------------------------------------------
#endif
