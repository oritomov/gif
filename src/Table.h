/**
 * @(#)table.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef TableH
#define TableH

#include "GifCommon.h"

//---------------------------------------------------------------------------
#define STEP 0x1
#define HASH 5381
//---------------------------------------------------------------------------
class LzwTable;
class LzwLine {

  friend class LzwTable;

  public:
    LzwLine(void);
    ~LzwLine(void);

    void clear(void);
    void add(int);
    void assign(LzwLine*);

    int getCode(int);
    int getSize(void);
    unsigned int getHash(void);

  protected:
    void setSize(int);
    void rehash(int);

  private:
    int size;
    int sizeofline;
    int* line;
    unsigned int hash;
};
//---------------------------------------------------------------------------
class LzwTable {

  public:
    LzwTable();
    ~LzwTable();

    void clear();
    LzwLine* newLine(); // decompress only
    void add(LzwLine*);
    int indexOf(LzwLine*);
    LzwLine* getLine(int);
    int getSize(void);

  protected:
    void setSize(int);

  private:
    int size;
    int sizeoftable;
    LzwLine** table;
};
//---------------------------------------------------------------------------
#endif
