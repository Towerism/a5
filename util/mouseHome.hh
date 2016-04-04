//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

// Keeps track of the position of the first mouse click
class MouseHome {
public:
  MouseHome();

  // register a click, if it is the first click
  // update x and y
  void click(int x, int y);

  int getX() const { return x; }
  int getY() const { return y; }

private:
  int x, y;
  bool firstClick;
};
