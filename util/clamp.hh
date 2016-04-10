//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

inline float clamp(float min, float max, float value) {
  if (value < min) 
    return min;
  else if (value > max)
    return max;
  else
    return value;
}
