/**
 * @(#)tree.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "tree.h"
//---------------------------------------------------------------------------
LzwNode::LzwNode(LzwNode* parent, int code) {
  this->code = code;
  this->parent = parent;
  child = NULL;
  if (parent) {
    if (parent->getId() < 0) {
      id = code;
    } else {
      id = parent->getNext();
    }
  } else {
    id = -1;
  }
}

//---------------------------------------------------------------------------
LzwNode::~LzwNode(void) {
  clear();
}

//---------------------------------------------------------------------------
void LzwNode::clear(void) {
  if (child) {
    for (int i = 0; i < getInit(); i++) {
      if (child[i]) {
        delete child[i];
      }
    }
    free(child);
    child = NULL;
  }
}

//---------------------------------------------------------------------------
bool LzwNode::add(int code, LzwNode*& node) {
  if (!child) {
    child = (LzwNode**) malloc(getInit() * sizeof(LzwNode*));
    for (int i = 0; i < getInit(); i++) {
      child[i] = NULL;
    }
  }
  node = child[code];
  if (node) {
    return false;
  }
  node = new LzwNode(this, code);
  child[code] = node;
  return true;
}

//---------------------------------------------------------------------------
int LzwNode::getId(void) {
  return id;
}

//---------------------------------------------------------------------------
int LzwNode::getInit() {
  return parent->getInit();
}

//---------------------------------------------------------------------------
int LzwNode::getNext() {
  return parent->getNext();
}

//===========================================================================
LzwTree::LzwTree(void) : LzwNode(NULL, -1) {
}

//---------------------------------------------------------------------------
LzwTree::~LzwTree(void) {
  clear();
}

//---------------------------------------------------------------------------
int LzwTree::getInit() {
  return init;
}

//---------------------------------------------------------------------------
void LzwTree::setInit(int init) {
  this->init = init;
  ids = init + 2;
}

//---------------------------------------------------------------------------
int LzwTree::getNext() {
  return ids++;
}

//---------------------------------------------------------------------------
int LzwTree::getSize() {
  return ids;
}
