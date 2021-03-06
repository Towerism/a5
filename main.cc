//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#include "scan/activeEdgeList.hh"
#include "scan/activeEdgeTable.hh"
#include "scan/color.hh"
#include "scan/edge.hh"
#include "scan/polygon.hh"
#include "scan/triangle.hh"
#include "util/vector2.hh"

#include <algorithm>
#include <fstream>
#include <GL/glut.h>
#include <iostream>
#include <list>
#include <limits>
#include <math.h>
#include <random>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/******************************************************************
        Notes:	Image size is 400 by 400.
        This is a LEFT handed coordinate system.  That is, x is in the
            horizontal direction starting from the left, y is the
                vertical direction starting from the bottom, and z is pointing
                INTO the screen (so bigger z values mean farther away).  Think
                of it as inverting the z-axis if that helps.
        Your view vector is ALWAYS [0 0 1] (i.e. you are infinitely far away,
                looking in the POSITIVE Z direction).  So, from any point on
                a triangle, the direction vector to your eye is [0 0 -1]
        This file already contains code to load in data (triangles,
                lights, textures).  You just need to access the data stored
                in the trianglelist, lightlist, and texturelist.  Some
                other helpful routines are also included.
        Call setFramebuffer to set a pixel.  This should be the only
                routine you use to set the color.  Use the getTextureRGB to
                get a texture value.  drawit() will cause the current
                framebuffer to be displayed.
        You can create separate routines, global variables, etc. as
                necessary.  You'll probably want to define a global Z buffer.
*****************************************************************/

#define ImageW 400
#define ImageH 400

float framebuffer[ImageH][ImageW][3];
float ZMAX = 10000.0;	// NOTE: Assume no point has a Z value greater than 10000.0
float zbuffer[ImageH][ImageW];

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0, 1.0);

std::string sourcefile="triangle.dat";

struct color {
  float r, g, b;
};

struct light {
  // Note: assume all lights are white
  float x,y,z;		// x, y, z coordinates of light
  color brightness;	// Level of brightness of light (0.0 - 1.0)
};

struct texture {
  // Note access using getTextureRGB provided below
  int xsize, ysize;	// The size of the texture in x and y
  float* elements;	// RGB values
};


int numtriangles;		// The number of triangles in the scene
int numlights;			// The number of lights (not including ambient) in the scene
int numtextures;		// The number of textures used in the scene

color ambientlight;		// The coefficient of ambient light

triangle* trianglelist;	// Array of triangles
light* lightlist;		// Array of lights
texture* texturelist;	// Array of textures

/* Pass in a pointer to the texture, t, and the texture coordinates, u and v
   Returns (in R,G,B) the color of the texture at those coordinates */
void getTextureRGB(texture* t, float u, float v, float& R, float& G, float& B) {
  int xval,yval;
  if (u<1.0) 
    if (u>=0.0) xval = (int)(u*t->xsize);
    else xval = 0;
  else xval = t->xsize-1;
  if (v<1.0) 
    if (v>=0.0) yval = (int)(v*t->ysize);
    else yval = 0;
  else yval = t->ysize-1;
        
  R = t->elements[3*(xval*t->ysize+yval)];
  G = t->elements[(3*(xval*t->ysize+yval))+1];
  B = t->elements[(3*(xval*t->ysize+yval))+2];
}


// Draws the scene
void drawit(void)
{
  glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);
  glFlush();
}

float clampFloat(float f) {
  if (f < 0.0)
    return 0.0;
  if (f > 1.0)
    return 1.0;
  return f;
}

// make sure color values are between 0 and 1
Color clampColorValues(Color color) {
  return { clampFloat(color.red()), clampFloat(color.green()), clampFloat(color.blue()) };
}

// Sets pixel x, y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly - Scott Schaefer 
// Made the function less ugly - Martin Fracker

// repositioning the origin is inappropriate for the triangle data given
// for this assignment
void repositionOrigin(Vector2& position) {
  // position.y = ImageH - 1 - position.y;
}

