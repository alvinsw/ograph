#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include "graph/IMutableGraph.h"
#include "Random.h"
#include "graph/DiGraphAdjMatrix.h"



struct Point {
    double x;
    double y;
};

struct Vehicle {
  uint32_t id;
  Point pos;
  double radioRange;
  double radioRangeSqr;
  float delayConstraint;
  float delay;
};
bool operator==(const Vehicle& lhs, const Vehicle& rhs) {
  return (lhs.id==rhs.id);
}
bool operator<(const Vehicle& lhs, const Vehicle& rhs) {
  return lhs.id < rhs.id;
};

typedef IVertex<Vehicle, double> TVertex;
typedef const IVertex<Vehicle, double>* TVertexPtr;
typedef const IEdge<Vehicle, double>* TEdgePtr;
typedef IEdge<Vehicle, double> TEdge;
typedef DiGraphAdjMatrix<Vehicle, double> TGraph;

const double MIN_X = 0;
const double MIN_Y = 0;
const double MAX_X = 1000;
const double MAX_Y = 1000;
const double minRadioRange = 200;
const double maxRadioRange = 300;
const double MIN_DIST = 50;

bool CheckMinDistance() {
}
// keep the vertices for easy access in a vector
template <typename T>
void GenerateNetworkGraph(IMutableGraph<Vehicle, double>& graph, std::vector< TVertexPtr >& vertices, 
                          uint32_t num, T minDistance, T minRange, T maxRange, T minX, T maxX, T minY, T maxY) {
  Random rnd;
  if (num > 0) { // create first node
      Vehicle v;
      v.id = 0;
      v.radioRange = minRange + rnd.NextInt(maxRange-minRange);
      v.radioRangeSqr = v.radioRange * v.radioRange;
      v.delayConstraint = 100 + rnd.NextInt(100);
      v.delay = 1 + rnd.NextInt(100);
      v.pos.x = rnd.NextDouble(minX, maxX);
      v.pos.y = rnd.NextDouble(minY, maxY);
      TVertexPtr vptr = graph.AddVertex(v);
      vertices.push_back(vptr);
  }
  for (uint32_t i=1; i<num; ++i) {
        Vehicle v;
        v.id = i;
        v.radioRange = minRange + rnd.NextInt(maxRange-minRange);
        v.radioRangeSqr = v.radioRange * v.radioRange;
        v.delayConstraint = 100 + rnd.NextInt(100);
        v.delay = 1 + rnd.NextInt(100);
        T minDistanceSqr = minDistance * minDistance;
        T x, y, m;
        bool minDistOk = false;
        do {
          int vid = rnd.NextInt(i); // pick existing node
          //find min max pos
          T xp = vertices[vid]->GetValue().pos.x;
          T yp = vertices[vid]->GetValue().pos.y;
          T rp = vertices[vid]->GetValue().radioRange;
          T minx = std::max(xp-rp, minX);
          T maxx = std::min(xp+rp, maxX);
          T miny = std::max(yp-rp, minY);
          T maxy = std::min(yp+rp, maxY);
          x = rnd.NextDouble(minx, maxx);
          y = rnd.NextDouble(miny, maxy);
          T dx = x - xp;
          T dy = y - yp;
          m = dx*dx + dy*dy;
          if (m > v.radioRangeSqr) continue;
          minDistOk = true;
          for (uint32_t j = 0; j < vertices.size(); ++j) {
            T x2 = vertices[j]->GetValue().pos.x;
            T y2 = vertices[j]->GetValue().pos.y;
            T dx2 = x - x2;
            T dy2 = y - y2;
            T d = dx2*dx2 + dy2*dy2;
            if (d < minDistanceSqr) minDistOk = false;
          }
        } while (!minDistOk || m > v.radioRangeSqr);
        v.pos.x = x;
        v.pos.y = y;
        TVertexPtr vptr = graph.AddVertex(v);
        vertices.push_back(vptr);
    }
    for (uint32_t i=0; i<num; ++i) {
        for (uint32_t j=i+1; j<num; ++j) {
            const TVertex& v1 = *vertices[i];
            const TVertex& v2 = *vertices[j];

            double dx = v1.GetValue().pos.x - v2.GetValue().pos.x;
            double dy = v1.GetValue().pos.y - v2.GetValue().pos.y;
            long distSqr = dx*dx + dy*dy; //square distance

            if (v1.GetValue().radioRangeSqr > distSqr) {
                graph.AddEdge(v1, v2, distSqr);
            }
            if (v2.GetValue().radioRangeSqr > distSqr) {
                graph.AddEdge(v2, v1, distSqr);
            }
        }
    }
}

#endif
