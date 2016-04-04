//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#include "mouseHome.hh"

MouseHome::MouseHome() : x(0), y(0), firstClick(true) {}

void MouseHome::click(int x, int y) {
  if (firstClick) {
    this->x = x;
    this->y = y;
    firstClick = false;
  }
}
