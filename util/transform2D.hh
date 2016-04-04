//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <GL/glut.h>

#include "vector2.hh"

namespace Transform2D {

// translate current matrix by a translation vector2
void translate(Vector2 translation);

// rotate current matrix about an origin by spin degrees
void rotate(float spin, Vector2 origin);

}
