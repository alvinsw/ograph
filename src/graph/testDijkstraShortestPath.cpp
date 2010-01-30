#include <iostream>
#include <string>
#include "DijkstraShortestPath.h"
#include "DiGraphAdjMatrix.h"
#include "Marker.h"

using namespace std;
// class intedgetodouble : public IMarker<IEdge<string, int>, double> {
//   double Get(const IEdge<string, int>& key) const {
//     return double(key.GetValue());
//   }
//   void Set(const IEdge<string, int>& key, const double& value) {}
// };
// intedgetodouble marker;

int main() {
  cout << "start" << endl;
  typedef DiGraphAdjMatrix<string, int> Graph;
  Graph g;
  Graph::VertexPtr s = g.AddVertex("A");
  Graph::VertexPtr v1 = g.AddVertex("B");
  Graph::VertexPtr v2 = g.AddVertex("C");
  Graph::VertexPtr v3 = g.AddVertex("D");
  Graph::VertexPtr v4 = g.AddVertex("E");
  g.AddEdge(*s, *v1, 1);
  g.AddEdge(*v1, *v2, 1);
  g.AddEdge(*v2, *v3, 1);
  g.AddEdge(*v3, *v4, 1);
  g.AddEdge(*s, *v4, 2);
  //g.Debug();
  DijkstraShortestPath<string, int, int> dsp(g);
  dsp.Run(*s);
  
  // use hashtable mapper for the vertex marker
//   HashtableMarker<Graph::Vertex*, double>::Parameters param1();
//   FibHeapPQ<Graph::Vertex*, double>::Parameters param2(g.VerticesSize(), g.GetVertexIndexer());
//   DijkstraShortestPath<string, int, double, HashtableMarker<Graph::Vertex*, double>, FibHeapPQ<Graph::Vertex*, double> > dsp(g, marker, param1, param2);
  cout << "finish" << endl;
  return 0;
}