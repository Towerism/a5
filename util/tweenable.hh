//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include "tween/tween.hh"

#include <limits>

// Encapsulates a float value that can
// linearly tween itself by a given delta
class Tweenable {
public:
  Tweenable(float base, float bias, float minimum = -std::numeric_limits<float>::max())
    : base(base), bias(bias), minimum(minimum), value(base), tweener(value) {}

  void tween(float delta, float increment);
  void reset();
  float getValue() const { return value; }

  operator float() { return getValue(); }

private:
  float base, bias, minimum, value;
  Tween tweener;

  void clampToMinimum();
};

