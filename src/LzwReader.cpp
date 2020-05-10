/**
 * @(#)lzwreader.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "LzwReader.h"
//---------------------------------------------------------------------------
LzwReader::LzwReader() : LzwIn() {
  bits = 0;
  buffSize = 0;
  pos = 0;
  shift = 0;
  mask = 0;
}

//---------------------------------------------------------------------------
LzwReader::~LzwReader() {
}

//---------------------------------------------------------------------------
void LzwReader::init() {
  LzwIn::init();
  while ((mask >> 1) > getSize()) {
    bits--;
    mask = mask >> 1;
  }
}

//---------------------------------------------------------------------------
void LzwReader::setBits(byte value) {
  bits = value;
  mask = 1;
  for (int i = 0; i < bits; i++)
    mask = (mask << 1) | 0x01;
}

//---------------------------------------------------------------------------
int LzwReader::read() {
  if ((buffSize == 0) | (pos + ((shift + bits + 8 /** 7 + 1 */) / 8) > buffSize))
    readBuffer();
  int result = 0;
  for (int i = 0; i < ((8 + 7 + bits) / 8); i++)
    result += buffer[pos + i] << (i * 8);
  result = result >> shift;
  result = result & mask;

  shift += bits + 1;
  pos += shift / 8;
  shift = shift % 8;

  if (result == _init + 1) {
#ifdef DEBUG  
    printf("end!\n");
#endif //DEBUG
    return -1;
  }
  else if (result == _init) {
#ifdef DEBUG  
    printf("read[%x]: %x\n", pos, result);
#endif //DEBUG
    decompress(result);
    result = read();
  } else {
    result = decompress(result);
//#ifdef DEBUG  
//  printf("read[%x]: %x= ", pos, result);
//  for (int i = 0; i < getLine(result)->getSize(); i++)
//    printf("%x, ",  getLine(result)->getCode(i));
//  printf(" \n");
//#endif //DEBUG
  }
  if ((getSize() > mask) && (getSize() < MAX_SIZE)) {
    bits++;
    mask = (mask << 1) | 0x01;
  }

  return result;
}

//---------------------------------------------------------------------------
void LzwReader::readBuffer() {
  int old = buffSize;
  buffSize = readBufferSize();
//#ifdef DEBUG  
//    printf("buffSize: %x\n", buffSize);
//#endif //DEBUG

  if (pos < old)
    for (int i = 0; i + pos < old; i++)
      buffer[i] = buffer[pos + i];
  else
    old = pos;

  if (buffSize > 0)
    readBuffer(&buffer[old - pos], buffSize);

  buffSize += old - pos;
  pos = 0;
}
