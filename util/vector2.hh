//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <cmath>

struct Vector2 {
  int x, y;
};

inline Vector2 operator-(Vector2 vector2) {
  return { -vector2.x, -vector2.y };
}

inline bool operator==(Vector2 lhs, Vector2 rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline Vector2 operator-(Vector2 lhs, Vector2 rhs) {
  return { lhs.x - rhs.x, lhs.y - rhs.y };
}

inline void operator -=(Vector2& lhs, Vector2 rhs) {
  lhs = lhs - rhs;
}

inline void translate(Vector2& vector, int offset) {
  vector.x += offset;
}

inline float magnitude(Vector2 vector) {
  return sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline void rotate(Vector2& vector, float cos, float sin) {
  float xtemp = vector.x * cos + vector.y * sin;
  vector.y = vector.y * cos - vector.x * sin;
  vector.x = xtemp;
}

inline Vector2 intersect(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
  float  mag, cos, sin, relativeIntersect;
  Vector2 intersect = p1;
  // translate coordinate plane so that p2 is a vector starting at the origin
  p2 -= p1;
  p3 -= p1;
  p4 -= p1;

  // calculate the magnitude of p2
  // in order to rotate the coordinate plane so that p2 lies on the x axis
  mag = magnitude(p2);
  cos = p2.x / mag;
  sin = p2.y / mag;
  rotate(p3, cos, sin);
  rotate(p4, cos, sin);

  // calculate the intersect in the new coordinate plane and transform it into
  // the intersect in the old coordinate plane
  relativeIntersect = (p3.x - p4.x) * p4.y / (p4.y - p3.y) + p4.x;
  intersect.x = relativeIntersect * cos + p1.x;
  intersect.y = relativeIntersect * sin + p1.y;

  return intersect;

}
