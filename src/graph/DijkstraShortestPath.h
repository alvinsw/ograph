/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef DIJKSTRASHORTESTPATH_H
#define DIJKSTRASHORTESTPATH_H

#include <limits>

#include "Marker.h"
#include "IGraph.h"
#include "IndexedArrayPriorityQueue.h"
#include <iostream>
#include <cassert>

template <typename T>
class IVisitor {
  virtual void operator()(const T& t) const = 0;
};

/**
 * W is the type of weight, must be number (int, long, double, etc)
 * DijkstraShortestPath will fins a path with the least weight W
 */
template <typename V, typename E, typename W>
//          typename TVertexMarker = IndexedArrayMarker , 
//          template <typename Y, typename X> class TVertexMarker = IndexedArrayMarker, 
//          typename TPriorityQueue = IndexedArrayPriorityQueue<const IVertex<V, E>*, Comparator> >
class DijkstraShortestPath {
  //typedef typename TPriorityQueue::instanceOfPriorityQueue PriorityQueue_extends_IPriorityQueue;
  //typedef typename VT::instanceOfTransformer VT_extends_ITransformer;
  //typedef typename TVertexMarker::Parameters TVMParams;
  //typedef typename TPriorityQueue::Parameters TPQParams;
  //typedef typename ET::instanceOfTransformer ET_extends_ITransformer;
  public:
    //T=VertexPtr  : public binary_function<VertexPtr, VertexPtr, bool>
    
    typedef IVertex<V, E> Vertex;
    typedef IEdge<V, E> Edge;
    typedef const IVertex<V, E>* VertexPtr;
    typedef const IVertex<V, E>& VertexRef;
    typedef Iterator< VertexPtr > VertexIterator;
    typedef const IEdge<V, E>* EdgePtr;
    typedef const IEdge<V, E>& EdgeRef;
    typedef Iterator< EdgePtr > EdgeIterator;
    enum Color { WHITE=0, GRAY=1, BLACK=2 };
    
    /** 
     * User can supply a customized created PriorityQueue object in pq parameter. 
     * Just create a new object, and the DijkstraShortestPath object will handle the delete.
     * Example: DijkstraShortestPath dsp(graph, marker, new XXXPriorityQueue(p1,p1,xxx));
     */
    DijkstraShortestPath(const IGraph<V, E>& g, const IMarker<EdgePtr, W>* edgeWeightMarker = 0) 
                         : _g(&g), _ewm(edgeWeightMarker), INFINITY(std::numeric_limits<W>::max()), defaultEdgeWeightMarker(), _comp()
    {
      _pq = new TPriorityQueue(g.VerticesSize(), &g.GetVertexIndexer(), _comp);
      if (_ewm==0) { _ewm = &defaultEdgeWeightMarker; }
      //INFINITY = ;
    }
    
