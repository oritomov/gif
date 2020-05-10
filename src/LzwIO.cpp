/**
 * @(#)lzw.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "lzwio.h"
//---------------------------------------------------------------------------
Lzw::Lzw() {
  _init = -1;
}

//---------------------------------------------------------------------------
Lzw::~Lzw() {
}

//===========================================================================
LzwIn::LzwIn() : Lzw() {
  old = -1;
  //table = NULL;
}

//---------------------------------------------------------------------------
LzwIn::~LzwIn() {
  //delete table;
  table.clear();
}

//---------------------------------------------------------------------------
void LzwIn::init() {
  table.clear();
  for (int i = 0; i < _init + 2; i++) {
    LzwLine* line = table.newLine();
    line->add(i);
  }
  old = -1;
}

//---------------------------------------------------------------------------
int LzwIn::decompress(int code) {
  if (code == _init) {
    init();
    return -1;
  }
  if (old < 0) {
    /** CODE = */
    /** OLD = CODE */
    old = code;
    /** output the string for <code> */
    return code;
  }
  LzwLine* line = table.newLine();
  /** CODE = */
  /** does <code> exist? */
  if (code < table.getSize()) {
    /** [...] <- translation for <old> */
    line->assign(table.getLine(old));
    /** K <- first character of translation for <code> */
    int k = table.getLine(code)->getCode(0);
    /** add [...]K to the string table */
    line->add(k);
    /** OLD = CODE */
    old = code;
    /** output the string for <code> */
    return code;
  }
  /** [...] <- translation for <old> */
  line->assign(table.getLine(old));
  /** K <- first character of [...] */
  int k = line->getCode(0);
  /** add [...]K to string table */
  line->add(k);
  /** OLD = CODE */
  old = code;
  /** output [...]K to charstream */
  return table.getSize() - 1;
}

//---------------------------------------------------------------------------
int LzwIn::getOld() {
  return old;
}

//---------------------------------------------------------------------------
int LzwIn::getSize() {
  return table.getSize();
}

//---------------------------------------------------------------------------
LzwLine* LzwIn::getLine(int index) {
  if (index >= table.getSize()) {
    char buff[64];
    sprintf(buff, "err1: 0x%x index out of bounds!", index);
#if defined(ARDUINO)
    GifCommon::runtime_error((char*)buff);
#elif defined(__BORLANDC__)
    throw std::runtime_error(buff);
#endif // defined
  }
  return table.getLine(index);
}

//===========================================================================
LzwOut::LzwOut() : Lzw() {
  old = NULL;
}

//---------------------------------------------------------------------------
LzwOut::~LzwOut() {
}

//---------------------------------------------------------------------------
void LzwOut::init() {
  tree.clear();
  tree.setInit(_init);
  old = NULL;
}

//---------------------------------------------------------------------------
int LzwOut::compress(int code) {
  if (code == _init) {
    int temp;
    if (old) {
      temp = old->getId();
    } else {
      temp = -1;
    }
    init();
    //printf("0: %d\n", temp);
    return temp;
  }
  // first OLD =
  if (!old) {
    tree.add(code, old);
    //printf("1: -1\n");
    return -1;
  }
  /** CODE = */
  /** OLD+CODE */
  LzwNode* node;
  /** doesn't <code> exist? */
  if (old->add(code, node)) {
    // output the code for OLD
    int result = old->getId();
    // add OLD+CODE to the string table
    tree.add(code, node);
    // OLD = CODE
    old = node;
    //printf("2: %d\n", result);
    return result;
  }
  // OLD = OLD+CODE
  old = node;
  //printf("3: -1\n");
  return -1;
}

//---------------------------------------------------------------------------
int LzwOut::getOld() {
  return old->getId();
}

//---------------------------------------------------------------------------
void LzwOut::setInit(int value) {
  tree.setInit(value);
  _init = value;
}

//---------------------------------------------------------------------------
int LzwOut::getSize() {
  return tree.getSize();
}
