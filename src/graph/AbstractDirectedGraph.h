#ifndef ABSTRACT_GRAPH_H
#define ABSTRACT_GRAPH_H

#include "IGraph.h"
#include "IMutableGraph.h"
#include <stdexcept>


/**
 * An abstract graph as a base class that implements some of the methods.
 */
template <typename V, typename E>
class AbstractDirectedGraph : public IMutableGraph<V, E> {
  protected:
    E EDGE_NONE;
    AbstractDirectedGraph ( E NoEdgeValue = E() ) : EDGE_NONE ( NoEdgeValue ) {}

  public:
    typedef typename IGraph<V, E>::Vertex Vertex;
    typedef typename IGraph<V, E>::Edge Edge;
    typedef typename IGraph<V, E>::VertexRef VertexRef;
    typedef typename IGraph<V, E>::VertexPtr VertexPtr;
    typedef typename IGraph<V, E>::VertexIterator VertexIterator;
    typedef typename IGraph<V, E>::EdgeRef EdgeRef;
    typedef typename IGraph<V, E>::EdgeIterator EdgeIterator;

    using IGraph<V, E>::GetVertex;
    using IGraph<V, E>::GetVertices;
    using IMutableGraph<V, E>::AddVertex;

    virtual const E& NoEdge() const {
      return EDGE_NONE;
    }
    virtual bool IsDirected() {
      return true;
    }
    virtual bool IsParallel() {
      return false;
    }
    virtual bool IsLoop() {
      return true;
    }

    virtual const V& GetVertexValue ( const IVertex<V, E>& vertex ) const {
      return vertex.GetValue();
    }

    using IGraph<V, E>::GetEdgeValue;
    virtual const E& GetEdgeValue ( const V& source, const V& target ) const {
      VertexPtr s = GetVertex ( source );
      VertexPtr t = GetVertex ( target );
      if ( s != 0 && t != 0 ) {
        GetEdgeValue ( *s, *t );
      } else {
        throw std::invalid_argument ( "Source or target vertex does not belong to this graph." );
      }
      ;
    }
    
    virtual VertexIterator GetAdjVertices ( const IVertex<V, E>& vertex ) const {
      return this->GetSuccessor(vertex);
    }
    
    /////////////IMutableGraph

    virtual void SetNoEdgeValue ( const E& e ) {
      EDGE_NONE = e;
    }

    using IMutableGraph<V, E>::AddEdge;
    virtual bool AddEdge ( const V& source, const V& target, const E& edge ) {
      Vertex* s = GetVertex ( source );
      Vertex* t = GetVertex ( target );
      if (s == 0) {
        s = &AddVertex(source);
      }
      if (t == 0) {
        t = &AddVertex(target);
      }
      if ( s != 0 && t != 0 ) {
        return AddEdge ( *s, *t, edge );
      } else {
        throw std::invalid_argument ( "Source or target vertex does not belong to this graph." );
      }
    }

    virtual void RemoveAllEdges ( void ) {
      VertexIterator i1 = this->GetVertices();
      VertexIterator i2 = this->GetVertices();
      //loop and remove edge
      while ( i1.HasNext() ) {
        VertexRef v1 = i1.Next();
        while ( i2.HasNext() ) {
          VertexRef v2 = i2.Next();
          RemoveEdges ( v1, v2 );
        }
      }
    }

    using IMutableGraph<V, E>::RemoveEdge;
    virtual bool RemoveEdge ( const V& source, const V& target, const E& e ) {
      return RemoveEdge ( *GetVertex ( source ), *GetVertex ( target ), e );
    }

    using IMutableGraph<V, E>::RemoveEdges;
    virtual bool RemoveEdges ( const V& source, const V& target ) {
      return RemoveEdges ( *GetVertex ( source ), *GetVertex ( target ) );
    }

    using IMutableGraph<V, E>::RemoveVertex;
    virtual bool RemoveVertex ( const V& v ) {
      IVertex<V, E>* vx = const_cast< IVertex<V, E>* > ( GetVertex ( v ) );
      return RemoveVertex ( *vx );
    }

};

#endif // ABSTRACT_GRAPH_H
