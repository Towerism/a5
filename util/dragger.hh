//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

// Facilitates dragging logic
class Dragger {
public:
  Dragger(int& xBinding, int& yBinding)
    : startX(0), startY(0), endX(0), endY(0),
      xBinding(xBinding), yBinding(yBinding) {}

  // indicate where the drag is starting
  void start(int x, int y);

  // where it is ending
  void end(int x, int y);

  // execute the drag
  void operator()();

private:
  int startX, startY, endX, endY;
  int& xBinding; // the x value we are controlling
  int& yBinding;
};
