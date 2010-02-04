#ifndef BSMA_H
#define BSMA_H

#include "IGraph.h"
#include "PriorityQueue.h"
#include "DepthFirstIterator.h"
#include "Set.h"


template <typename V, typename E, typename W> 
//          typename TVertexMarker = IndexedArrayMarker , 
//          template <typename Y, typename X> class TVertexMarker = IndexedArrayMarker, 
//          typename TPriorityQueue = IndexedArrayPriorityQueue<const IVertex<V, E>*, W> >
class BoundedShortestMulticastAlgorithm {
  public:
    typedef PriorityQueue TPriorityQueue;
    typedef Path TPath;
    typedef typename IGraph<V,E>::VertexPtr VertexPtr;
    typedef typename IGraph<V,E>::VertexIterator VertexIterator;
    
    int K_MAX = 30;

  private:
    class SuperEdge : public IPath<VertexPtr> {
      operator<() {
      }
    };
    // tree must be optimized such that it does not contain leaves nodes that is not belong in targets
    void InitSuperEdges(TPriorityQueue& pq, const IGraph<V,E>& tree, VertexPtr source, Set<VertexPtr>& targets) {
      bool begin = true;
      SuperEdge se;
      DepthFirstVertexIterator<V, E> dfi(tree, source);
      while (dfi.HasNext()) {
        VertexPtr vptr = dfi.Next();
        if (begin==true) {
          se = SuperEdge();
          begin = false;
        }
        se.AddFront(tree.GetParent(vptr));
        if (targets.
      }
      
    }
};


#endif // BSMA_H
