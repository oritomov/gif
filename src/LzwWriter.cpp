/**
 * @(#)lzwwriter.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "LzwWriter.h"
//---------------------------------------------------------------------------
LzwWriter::LzwWriter() : LzwOut(){
  firsttime = true;
  bits = 0;
  buffSize = 0;
  pos = 0;
  shift = 0;
  mask = 0;
  for (int i = 0; i < sizeof(buffer); i++)
    buffer[i] = 0;
}

//---------------------------------------------------------------------------
void LzwWriter::writeBuffer() {
  if (firsttime) {
    writeAll();
    firsttime = false;
  }
  byte buffSize = BUFFSIZE;
  if (pos + ((shift + bits + 8 /** 7 + 1 */) / 8) < BUFFSIZE)
    buffSize = pos + ((shift + bits + 8 /** 7 + 1 */) / 8);
  writeBufferSize(buffSize);
  //printf("buffSize: %x\n", buffSize);
  writeBuffer(buffer, buffSize);
  //if (pos + ((shift + bits + 8 /** 7 + 1 */) / 8) >= BUFFSIZE) {
  if (buffSize == BUFFSIZE) {
    for (int i = 0; i < BUFFSIZE; i++)
      buffer[i] = 0;
  //  printf("pos: %x\n", pos);
  //  printf("shift: %x\n", shift);
  //  printf("bits: %x\n", bits);
    for (int i = 0; i <= pos + ((shift + bits + 8 /** 7 + 1 */) / 8) - BUFFSIZE; i++) {
      buffer[i] = buffer[BUFFSIZE + i];
      //printf("[%x] <- [%x]: %x\n", i, BUFFSIZE + i, buffer[BUFFSIZE + i]);
    }
    for (int i = BUFFSIZE; i < sizeof(buffer); i++)
      buffer[i] = 0;
    pos -= BUFFSIZE;
  }
  //printf("write buffer.\n");
}

//---------------------------------------------------------------------------
void LzwWriter::write(int index) {
  //printf("write: %x\n", index);
  if (index == -1) {
    //printf("warning!!!\n");
    int old = index = getOld();
    index = compress(index);
    if (index < 0)
      index = old;
  }
  else if (index == _init + 1) {
    write(/*GetOld()*/-1);
    //printf("writeOld: %x\n", getOld());
  }
  else if (index == _init) {
    index = compress(_init);//getOld();
    if (index < 0)
      index = _init;
    else {
      write(_init);
      while ((mask >> 1) > getSize()) {
        bits--;
        mask = mask >> 1;
      }
      index = compress(index);
    }
  } else
    index = compress(index);
  /*
  if (tableSize >= 0x1000) {
    _write(init);
  }
  */
  if (index >= 0)
  {
    //if (index == _init)
    //printf("write[%x]: %x\n", pos, index);
    //else {
    //printf("write[%x]: %x= ", pos, index);
    //for (int i = 0; i < line[index]->getSize(); i++)
    //  printf("%x, ",  line[index]->getCode(i));
    //printf("\n");
    //}
    for (int i = 0; i < ((8 + 7 + bits) / 8); i++){
      buffer[pos + i] = buffer[pos + i] | (((index<< shift) >> (i * 8)) & 0xff);
      //printf("[%x]:",  pos + i);
      //printf(" %x\n", buffer[pos + i]);
    }

    if (pos + ((8 + bits + shift) / 8) > BUFFSIZE)
      writeBuffer();

    if (index == _init + 1)
      writeBuffer();

    shift += bits + 1;
    pos += shift / 8;
    shift = shift % 8;

    if (index == _init) {
    }

  if (getSize() > MAXTSIZE) {
      write(_init);
    }

    if (getSize() - 1 > mask)  {
      bits++;
      mask = (mask << 1) | 0x01;
    }
  }
}

//---------------------------------------------------------------------------
void LzwWriter::setBits(byte value) {
  bits = value;
  mask = 1;
  for (int i = 0; i < bits; i++)
    mask = (mask << 1) | 0x01;
}
