#include <QtGui/QApplication>
#include "mainwindow.h"
#include "graph/DiGraphAdjMatrix.h"
#include "Random.h"
#include <vector>
#include <string>
#include <map>
#include <stdint.h>

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

typedef IVertex<Vehicle, double> Vertex;
typedef const IVertex<Vehicle, double>* VertexPtr;
typedef const IEdge<Vehicle, double>* EdgePtr;
typedef IEdge<Vehicle, double> Edge;

const double MAX_X = 1000;
const double MAX_Y = 1000;

// keep the vertices for easy access in a vector
void GenerateNetworkGraph(IMutableGraph<Vehicle, double>& graph, std::vector< VertexPtr >& vertices, uint32_t num) {
  Random rnd;
  for (int i=0; i<num; ++i) {
        Vehicle v;
        v.pos.x = rnd.NextDouble(0, MAX_X);
        v.pos.y = rnd.NextDouble(0, MAX_Y);
        v.radioRangeSqr = 250 + rnd.NextInt(100);
        v.radioRangeSqr = v.radioRangeSqr * v.radioRangeSqr;
        v.delayConstraint = 100 + rnd.NextInt(100);
        v.delay = 1 + rnd.NextInt(100);
        VertexPtr vptr = graph.AddVertex(v);
        vertices.push_back(vptr);
    }
    for (int i=0; i<num; ++i) {
        for (int j=i+1; j<num; ++j) {
            const Vertex& v1 = *vertices[i];
            const Vertex& v2 = *vertices[j];

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


int main(int argc, char** argv)
{
    static const int count = 10;
    typedef DiGraphAdjMatrix<int, int> graph;
    graph g(count, -1);
    graph::VertexPtr vps[count];
    for (int i=0; i < count; ++i) {
      vps[i] = g.AddVertex(i);
    }

    for (int i=0; i<count; ++i) {
      for (int j=0; j<count; ++j) {
        if (i!=j) graph::EdgePtr e = g.AddEdge(*vps[i], *vps[j], 1);
      }
    }

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
    //return 0;
}
