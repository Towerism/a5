//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

class Tween {
public:
  Tween(float& current)
    : current(current), target(0.0), increment(0.0), complete(false) {}
  // Tweens a current value to a target value using increment over time
  // Once the tween is complete, current is set to 0
  void oneShotLinear();

  void setTarget(float newTarget, float newIncrement);
  void reset();

private:
  float& current;
  float target;
  float increment;
  bool complete;

  void checkComplete(bool linearUp);

  void linearContinue();
  void linearUp();
  void linearDown();
};
