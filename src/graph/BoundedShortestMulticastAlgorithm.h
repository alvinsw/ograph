#ifndef BSMA_H
#define BSMA_H

#include "IGraph.h"
#include "PriorityQueue.h"
#include "DepthFirstIterator.h"
#include "Set.h"
#include "Path.h"


template <typename V, typename E, typename W> 
//          typename TVertexMarker = IndexedArrayMarker , 
//          template <typename Y, typename X> class TVertexMarker = IndexedArrayMarker, 
//          typename TPriorityQueue = IndexedArrayPriorityQueue<const IVertex<V, E>*, W> >
class BoundedShortestMulticastAlgorithm {
  public:
    typedef Path TPath;
    typedef typename IGraph<V,E>::VertexPtr VertexPtr;
    typedef typename IGraph<V,E>::VertexIterator VertexIterator;
    typedef typename IGraph<V,E>::EdgePtr EdgePtr;
    
    int K_MAX = 30;
    
    void Run() {
      TPriorityQueue pq;
      
    }

  private:
    typedef const SuperEdge* SuperEdgePtr;
    typedef PriorityQueue<SuperEdge*, CompareSuperEdge> TPriorityQueue;
    struct CompareSuperEdge {
      bool operator() (const SuperEdgePtr& x, const SuperEdgePtr& y) const {
        assert(x!=0);
        assert(y!=0);
        return (x->GetCost())<(y->GetCost());
      }
    };
    class SuperEdge : public ListPath<VertexPtr> {
      public:
        SuperEdge() : _costCache(0) {}
        void AddBack(const VertexPtr& vptr, W cost) {
          AddBack(vptr);
          _costCache += cost;
        }
        W GetCost() const {
          return _costCache;
        }
        W _costCache;
    };
    // tree must be guranteed to be a tree
    // tree must be optimized such that it does not contain leaves nodes that is not belong in targets
    void InitSuperEdges(TPriorityQueue& pq, const IGraph<V,E>& tree, VertexPtr source, const Set<VertexPtr>& targets) {
      int maxOutDegree = 2;
      if (tree.IsDirected()) maxOutDegree = 1;
      bool begin = true;
      SuperEdge* sePtr;
      DepthFirstEdgeIterator<V, E> dfi(tree, source);
      while (dfi.HasNext()) {
        EdgePtr ePtr = dfi.Next();
        if (begin==true) {
          sePtr = new SuperEdge();
          begin = false;
          sePtr->AddBack(ePtr->GetSourceVertex());
        }
        sePtr->AddBack(ePtr->GetTargetVertex(), _ewm.Get(ePtr));
//        if (targets.Contains(vptr) || tree.GetChildrenCount(vptr) > 1) {
        if (targets.Contains(vptr) || tree.OutDegree(vptr) > maxOutDegree) {
          pq.Push(se);
          sePtr = 0;
          begin = true;
        }
      }
    }
    
    const IMarker<EdgePtr, W>& _ewm; // edge weight

};


#endif // BSMA_H
