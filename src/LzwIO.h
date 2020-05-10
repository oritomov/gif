/**
 * @(#)lzwio.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef LzwIOH
#define LzwIOH
//---------------------------------------------------------------------------

#include "tree.h"
#include "table.h"

//---------------------------------------------------------------------------
class Lzw {

  public:
    Lzw();
    virtual ~Lzw();
  
  protected:
    int _init;
};

//---------------------------------------------------------------------------
class LzwIn : public Lzw {

  public:
    LzwIn();
    virtual ~LzwIn();

  protected:
    virtual void init(void);
    int decompress(int);
    int getOld();
    int getSize(void);
    LzwLine* getLine(int);

  private:
    LzwTable table;
    int old;
};

//---------------------------------------------------------------------------
class LzwOut : public Lzw {

  public:
    LzwOut();
    virtual ~LzwOut();

  protected:
    virtual void init(void);
    int compress(int);
    int getOld();
    void setInit(int);
    int getSize(void);

  private:
    LzwTree tree;
    LzwNode* old;
};

//---------------------------------------------------------------------------
#endif
