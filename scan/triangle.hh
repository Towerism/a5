//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

struct vertex {
  float x,y,z;		// x, y, z coordinates
  float nx,ny,nz;		// Normal at the vertex
  float u,v;			// Texture coordinates
};

struct triangle {
  int whichtexture;	// The index number of the corresponding texture to apply
  // Note: Use the color returned by the texture for the
  // ambient, diffuse, and specular color, scaled by the
  // coefficients of ambient, diffuse, and specular reflection
  vertex v[3];		// The three vertices
  float kamb;			// The coefficient of ambient reflection
  float kdiff;		// The coefficient of diffuse reflection
  float kspec;		// The coefficient of specular reflection
  int shininess;		// The exponent to use for Specular Phong Illumination
};
