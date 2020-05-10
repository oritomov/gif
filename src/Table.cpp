/**
 * @(#)table.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "table.h"

//---------------------------------------------------------------------------
LzwLine::LzwLine() {
  size = 0;
  sizeofline = 0;
  line = NULL;
  hash = HASH;
}

//---------------------------------------------------------------------------
LzwLine::~LzwLine() {
  clear();
}

//---------------------------------------------------------------------------
void LzwLine::clear() {
  size = 0;
  sizeofline = 0;
  free(line);
  line = NULL;
  hash = HASH;
}

//---------------------------------------------------------------------------
void LzwLine::add(int code) {
  setSize(size + 1);
  line[size - 1] = code;
  rehash(code);
}

//---------------------------------------------------------------------------
void LzwLine::assign(LzwLine* other) {
  setSize(other->size);
  for (int i = 0; i < size; i++) {
    line[i] = other->getCode(i);
    rehash(line[i]);
  }
}

//---------------------------------------------------------------------------
void LzwLine::setSize(int newSize) {
  if (!line) {
    sizeofline = STEP;
    while (newSize > sizeofline) {
      sizeofline += STEP;
    }
    line = (int*)malloc(sizeofline * sizeof(int));
  } else if (newSize > sizeofline) {
    while (newSize > sizeofline) {
      sizeofline += STEP;
    }
    line = (int*)realloc(line, sizeofline * sizeof(int));
  }
  size = newSize;
}

//---------------------------------------------------------------------------
void LzwLine::rehash(int code) {
  // Professor Daniel J. Bernstein algorithm
  hash = ((hash << 5) + hash) + code;
}

//---------------------------------------------------------------------------
int LzwLine::getCode(int index) {
  if (index >= size) {
    char buff[64];
    sprintf(buff, "err5: %d index out of bounds!", index);
#if defined(ARDUINO)
    GifCommon::runtime_error((char*)buff);
#elif defined(__BORLANDC__)
    throw std::runtime_error(buff);
#endif // defined
  }
  return line[index];
}

//---------------------------------------------------------------------------
int LzwLine::getSize() {
  return size;
}

//---------------------------------------------------------------------------
unsigned int LzwLine::getHash() {
  return hash;
}

//===========================================================================
LzwTable::LzwTable() {
  size = 0;
  sizeoftable = 0;
  table = NULL;
}

//---------------------------------------------------------------------------
LzwTable::~LzwTable() {
//  clear();
}

//---------------------------------------------------------------------------
void LzwTable::clear() {
  for (int i = 0; i < size; i++)
    table[size - 1]->clear();
  size = 0;
  sizeoftable = 0;
  free(table);
  table = NULL;
}

//===========================================================================
// decompress only
LzwLine* LzwTable::newLine() {
  setSize(size + 1);
  LzwLine* result = new LzwLine();
  table[size - 1] = result;
  return result;
}

//---------------------------------------------------------------------------
void LzwTable::add(LzwLine* line) {
  setSize(size + 1);
  table[size - 1] = new LzwLine();
  table[size - 1]->assign(line);
}

//---------------------------------------------------------------------------
void LzwTable::setSize(int newSize) {
  if (!table) {
    sizeoftable = STEP;
    while (newSize > sizeoftable) {
      sizeoftable += STEP;
    }
    table = (LzwLine**)malloc(sizeoftable * sizeof(LzwLine*));
  } else if (newSize > sizeoftable) {
    sizeoftable += STEP;
    while (newSize > sizeoftable) {
      sizeoftable += STEP;
    }
    table = (LzwLine**)realloc(table, sizeoftable * sizeof(LzwLine*));
  }
  size = newSize;
}

//---------------------------------------------------------------------------
int LzwTable::indexOf(LzwLine* line) {
  int lineSize = line->getSize();
  for (int i = 0; i < size; i++) {
    if (lineSize == table[i]->getSize()) {
      if (line->getHash() == table[i]->getHash()) {
        bool result = true;
        for (int j = 0; j < lineSize; j++)
          if ( !(result = (line->getCode(j) == table[i]->getCode(j))))
            break;
        if (result)
          return i;
      }
    }
  }
  return -1;
}

//---------------------------------------------------------------------------
LzwLine* LzwTable::getLine(int index) {
  if (index >= size) {
    char buff[64];
    sprintf(buff, "err6: %x index out of bounds! %x", index, size);
#if defined(ARDUINO)
    GifCommon::runtime_error((char*)buff);
#elif defined(__BORLANDC__)
    throw std::runtime_error(buff);
#endif // defined
  }
  return table[index];
}

//---------------------------------------------------------------------------
int LzwTable::getSize() {
  return size;
}
