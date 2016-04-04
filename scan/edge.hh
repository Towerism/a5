//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <iostream>

#include "util/vector2.hh"

struct Edge {
  Vector2 start;
  Vector2 end;
  int maxY;
  float currentX, xIncr;
};

template <typename iterable>
void printEdges(iterable edges) {
  std::cout << "BEGIN EDGES" << std::endl;
  for (auto edge : edges) {
    std::cout << "  BEGIN EDGE" << std::endl;
    std::cout << "    START: ( " << edge.start.x << ", " << edge.start.y << " )" << std::endl;
    std::cout << "    END: ( " << edge.end.x << ", " << edge.end.y << " )" << std::endl;
    std::cout << "    MAXY: " << edge.maxY << std::endl;
    std::cout << "    CURRENTX: " << edge.currentX << std::endl;
    std::cout << "    XINCR: " << edge.xIncr << std::endl;
    std::cout << "  END EDGE" << std::endl;
  }
  std::cout << "END EDGES" << std::endl;
}
