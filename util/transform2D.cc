//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#include "transform2D.hh"

namespace Transform2D {

// translate current matrix to (x, y)
void translate(Vector2 translation) {
  glTranslatef(translation.x, translation.y, 0);
}

// rotate current matrix about (x,y) by spin degrees
void rotate(float spin, Vector2 origin) {
  translate(-origin);
  glRotatef(spin, 0, 0, 1);
  translate(origin);
}

}
