#include <iostream>
#include "DepthFirstIterator.h"
#include "DiGraphAdjMatrix.h"

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
  Graph::VertexPtr v6 = g.AddVertex("v6");
  Graph::VertexPtr v7 = g.AddVertex("v7");
  g.AddEdge(*s, *v1, 1);
  g.AddEdge(*s, *v2, 1);
  g.AddEdge(*s, *v3, 1);
  g.AddEdge(*v1, *v4, 1);
  g.AddEdge(*v1, *v5, 1);
  g.AddEdge(*v5, *v6, 1);
  g.AddEdge(*v6, *v7, 1);

  DepthFirstVertexIterator<std::string, int> dfi(g, *s);
  while (dfi.HasNext()) {
    std::cout << "v:" << dfi.Next()->GetValue() << std::endl;
  }
  
  return 0;
}