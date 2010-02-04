#include <cassert>
#include <iostream>
#include "DiGraphAdjMatrix.h"

using namespace std;
class Node {
  public:
    Node() : id(0) {}
    explicit Node(int id_) : id(id_) {}
    int id;
    
};
class Arc {
  public:
    Arc() : id(0) {}
    explicit Arc(int id_) : id(id_) {}
    int id;
};
int main() {
  cout << "start" << endl;
  static const int count = 10;
  typedef DiGraphAdjMatrix<int, int> Graph;
  Graph g(count);
  g.SetNoEdgeValue(-1);
  assert( g.GetCapacity()==count );
  assert( g.NoEdge()==-1 );
  
  // test add
  Graph::VertexPtr vr[count];
  cout << "test add vertex" << endl;
  for (int i=0; i<count; ++i) {
    vr[i] = g.AddVertex(i);
    assert( vr[i]->GetValue() == i );
  }
  cout << "test g.VerticesSize()" << endl;
  assert( g.VerticesSize()==count );
  cout << "test g.GetVertices()" << endl;
  {
    Graph::VertexIterator vi = g.GetVertices(); int i = 0;
    while (vi.HasNext()) {
      assert( vr[i] == vi.Next() );
      ++i;
    }
  }
  cout << "test g.AddEdge" << endl;
  for (int i=0; i<count; ++i) {
    for (int j=0; j<count; ++j) {
      if (i!=j) {
        Graph::EdgePtr e = g.AddEdge(*vr[i], *vr[j], i*j);
        //cout << "test g.AddEdge: " << *e << endl;
        assert( e->GetValue() == i*j );
      }
    }
  }
  cout << "test g.EdgesSize" << endl;
  assert( g.EdgesSize() == count*count-count );

  cout << "test g.GetEdges" << endl;
  {
    Graph::EdgeIterator ei = g.GetEdges(); int i = 0;
    while (ei.HasNext()) {
      const Graph::EdgePtr eptr = ei.Next();
      assert( eptr->GetOwner() == &g );
      ++i;
    }
    assert( i == g.EdgesSize() );
  }
  
  cout << "test g.GetOutEdges" << endl;
  {
    Graph::EdgeIterator ei = g.GetOutEdges(*vr[0]); int i = 0;
    while (ei.HasNext()) {
      const Graph::EdgePtr eptr = ei.Next();
      assert( eptr->GetOwner() == &g );
      assert( eptr->GetSourceVertex() == vr[0] );
      cout << "test: " << *eptr << endl;
      ++i;
    }
    assert( i == g.OutDegree(*vr[0]) );
  }
  
  cout << "test g.GetInEdges" << endl;
  {
    Graph::EdgeIterator ei = g.GetInEdges(*vr[0]); int i = 0;
    while (ei.HasNext()) {
      const Graph::EdgePtr eptr = ei.Next();
      assert( eptr->GetOwner() == &g );
      assert( eptr->GetTargetVertex() == vr[0] );
      cout << "test: " << *eptr << endl;
      ++i;
    }
    assert( i == g.InDegree(*vr[0]) );
  }
  
  cout << "finish" << endl;
  return 0;
}