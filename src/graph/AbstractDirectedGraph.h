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
    typedef typename IGraph<V, E>::VertexPtr VertexPtr;
    typedef typename IGraph<V, E>::VertexRef VertexRef;
    typedef typename IGraph<V, E>::VertexIterator VertexIterator;
    typedef typename IGraph<V, E>::EdgePtr EdgePtr;
    typedef typename IGraph<V, E>::EdgeRef EdgeRef;
    typedef typename IGraph<V, E>::EdgeIterator EdgeIterator;

//     using IGraph<V, E>::GetVertex;
//     using IGraph<V, E>::GetVertices;
//     using IMutableGraph<V, E>::AddVertex;

    virtual const E& NoEdge() const { return EDGE_NONE; }
    virtual bool IsDirected() { return true; }
    virtual bool IsParallel() { return false; } //does not allow parallel edges
    virtual bool IsLoop() { return true; } // allow loops

    virtual uint32_t VerticesSize() const = 0; //
    virtual uint32_t EdgesSize() const = 0; //
    virtual VertexPtr GetVertex (const V& v) const = 0; //

    virtual const V& GetVertexValue (VertexRef vertex) const {
      return vertex.GetValue();
    }

    virtual const E& GetEdgeValue (VertexRef sourceVertex, VertexRef targetVertex ) const = 0; //
    //using IGraph<V, E>::GetEdgeValue;
    virtual const E& GetEdgeValue ( const V& source, const V& target ) const {
      VertexPtr s = GetVertex ( source );
      VertexPtr t = GetVertex ( target );
      if ( s != 0 && t != 0 ) {
        return GetEdgeValue ( *s, *t );
      } else {
        throw std::invalid_argument ( "Source or target vertex does not belong to this graph." );
      }
    }

    virtual VertexIterator GetVertices() const = 0; //
    virtual EdgeIterator GetEdges() const = 0; //

    virtual VertexIterator GetAdjVertices ( const IVertex<V, E>& vertex ) const {
      return this->GetSuccessor(vertex);
    }

    virtual VertexIterator GetPredecessor (VertexRef vertex ) const = 0; //in digraph predecessor is the same with successor
    virtual VertexIterator GetSuccessor (VertexRef vertex ) const = 0;
    virtual uint32_t InDegree(VertexRef vertex) const = 0;
    virtual uint32_t OutDegree(VertexRef vertex) const = 0;

    virtual VertexPtr GetOpposite(VertexRef vertex, EdgeRef edge ) {
      if (edge.GetSourceVertex()->Equals(vertex)) {
        return edge.GetTargetVertex();
      } else if (edge.GetTargetVertex()->Equals(vertex)) {
        return edge.GetSourceVertex();
      }
      return 0;
    }
    
    virtual bool Contains(VertexRef sourceVertex, VertexRef targetVertex) const {
      return (GetEdgeValue(sourceVertex, targetVertex)!=NoEdge());
    }

    /////////////IMutableGraph

    virtual void SetNoEdgeValue ( const E& e ) { EDGE_NONE = e; }

    virtual VertexPtr AddVertex(const V& v) = 0;
    virtual EdgePtr AddEdge(VertexRef sourceVertex, VertexRef targetVertex, const E& edge) = 0;

    //using IMutableGraph<V, E>::AddEdge;
    virtual EdgePtr AddEdge ( const V& source, const V& target, const E& edge ) {
      VertexPtr s = GetVertex ( source );
      VertexPtr t = GetVertex ( target );
      if (s == 0) {
        s = AddVertex(source);
      }
      if (t == 0) {
        t = AddVertex(target);
      }
      if ( s != 0 && t != 0 ) {
        return AddEdge ( *s, *t, edge );
      } else {
        throw std::invalid_argument ( "Source or target vertex does not belong to this graph." );
      }
    }

    virtual void RemoveAllEdges () {
      VertexIterator i1 = this->GetVertices();
      VertexIterator i2 = this->GetVertices();
      //loop and remove edge
      while ( i1.HasNext() ) {
        VertexPtr v1 = i1.Next();
        while ( i2.HasNext() ) {
          VertexPtr v2 = i2.Next();
          RemoveEdges (*v1, *v2);
        }
      }
    }

    virtual bool RemoveEdge(EdgeRef edge) = 0;

    virtual bool RemoveEdge(VertexRef sourceVertex, VertexRef targetVertex, const E& e) = 0;
    //using IMutableGraph<V, E>::RemoveEdge;
    virtual bool RemoveEdge ( const V& source, const V& target, const E& e ) {
      return RemoveEdge ( *GetVertex ( source ), *GetVertex ( target ), e );
    }

    virtual bool RemoveEdges(VertexRef sourceVertex, VertexRef targetVertex) = 0;
    //using IMutableGraph<V, E>::RemoveEdges;
    virtual bool RemoveEdges ( const V& source, const V& target ) {
      return RemoveEdges ( *GetVertex ( source ), *GetVertex ( target ) );
    }

    virtual bool RemoveVertex(VertexRef vertex) = 0;
    //using IMutableGraph<V, E>::RemoveVertex;
    virtual bool RemoveVertex ( const V& v ) {
      VertexPtr vp = GetVertex(v);
      return RemoveVertex ( *vp );
    }

};

#endif // ABSTRACT_GRAPH_H