void setFramebuffer(Vector2 position, Color color) {
  // changes the origin from the lower-left corner to the upper-left corner
  repositionOrigin(position);
  framebuffer[position.y][position.x][0] = color.red();
  framebuffer[position.y][position.x][1] = color.green();
  framebuffer[position.y][position.x][2] = color.blue();
}

void setZbuffer(Vector2 position, float depth) {
  repositionOrigin(position);
  zbuffer[position.y][position.x] = depth;
}

int getDepth(Vector2 position) {
  repositionOrigin(position);
  return zbuffer[position.y][position.x];
}

Color calculateAndApplyIntensity(triangle tri, Vector3 pixel, Vector3 normal, Vector3 eye, Color color) {
  Color result = color;
  Vector3 intensity;

  Vector3 ambient = { ambientlight.r, ambientlight.g, ambientlight.b };
  Vector3 diffuse;
  Vector3 specular;

  Vector3 lightbrightness;
  float lightcos;

  Vector3 light;
  Vector3 reflect;
  float reflectcos;

  ambient *= tri.kamb;
  intensity += ambient;

  eye = normalize(eye - pixel);
  normal = normalize(normal);

  for (int i = 0; i < numlights; ++i) {
    light = { lightlist[i].x, lightlist[i].y, lightlist[i].z };
    light = normalize(light - pixel);
    lightbrightness = { lightlist[i].brightness.r, lightlist[i].brightness.g, lightlist[i].brightness.b };
    diffuse = lightbrightness;
    specular = lightbrightness;
    lightcos = fmax(0, dot(light, normal));
    reflect = normalize(2 * lightcos * normal - light);
    reflectcos = fmax(0, dot(reflect, eye));

    diffuse *= tri.kdiff * lightcos;
    specular *= tri.kspec * pow(reflectcos, tri.shininess);
    if (lightcos == 0)
      specular = 0;

    intensity += diffuse + specular;
  }


  result.set_intensity(intensity);
  return result;
}

Color calculateAndApplyTextureUVs(triangle tri, Vector3 uv) {
  float x, y, z;
  getTextureRGB(texturelist + tri.whichtexture, uv.x, uv.y, x, y, z);

  return { x, y, z };
}

void drawScanLine(int y, int startX, int endX, int startZ, Vector3 startUV, Vector3 endUV,
                  Vector3 surfaceNormal, Vector3 startNormal, Vector3 endNormal,
                  Vector3 eye, triangle tri) {
  Color color = { 0, 0, 0 };
  float z = startZ;
  float rangeX = endX - startX;
  float deltaZ = surfaceNormal.x / surfaceNormal.z;
  Vector3 rangeN = endNormal - startNormal;
  Vector3 rangeUV = endUV - startUV;
  Vector3 deltaN = rangeN / rangeX;
  Vector3 deltaUV = rangeUV / rangeX;
  Vector3 currentN = startNormal;
  Vector3 currentUV = startUV;
  Vector3 pixel = { 0, 0, 0 };
  for (int x = startX; x < endX; ++x) {
    pixel = { (float)x, (float)y, (float)z };
    if (z < getDepth({x, y})) {
      setZbuffer({x, y}, z);
      color = calculateAndApplyTextureUVs(tri, currentUV);
      color = calculateAndApplyIntensity(tri, pixel, currentN, eye, color);
      setFramebuffer({x, y}, color);
    }
    if (rangeX != 0) {
      currentN += deltaN;
      currentUV += deltaUV;
    }
    if (surfaceNormal.z != 0) {
      z -= deltaZ;
    }
  }
}

void scanfill(triangle tri) {
  std::list<Edge> edges = makeEdges(tri);
  ActiveEdgeTable edgeTable = makeActiveEdgeTable(edges);
  ActiveEdgeList edgeList(findMinYFromEdges(edges));
  Vector3 normal = calculateNormal(edges, tri);
  for (auto list : edgeTable) {
    edgeList.add(list);
    for (std::size_t i = 0; i < edgeList.size(); i += 2) {
      drawScanLine(edgeList.getCurrentY(),
                   edgeList[i].currentX,
                   edgeList[i + 1].currentX,
                   edgeList[i].currentZ,
                   edgeList[i].currentUV,
                   edgeList[i + 1].currentUV,
                   normal,
                   edgeList[i].currentN,
                   edgeList[i + 1].currentN,
                   { (float)ImageW / 2, (float)ImageH / 2, -ZMAX },
                   tri);
    }
  }
}

