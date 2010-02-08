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

const double MAX_X = 500;
const double MAX_Y = 500;
const double minRadioRange = 300;
const double maxRadioRange = 500;

// keep the vertices for easy access in a vector
void GenerateNetworkGraph(IMutableGraph<Vehicle, double>& graph, std::vector< TVertexPtr >& vertices, uint32_t num) {
  Random rnd;
  for (uint32_t i=0; i<num; ++i) {
        Vehicle v;
        v.id = i;
        v.pos.x = rnd.NextDouble(0, MAX_X);
        v.pos.y = rnd.NextDouble(0, MAX_Y);
        v.radioRangeSqr = minRadioRange + rnd.NextInt(maxRadioRange-minRadioRange);
        v.radioRangeSqr = v.radioRangeSqr * v.radioRangeSqr;
        v.delayConstraint = 100 + rnd.NextInt(100);
        v.delay = 1 + rnd.NextInt(100);
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
