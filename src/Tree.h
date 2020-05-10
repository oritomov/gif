/**
 * @(#)tree.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef TreeH
#define TreeH

#include "GifCommon.h"

//---------------------------------------------------------------------------
class LzwNode {
  public:
    LzwNode(LzwNode*, int);
    virtual ~LzwNode(void);

    void clear(void);
    bool add(int, LzwNode*&);
    int getId(void);

  protected:
    virtual int getInit(void);
    virtual int getNext(void);

  private:
    int code;
    int id;
    LzwNode* parent;
    LzwNode** child;
};
//---------------------------------------------------------------------------
class LzwTree: public LzwNode {
  public:
    LzwTree(void);
    virtual ~LzwTree(void);

    int getSize(void);
    void setInit(int);

  protected:
    virtual int getInit(void);
    virtual int getNext(void);

  private:
    int init;
    int ids;
};
//---------------------------------------------------------------------------
#endif
