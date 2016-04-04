//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <algorithm>
#include <list>
#include <vector>

#include "edge.hh"

struct ActiveEdgeList {
  ActiveEdgeList(int startingY) : currentY(startingY - 1) {}

  void add(std::list<Edge> newedges) {
    ++currentY;
    if (!newedges.empty())
      edges.insert(edges.end(), newedges.begin(), newedges.end());
    prune();
    incrementX();
    sort();
  }

  void print() const {
    printEdges(edges);
  }

  auto begin() { return edges.begin(); }
  auto end() { return edges.end(); }
  Edge operator[](std::size_t i) { return edges[i]; }

  int getCurrentY() const { return currentY; }

  std::size_t size() const { return edges.size(); }

private:
  std::vector<Edge> edges;
  int currentY;

  void prune() {
    for (auto it = edges.begin(); it != edges.end();) {
      if (it->end.y == currentY || it->start.y == it->end.y)
        it = edges.erase(it);
      else
        ++it;
    }
  }

  void incrementX() {
    for (auto& edge : edges) {
      edge.currentX += edge.xIncr;
    }
  }

  void sort() {
    std::sort(edges.begin(), edges.end(),
              [](Edge a, Edge b) { return a.currentX < b.currentX; });
  }
};
