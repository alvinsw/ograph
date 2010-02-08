#include <iostream>
#include <string>
#include "DijkstraShortestPath.h"
#include "DiGraphAdjMatrix.h"
#include "Marker.h"

// class intedgetodouble : public IMarker<IEdge<string, int>, double> {
//   double Get(const IEdge<string, int>& key) const {
//     return double(key.GetValue());
//   }
//   void Set(const IEdge<string, int>& key, const double& value) {}
// };
// intedgetodouble marker;

int main() {
  std::cout << "start" << std::endl;
  typedef DiGraphAdjMatrix<std::string, int> Graph;
  Graph g;
  Graph::VertexPtr s = g.AddVertex("s");
  Graph::VertexPtr v1 = g.AddVertex("v1");
  Graph::VertexPtr v2 = g.AddVertex("v2");
  Graph::VertexPtr v3 = g.AddVertex("v3");
  Graph::VertexPtr v4 = g.AddVertex("v4");
  Graph::VertexPtr v5 = g.AddVertex("v5");
  g.AddEdge(*s, *v1, 1);
  g.AddEdge(*v1, *v2, 1);
  g.AddEdge(*v2, *v3, 1);
  g.AddEdge(*v3, *v4, 1);
  g.AddEdge(*s, *v4, 2);
  g.AddEdge(*s, *v5, 3);
  g.AddEdge(*v1, *v5, 1);
  //g.Debug();
  DijkstraShortestPath<std::string, int, int> dsp(g);
  dsp.Run(*s);
  std::cout << "" << std::endl;
  TreeSet<Graph::VertexPtr> set;
  set.Add(v3);
  set.Add(v2);
  set.Add(v1);
  Iterator<Graph::VertexPtr> iter = set.GetIterator();
  while (iter.HasNext()) {
    std::cout << "set:" << iter.Next()->GetValue() << std::endl;
  }
  
  TreeSet<int> set1;
  set1.Add(3);
  set1.Add(2);
  set1.Add(1);
  Iterator<int> iter1 = set1.GetIterator();
  while (iter1.HasNext()) {
    std::cout << "set1:" << iter1.Next() << std::endl;
  }

  dsp.Run(*s, set);
  
  
  // use hashtable mapper for the vertex marker
//   HashtableMarker<Graph::Vertex*, double>::Parameters param1();
//   FibHeapPQ<Graph::Vertex*, double>::Parameters param2(g.VerticesSize(), g.GetVertexIndexer());
//   DijkstraShortestPath<string, int, double, HashtableMarker<Graph::Vertex*, double>, FibHeapPQ<Graph::Vertex*, double> > dsp(g, marker, param1, param2);
  std::cout << "finish" << std::endl;
  return 0;
}