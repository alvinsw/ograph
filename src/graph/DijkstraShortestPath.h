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

#include "IPriorityQueue.h"
#include "Marker.h"
#include "IGraph.h"
#include <iostream>

template <typename T>
class IVisitor {
  virtual void operator()(const T& t) const = 0;
};


/**
 * W is the type of weight, must be number (int, long, double, etc)
 */
template <typename V, typename E, typename W, 
//          typename TVertexMarker = IndexedArrayMarker , 
          template <typename Y, typename X> class TVertexMarker = IndexedArrayMarker, 
          typename TPriorityQueue = IndexedArrayPriorityQueue<const IVertex<V, E>*, W> >
class DijkstraShortestPath {
  typedef typename TPriorityQueue::instanceOfPriorityQueue PriorityQueue_extends_IPriorityQueue;
  //typedef typename VT::instanceOfTransformer VT_extends_ITransformer;
  //typedef typename TVertexMarker::Parameters TVMParams;
  typedef typename TPriorityQueue::Parameters TPQParams;
  //typedef typename ET::instanceOfTransformer ET_extends_ITransformer;
  public:
    typedef IVertex<V, E> Vertex;
    typedef const IVertex<V, E>* VertexPtr;
    typedef const IVertex<V, E>& VertexRef;
    typedef IEdge<V, E> Edge;
    typedef const IEdge<V, E>* EdgePtr;
    typedef const IEdge<V, E>& EdgeRef;
    
    /** 
     * User can supply a customized created PriorityQueue object in pq parameter. 
     * Just create a new object, and the DijkstraShortestPath object will handle the delete.
     * Example: DijkstraShortestPath dsp(graph, marker, new XXXPriorityQueue(p1,p1,xxx));
     */
    DijkstraShortestPath(const IGraph<V, E>& g, const IMarker<EdgePtr, W>* edgeWeightMarker = 0, const TPQParams* pqp = 0) 
                         : _g(&g), _ewm(edgeWeightMarker), INFINITY(std::numeric_limits<W>::max())
    {
      if (pqp==0) { 
        _pq = TPriorityQueue(TPQParams(g.GetVertexIndexer()));
      } else {
        _pq = TPriorityQueue(*pqp);
      }
      if (_ewm==0) { _ewm == &defaultEdgeWeightMarker; }
      //INFINITY = ;
    }
    
    virtual ~DijkstraShortestPath() {
    }
    //DijkstraShortestPath(const IGraph<V, E>& g, const IMarker<E, W>& edgeWeightMarker, IPriorityQueue<V, W>& pq) {}
//     void Run(const Graph::Vertex& s, IVisitor<V>& initVertex = _vv, IVisitor<V>& examineVertex = _vv, IVisitor<E>& examineEdge = _ev,
//              IVisitor<V>& discoverVertex = _vv, IVisitor<E>& edgeRelaxed = _ev, IVisitor<E>& edgeNotRelaxed = _ev, IVisitor<V>& finishVertex = _vv) {
    void Run(const IVertex<V, E>& s) {
      const Indexer<VertexPtr>* indexer = &_g->GetVertexIndexer();
      uint32_t size = indexer->GetLastIndex();
      TVertexMarker<VertexPtr, W> dist(size, indexer); // distance marker
      TVertexMarker<VertexPtr, VertexPtr> prev(size, indexer); // predecessor marker
      TVertexMarker<VertexPtr, bool> visited(size, indexer); // visited marker
      // init
      Iterator< const IVertex<V, E>& > i = _g->GetVertices();
      const IVertex<V, E>* sptr = &s;
      while (i.HasNext()) {
        const IVertex<V, E>* vptr = &i.Next();
        dist.Set(vptr, INFINITY);
        prev.Set(vptr, 0);
        visited.Set(vptr, false);
        //_pq.Push(v, d.Get(v)); //test
        //std::cout << "test:" << v->GetValue() << " - " << x << std::endl;
        //_pq.Push(v, x); //test
        //x /= 2.0f;
      }
      visited.Set(sptr, true);
      dist.Set(sptr, 0);
      _pq.Push(sptr, 0);
      while (!_pq.Empty()){
        const IVertex<V, E>* uptr = _pq.Poll(); //vertex in Q with smallest dist[]
        if (dist.Get(uptr) == INFINITY) break; // all remaining vertices are inaccessible from source
        // for each
        Iterator< const IEdge<V, E>& > iter = _g->GetOutEdges(*uptr);
        while (iter.HasNext()) {
          const IEdge<V, E>* eptr = &iter.Next();
          const IVertex<V, E>* vptr = &eptr->GetTargetVertex();
          if (visited.Get(vptr) == false) {
            W alt = dist.Get(uptr) + _ewm->Get(eptr);
            if (alt < dist.Get(vptr)) {             // Relax (u,v,a)
              dist.Set(vptr, alt);
              prev.Set(vptr, uptr);
              _pq.Update(vptr, alt);
            }            
          }
          
        } //end for each

        std::cout << "test:" << _pq.Poll()->GetValue() << std::endl;
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
        W Get(const EdgePtr& key) const { return W(key->GetValue()); }
        void Set(const EdgePtr& key, const W& value) { }
    };
    DefaultEdgeWeightMarker defaultEdgeWeightMarker;
    
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
    TPriorityQueue _pq;
    const W INFINITY;
};

#endif // DIJKSTRASHORTESTPATH_H
