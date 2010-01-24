#ifndef ABSTRACT_GRAPH_H
#define ABSTRACT_GRAPH_H

#include "IGraph.h"
#include "IMutableGraph.h"
#include <stdexcept>


/**
 * An abstract graph as a base class that implements some of the methods.
 */
template <typename V, typename E>
class AbstractGraph : public IMutableGraph<V, E> {
  protected:
    E EDGE_NONE;
    AbstractGraph() : EDGE_NONE( E() ) {}

  public:
    virtual const E& NoEdge() {
      return EDGE_NONE;
    }
    virtual bool IsDirected() { return false; }
    virtual bool IsParallel() { return false; }
    
    virtual const V& GetVertexValue(const IVertex<V, E>& vertex) const {
      return vertex.GetValue();
    }

    using IGraph<V, E>::GetEdgeValue;
    virtual const E& GetEdgeValue(const V& source, const V& target) const {
      const IVertex<V, E>* s = GetVertex(source);
      const IVertex<V, E>* t = GetVertex(target);
        if (s != 0 && t != 0) {
	  GetEdgeValue(*s, *t);
        } else {
            throw std::invalid_argument("Source or target vertex does not belong to this graph.");
        }
      ; 
    }

    virtual void SetNoEdgeValue(const E& e) {
      EDGE_NONE = e;
    }
    
    using IMutableGraph<V, E>::AddEdge;
    virtual bool AddEdge(const V& source, const V& target, const E& edge) {
      const IVertex<V, E>* s = GetVertex(source);
      const IVertex<V, E>* t = GetVertex(target);
      if (s != 0 && t != 0) {
	return AddEdge(*s, *t, edge);
      } else {
	  throw std::invalid_argument("Source or target vertex does not belong to this graph.");
      }
    }
    
    virtual void RemoveAllEdges(void) {
      //loop and remove edge
      //RemoveEdges();
    }
    
    using IMutableGraph<V, E>::RemoveEdge;
    virtual bool RemoveEdge(const V& source, const V& target, const E& e) {
      return RemoveEdge(*GetVertex(source), *GetVertex(target), e);
    }
    
    using IMutableGraph<V, E>::RemoveEdges;
    virtual bool RemoveEdges(const V& source, const V& target) {
      return RemoveEdges(*GetVertex(source), *GetVertex(target));
    }
    
    using IMutableGraph<V, E>::RemoveVertex;
    virtual bool RemoveVertex(const V& v) {
      IVertex<V, E>* vx = const_cast< IVertex<V, E>* >( GetVertex(v) );
      return RemoveVertex(*vx);
    }
    
};

#endif // ABSTRACT_GRAPH_H