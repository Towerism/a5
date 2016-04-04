//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#include "tweenable.hh"

void Tweenable::tween(float delta, float increment) {
  float biasAdjustedDelta = delta / bias;
  tweener.setTarget(base + biasAdjustedDelta, increment);
  tweener.oneShotLinear();
  clampToMinimum();
}

void Tweenable::clampToMinimum() {
  if (value < minimum)
    value = minimum;
}

void Tweenable::reset() {
  value = base;
  tweener.reset();
}
