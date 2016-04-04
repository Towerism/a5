//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#include "dragger.hh"

void Dragger::start(int x, int y) {
  startX = x;
  startY = y;
}

void Dragger::end(int x, int y) {
  endX = x;
  endY = y;
}

void Dragger::operator()() {
  int deltaX = endX - startX;
  int deltaY = endY - startY;
  xBinding += deltaX;
  yBinding += -deltaY;
}
