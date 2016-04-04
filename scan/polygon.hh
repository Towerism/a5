//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <vector>

#include "color.hh"
#include "util/vector2.hh"

struct Polygon {
  Polygon(std::vector<Vector2> points, Color color)
    : points(points), color(color) { }

  std::vector<Vector2> points;
  std::vector<Vector2> clippedPoints;
  Color color;
};

bool operator==(Polygon lhs, Polygon rhs) {
  return lhs.points == rhs.points && lhs.color == rhs.color;
}
