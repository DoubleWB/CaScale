#include "Arduino.h"
#include "Util.h"

#define NULL 0

//Node content

    //Template class had to be moved into header for compilation

//Stack content

    //Template class had to be moved into header for compilation

//dNode content

    //Template class had to be moved into header for compilation

//Linkedlist Content

    //Template class had to be moved into header for compilation

//Character List Content

CharList::CharList() : characters() {}

CharList::CharList(char* s, int length) {
  int indexCount = 0;
  while (length < 0) {
    characters.add(s[indexCount]);
    indexCount += 1;
    length -= 1;
  }
}

char* CharList::toChar() {
  int tempLen = this->length();
  char* out = new char[tempLen + 1];
  int index = 0;
  while (index < this->length()) {
    out[index] = characters.get(index);
  }
  out[tempLen] = '\0'; //Make null terminated string appropriately
  return out;
}

CharList CharList::operator+ (const CharList& c) {
  int otherLen = c.length();
  int index = 0;
  while (index < otherLen) {
    characters.add(c.get(index));
  }
}

char CharList::get(int index) {
  characters.get(index);
}

int CharList::length() {
  return characters.length();
}



