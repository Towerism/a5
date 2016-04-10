//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <iostream>
#include <vector>

#include "util/vector3.hh"
#include "util/clamp.hh"

class Color {
public:
  Color(std::initializer_list<float> rgb) : rgb(rgb), intensity{0, 0, 0} {}

  float red() {
    return rgb[0] * intensity.x;
  }
  float green() {
    return rgb[1] * intensity.y;
  }
  float blue() {
    return rgb[2] * intensity.z;
  }

  void set_intensity(Vector3 value) {
    intensity = clamp(0, 1, value);
  }

  void set_intensity(float value) {
    set_intensity(Vector3{ value, value, value });
  }

  void set_intensity(Color value) {
    set_intensity(Vector3{ value.red(), value.green(), value.blue()});
  }

  Color operator+(Color other) {
    Color result = {
      red() + other.red(),
      green() + other.green(),
      blue() + other.blue()
    };
    result.set_intensity(1);
    return result;
  }

private:
  std::vector<float> rgb;
  Vector3 intensity;
};

bool operator==(Color lhs, Color rhs) {
  return lhs.red() == rhs.red() && lhs.green() == rhs.green() && lhs.blue() == rhs.blue();
}
