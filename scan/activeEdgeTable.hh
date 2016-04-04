//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <vector>
#include <list>

#include "color.hh"
#include "edge.hh"

struct ActiveEdgeTable {
  ActiveEdgeTable(Vector2 rangeY)
    : edgeLists(rangeY.y - rangeY.x + 1), rangeY(rangeY) {
    currentEdgeList = edgeLists.end();
  }

  void add(Edge edge) {
    edgeLists[edge.start.y - rangeY.x].push_back(edge);
  }

  const std::list<Edge>& next() {
    if (currentEdgeList == edgeLists.cend()) {
      currentEdgeList = edgeLists.cbegin();
    } else {
      ++currentEdgeList;
    }
    return *currentEdgeList;
  }

  auto begin() { return edgeLists.begin(); }
  auto end() { return edgeLists.end(); }

private:
  std::vector<std::list<Edge>> edgeLists;
  std::vector<std::list<Edge>>::const_iterator currentEdgeList;

  Vector2 rangeY;
};

inline void makeStartBelowEnd(Edge& edge) {
  if (edge.start.y > edge.end.y)
    std::swap(edge.start, edge.end);
  edge.maxY = edge.end.y;
  edge.currentX = edge.start.x;
}

inline void calculateXIncr(Edge& edge) {
  float deltaY = edge.end.y - edge.start.y;
  float deltaX = edge.end.x - edge.start.x;
  edge.xIncr = deltaX / deltaY;
}

inline void setEndPoint(Edge& edge, Vector2 point) {
  edge.end = point;
  makeStartBelowEnd(edge);
  calculateXIncr(edge);
}

inline std::list<Edge> makeEdges(std::vector<Vector2> points) {
  std::list<Edge> edges;
  for (std::size_t i = 0; i < points.size(); ++i) {
    edges.push_back({ points[i] });
    if (i + 1 != points.size()) {
      setEndPoint(edges.back(), points[i + 1]);
    }
  }
  setEndPoint(edges.back(), points.front());
  return edges;
}

inline int findMaxYFromEdges(std::list<Edge> edges) {
  auto it = std::max_element(edges.begin(), edges.end(),
                             [](Edge a, Edge b) {
                               return a.maxY < b.maxY;
                             });
  return it->maxY;
}

inline int findMinYFromEdges(std::list<Edge> edges) {
  auto it = std::min_element(edges.begin(), edges.end(),
                             [](Edge a, Edge b) {
                               return a.start.y < b.start.y;
                             });
  return it->start.y;
}

inline ActiveEdgeTable makeActiveEdgeTable(std::list<Edge> edges) {
  int maxY = findMaxYFromEdges(edges);
  int minY = findMinYFromEdges(edges);
  ActiveEdgeTable table({minY, maxY});
  for (auto edge : edges) {
    table.add(edge);
  }
  return table;
}
