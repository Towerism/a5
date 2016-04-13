//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <cmath>

#include "util/clamp.hh"

struct Vector3 {
  float x, y, z;

  void operator=(float value) {
    x = value;
    y = value;
    z = value;
  }
};

inline Vector3 operator-(Vector3 vector3) {
  return { -vector3.x, -vector3.y , -vector3.z};
}

inline bool operator==(Vector3 lhs, Vector3 rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator==(Vector3 lhs, float rhs) {
  return lhs == Vector3{ rhs, rhs, rhs };
}

inline bool operator<=(Vector3 lhs, float rhs) {
  return lhs.x <= rhs && lhs.y <= rhs && lhs.z <= rhs;
}

inline Vector3 operator-(Vector3 lhs, Vector3 rhs) {
  return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

inline void operator-=(Vector3& lhs, Vector3 rhs) {
  lhs = lhs - rhs;
}

inline Vector3 operator+(Vector3 lhs, Vector3 rhs) {
  return lhs - (-rhs);
}

inline void operator+=(Vector3& lhs, Vector3 rhs) {
  lhs = lhs + rhs;
}

inline Vector3 operator*(Vector3 lhs, float rhs) {
  Vector3 result = lhs;
  result.x *= rhs;
  result.y *= rhs;
  result.z *= rhs;
  return result;
}

inline void operator*=(Vector3& lhs, float rhs) {
  lhs.x *= rhs;
  lhs.y *= rhs;
  lhs.z *= rhs;
}

inline Vector3 operator*(float rhs, Vector3 lhs) {
  return lhs * rhs;
}

inline Vector3 operator/(Vector3 lhs, float rhs) {
  return lhs * (1 / rhs);
}

inline void operator/=(Vector3& lhs, float rhs) {
  lhs.x /= rhs; 
  lhs.y /= rhs; 
  lhs.z /= rhs; 
}

inline float magnitude(Vector3 vector) {
  return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

inline float dot(Vector3 u, Vector3 v) {
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vector3 cross(Vector3 u, Vector3 v) {
  Vector3 result;

  result.x = u.y * v.z - u.z * v.y;
  result.y = u.z * v.x - u.x * v.z;
  result.z = u.x * v.y - u.y * v.x;

  return result;
}

inline Vector3 normalize(Vector3 vector) {
  float mag = magnitude(vector);
  Vector3 result = vector;
  if (mag > 0)
    result /= mag;
  else
    result = { 0, 0, 0 };
  return result;
}

inline Vector3 clamp(int min, int max, Vector3 vector) {
  Vector3 result = {
    clamp(min, max, vector.x),
    clamp(min, max, vector.y),
    clamp(min, max, vector.z)
  };
  return result;
}