    virtual ~DijkstraShortestPath() {
      delete _pq;
    }
    //DijkstraShortestPath(const IGraph<V, E>& g, const IMarker<E, W>& edgeWeightMarker, IPriorityQueue<V, W>& pq) {}
//     void Run(const Graph::Vertex& s, IVisitor<V>& initVertex = _vv, IVisitor<V>& examineVertex = _vv, IVisitor<E>& examineEdge = _ev,
//              IVisitor<V>& discoverVertex = _vv, IVisitor<E>& edgeRelaxed = _ev, IVisitor<E>& edgeNotRelaxed = _ev, IVisitor<V>& finishVertex = _vv) {
    void Run(const IVertex<V, E>& s) {
      TPriorityQueue& pq = *_pq;
      const Indexer<VertexPtr>* indexer = &_g->GetVertexIndexer();
      uint32_t size = indexer->GetLastIndex()+1;
      IndexedArrayMarker<VertexPtr, W> dist(size, indexer); // distance marker
      IndexedArrayMarker<VertexPtr, VertexPtr> prev(size, indexer); // predecessor marker
      IndexedArrayMarker<VertexPtr, Color> visited(size, indexer); // visited marker
      //_comp.marker = &dist;
      pq.GetComparator().marker = &dist;
      // init
      VertexIterator i = _g->GetVertices();
      VertexPtr sptr = &s;
      while (i.HasNext()) {
        VertexPtr vptr = i.Next();
        dist.Set(vptr, INFINITY);
        prev.Set(vptr, 0);
        visited.Set(vptr, WHITE);
      }
      
      dist.Set(sptr, 0);
      pq.Push(sptr);
      while (!pq.Empty()){
        VertexPtr uptr = pq.Poll(); //vertex in Q with smallest dist[]
        std::cout << "uptr:" << uptr->GetValue() << std::endl;
        if (dist.Get(uptr) == INFINITY) break; // all remaining vertices are inaccessible from source
        visited.Set(uptr, BLACK); // put in the finished set S<--u
        // for each
        EdgeIterator iter = _g->GetOutEdges(*uptr);
        while (iter.HasNext()) {
          EdgePtr eptr = iter.Next();
          VertexPtr vptr = eptr->GetTargetVertex();
          Color color = visited.Get(vptr);
          if (color != BLACK) {
            W alt = dist.Get(uptr) + _ewm->Get(eptr);
            if (alt < dist.Get(vptr)) {             // Relax (u,v,a)
              dist.Set(vptr, alt);
              prev.Set(vptr, uptr);
              if (color == WHITE) {
                visited.Set(vptr, GRAY);
                pq.Push(vptr);
              } else { //else if gray
                pq.Update(vptr);
              }
            }
          }
        } //end for each

        //std::cout << "test:" << _pq.Poll()->GetValue() << std::endl;
      }
      VertexIterator it =  _g->GetVertices();
//  std::cout << "X1" << std::endl;
      while (it.HasNext()) {
        VertexPtr v = it.Next();
        V val;
        VertexPtr vp = prev.Get(v);
        if (vp!=0) val = vp->GetValue();
        std::cout << "v:" << v->GetValue() << " - p:" << val << std::endl;
      }
    }
    
//     class DefaultVertexVisitor : public IVisitor<V> {
//       void operator()(const V& v) const { //do nothing 
//       }
//     };
//     class DefaultEdgeVisitor : public IVisitor<E> {
//       void operator()(const V& v) const { //do nothing 
//       }
//     };
//     DefaultVertexVisitor _vv;
//     DefaultEdgeVisitor _ev;
    
  private:
    
    class DefaultEdgeWeightMarker : public IMarker<EdgePtr, W> {
      public:
        W Get(const EdgePtr& key) const { 
          return W(key->GetValue()); 
        }
        void Set(const EdgePtr& key, const W& value) { }
    };
    const DefaultEdgeWeightMarker defaultEdgeWeightMarker;

    struct VertexPtrComp {
      VertexPtrComp() : marker(0) {}
      bool operator() (const VertexPtr& x, const VertexPtr& y) const {
        assert(marker!=0);
        return (marker->Get(x))>(marker->Get(y));
      }
      IMarker<VertexPtr, W>* marker;
    };
    typedef IndexedArrayPriorityQueue<VertexPtr, VertexPtrComp> TPriorityQueue;
    
    
/*    class VertexPtrToUInt32Transformer : public ITransformer<IVertex<V, E>*, uint32_t> {
      public:
        uint32_t operator()(const IVertex<V, E>* t) {
          return uint32_t(t);
        }
        static const AnyToUInt32Transformer<T> instance;
    };*/
    
    //void Init(IGraph<V, E>* g, IMarker<E, W>& edgeWeightMarker, IPriorityQueue<V, W>& pq) {    }
    const IGraph<V, E>* _g;
    const IMarker<EdgePtr, W>* _ewm; // edge weight
    TPriorityQueue* _pq;
    const W INFINITY;
    VertexPtrComp _comp;

};

#endif // DIJKSTRASHORTESTPATH_H