// Normalizes the vector passed in
void normalize(float& x, float& y, float& z) {
  float temp = sqrt(x*x+y*y+z*z);
  if (temp > 0.0) {
    x /= temp;
    y /= temp;
    z /= temp;
  } else {
    x = 0.0;
    y = 0.0;
    z = 0.0;
  }
}

// Returns dot product of two vectors
float dot(float x1, float y1, float z1, float x2, float y2, float z2) {
  return (x1*x2+y1*y2+z1*z2);
}

// Returns angle between two vectors (in radians)
float angle(float x1, float y1, float z1, float x2, float y2, float z2) {
  normalize(x1,y1,z1);
  normalize(x2,y2,z2);
  return  acos(dot(x1,y1,z1,x2,y2,z2));
}

void display(void)
{
  for (int i = 0; i < numtriangles; ++i) {
    scanfill(trianglelist[i]);
  }

  drawit();
}

void init(void)
{
  int i,j,k;

  // Initialize framebuffer to clear
  for(i=0;i<ImageH;i++) {
    for (j=0;j<ImageW;j++) {
      framebuffer[i][j][0] = 0.0;
      framebuffer[i][j][1] = 0.0;
      framebuffer[i][j][2] = 0.0;
      zbuffer[i][j] = ZMAX;
    }
  }

  // Load in data
  ifstream infile(sourcefile);
  if (!infile) {
    cout << "Error! Input file " << sourcefile << " does not exist!" << endl;
    exit(-1);
  }
  infile >> numtriangles >> numlights >> numtextures;
        
  // First read triangles
  trianglelist = new triangle[numtriangles];
  for(i=0;i<numtriangles;i++) {
    infile >> trianglelist[i].whichtexture;
    infile >> trianglelist[i].kamb >> trianglelist[i].kdiff >> trianglelist[i].kspec;
    infile >> trianglelist[i].shininess;
    for(j=0;j<3;j++) {
      infile >> trianglelist[i].v[j].x >> trianglelist[i].v[j].y >> trianglelist[i].v[j].z;
      infile >> trianglelist[i].v[j].nx >> trianglelist[i].v[j].ny >> trianglelist[i].v[j].nz;
      infile >> trianglelist[i].v[j].u >> trianglelist[i].v[j].v;
    }
  }

  // Now read lights
  lightlist = new light[numlights];
  infile >> ambientlight.r >> ambientlight.g >> ambientlight.b;
  for(i=0;i<numlights;i++) {
    infile >> lightlist[i].x >> lightlist[i].y >> lightlist[i].z;
    infile >> lightlist[i].brightness.r >> lightlist[i].brightness.g >> lightlist[i].brightness.b;
  
}

  // Now read textures
  texturelist = new texture[numtextures];
  for(i=0;i<numtextures;i++) {
    infile >> texturelist[i].xsize >> texturelist[i].ysize;
    texturelist[i].elements = new float[texturelist[i].xsize*texturelist[i].ysize*3];
    for(j=0;j<texturelist[i].xsize;j++) {
      for (k=0;k<texturelist[i].ysize;k++) {
        infile >> texturelist[i].elements[3*(j*texturelist[i].ysize+k)];
        infile >> texturelist[i].elements[3*(j*texturelist[i].ysize+k)+1];
        infile >> texturelist[i].elements[3*(j*texturelist[i].ysize+k)+2];
      }
    }
  }

  infile.close();
}

int main(int argc, char** argv)
{
  if (argc >= 2) {
    sourcefile = std::string(argv[1]);
  }
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(ImageW,ImageH);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Martin Fracker - Assignment 5");
  init();	
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
