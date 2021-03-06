//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <iostream>

#include "util/vector3.hh"

struct Edge {
  Vector3 start, end;
  int maxY;
  float currentX, xIncr, currentZ, zIncr;
  Vector3 currentN, deltaN;
  Vector3 currentUV, deltaUV;
};

template <typename iterable>
void printEdges(iterable edges) {
  std::cout << "BEGIN EDGES" << std::endl;
  for (auto edge : edges) {
    std::cout << "  BEGIN EDGE" << std::endl;
    std::cout << "    START: ( " << edge.start.x << ", " << edge.start.y <<  ", " << edge.start.z <<" )" << std::endl;
    std::cout << "    END: ( " << edge.end.x << ", " << edge.end.y << ", " << edge.end.z << " )" << std::endl;
    std::cout << "    MAXY: " << edge.maxY << std::endl;
    std::cout << "    CURRENTZ: " << edge.currentX << std::endl;
    std::cout << "    XINCR: " << edge.xIncr << std::endl;
    std::cout << "    CURRENTZ: " << edge.currentZ << std::endl;
    std::cout << "    ZINCR: " << edge.zIncr << std::endl;
    std::cout << "    CURRENTN: ( " << edge.currentN.x << ", " << edge.currentN.y <<  ", " << edge.currentN.z <<" )" << std::endl;
    std::cout << "    DELTAN: ( " << edge.deltaN.x << ", " << edge.deltaN.y << ", " << edge.deltaN.z << " )" << std::endl;
    std::cout << "  END EDGE" << std::endl;
  }
  std::cout << "END EDGES" << std::endl;
}
