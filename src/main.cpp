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

typedef const IVertex<Vehicle, double> Vertex;
typedef const IVertex<Vehicle, double>* VertexPtr;
typedef IEdge<Vehicle, double> Edge;

const double MAX_X = 1000;
const double MAX_Y = 1000;

// keep the vertices for easy access in a vector
void GenerateNetworkGraph(IMutableGraph<Vehicle, double>& graph, std::vector< Vertex* >& vehicles, uint32_t num) {
  Random rnd;
  for (int i=0; i<num; ++i) {
        Vehicle v;
        v.pos.x = rnd.NextDouble(0, MAX_X);
        v.pos.y = rnd.NextDouble(0, MAX_Y);
        v.radioRangeSqr = 250 + rnd.NextInt(100);
        v.radioRangeSqr = v.radioRangeSqr * v.radioRangeSqr;
        v.delayConstraint = 100 + rnd.NextInt(100);
        v.delay = 1 + rnd.NextInt(100);
	VertexPtr vptr = &graph.AddVertex(v);
        vehicles.push_back(vptr);
    }
    for (int i=0; i<num; ++i) {
        for (int j=i+1; j<num; ++j) {
            Vertex& v1 = *vehicles[i];
            Vertex& v2 = *vehicles[j];

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
    typedef DiGraphAdjMatrix<int, int> graph;
    graph g;
    for (int i=1; i < 10; ++i) {
      graph::VertexRef v = g.AddVertex(i);
      std::cout << "size: " << g.VerticesSize() << std::endl;
    }
    graph::VertexIterator vi = g.Vertices();
    graph::VertexRef v1 = vi.Next();
    graph::VertexIterator vi2 = g.Vertices();
    while (vi2.HasNext()) {
      graph::VertexRef v2 = vi2.Next();
      g.AddEdge(v1, v2, 9);
      std::cout << "v2:" << v2.GetValue() << std::endl;
    }
    graph::VertexIterator i = g.Vertices();
    vi = i;
    while (i.HasNext()) {
      std::cout << "test: " << i.Next().GetValue() << std::endl;
    }
    graph::EdgeIterator ei = g.Edges();
    while (ei.HasNext()) {
      graph::EdgeRef e = ei.Next();
      std::cout << "edge: " << e.GetSourceValue() << "->" << e.GetTargetValue() << ":" << e.GetValue() << std::endl;
    }

    g.Clear();
//     QApplication app(argc, argv);
//     MainWindow w;
//     w.show();
//     return app.exec();
    //std::map<int, std::string> m;
    return 0;
}
