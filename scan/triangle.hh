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

inline Vector3 getTriangleVertex(triangle tri, std::size_t vertex) {
  return { tri.v[vertex].x, tri.v[vertex].y, tri.v[vertex].z };
}

inline Vector3 getVertexNormFromTriangle(triangle tri, Vector3 reference) {
  vertex v;
  for (int i = 0; i < 3; ++i) {
    v = tri.v[i];
    if (v.x == reference.x &&
        v.y == reference.y &&
        v.z == reference.z)
      return { v.nx, v.ny, v.nz };
  }
  return { 0, 0, 0 };
}

inline Vector3 getVertexUVsFromTriangle(triangle tri, Vector3 reference) {
  vertex v;
  for (int i = 0; i < 3; ++i) {
    v = tri.v[i];
    if (v.x == reference.x &&
        v.y == reference.y &&
        v.z == reference.z)
      return { v.u, v.v, 0 };
  }
  return { 0, 0, 0 };
}
