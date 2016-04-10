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
