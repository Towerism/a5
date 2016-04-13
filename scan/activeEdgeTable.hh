//  Copyright 2016 Martin Fracker, Jr.
//  All Rights Reserved.
// 
//  This project is free software, released under the terms
//  of the GNU General Public License v3. Please see the
//  file LICENSE in the root directory or visit
//  www.gnu.org/licenses/gpl-3.0.en.html for license terms.

#pragma once

#include <cassert>
#include <cmath>
#include <vector>
#include <list>

#include "color.hh"
#include "edge.hh"
#include "triangle.hh"
#include "util/vector2.hh"
#include "util/vector3.hh"

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
  if (deltaY != 0)
    edge.xIncr = deltaX / deltaY;
  else
    edge.xIncr = 0;
}

inline Vector3 calculateNormal(std::list<Edge> edges, triangle tri) {
  vertex v1 = tri.v[0];
  vertex v2 = tri.v[1];
  vertex v3 = tri.v[2];
  Vector3 vertex1 = { v1.x, v1.y, v1.z };
  Vector3 vertex2 = { v2.x, v2.y, v2.z };
  Vector3 vertex3 = { v3.x, v3.y, v3.z };

  Vector3 u = vertex3 - vertex1;
  Vector3 v = vertex2 - vertex1;
  Vector3 normal = cross(u, v);

  return normalize(normal);
}

inline void setEndPoint(Edge& edge, Vector3 point) {
  edge.end = point;
  makeStartBelowEnd(edge);
  calculateXIncr(edge);
}

inline void calculateZIncr(Edge& edge, Vector3 normal) {
  if (normal.z != 0)
    edge.zIncr = -normal.y / normal.z;
  else
    edge.zIncr = 0;
}

inline void applyNormal(std::list<Edge>& edges, triangle tri) {
  Vector3 normal = calculateNormal(edges, tri);
  for (auto& edge : edges) {
    calculateZIncr(edge, normal);
    edge.currentZ = edge.start.z;
  }
}

inline void setupNormalInterpolation(std::list<Edge>& edges, triangle tri) {
  Vector3 start;
  Vector3 end;
  Vector3 startNormal;
  Vector3 endNormal;
  float deltaY;
  for (auto& edge : edges) {
    start = edge.start;
    end = edge.end;
    startNormal = getVertexNormFromTriangle(tri, start);
    endNormal = getVertexNormFromTriangle(tri, end);
    edge.currentN = startNormal;
    deltaY = end.y - start.y;
    if (deltaY != 0)
      edge.deltaN = (endNormal - startNormal) / deltaY;
    else
      edge.deltaN = { 0, 0, 0 };
  }
}

inline void setupUVInterpolation(std::list<Edge>& edges, triangle tri) {
  Vector3 start;
  Vector3 end;
  Vector3 uvStart;
  Vector3 uvEnd;
  float deltaY;
  for (auto& edge : edges) {
    start = edge.start;
    end = edge.end;
    uvStart = getVertexUVsFromTriangle(tri, start);
    uvEnd = getVertexUVsFromTriangle(tri, end);
    edge.currentUV = uvStart;
    deltaY = end.y - start.y;
    if (deltaY != 0)
      edge.deltaUV = (uvEnd - uvStart) / deltaY;
    else
      edge.deltaUV = { 0, 0, 0 };
  }
}

inline std::list<Edge> makeEdges(triangle tri) {
  std::list<Edge> edges;
  std::vector<Vector3> points = { getTriangleVertex(tri, 0),
                                  getTriangleVertex(tri, 1),
                                  getTriangleVertex(tri, 2) };
  for (std::size_t i = 0; i < points.size(); ++i) {
    edges.push_back({ points[i] });
    if (i + 1 != points.size()) {
      setEndPoint(edges.back(), points[i + 1]);
    }
  }
  setEndPoint(edges.back(), points.front());
  applyNormal(edges, tri);
  setupNormalInterpolation(edges, tri);
  setupUVInterpolation(edges, tri);
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
