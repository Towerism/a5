//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#include "tween.hh"

void Tween::oneShotLinear() {
  if (!complete)
    linearContinue();
}

void Tween::linearContinue() {
  if (current < target)
    linearUp();
  else if (current > target)
    linearDown();
}

void Tween::linearUp() {
  current += increment;
  checkComplete(true);
}

void Tween::linearDown() {
  current -= increment;
  checkComplete(false);
}

void Tween::checkComplete(bool linearUp) {
  if (linearUp) {
    if (current > target)
      complete = true;
  } else if (current < target)
    complete = true;
}

void Tween::setTarget(float newTarget, float newIncrement) {
  target = newTarget;
  increment = newIncrement;
}

void Tween::reset() {
  complete = false;
}
