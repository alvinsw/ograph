#ifndef I_GRAPH_H
#define I_GRAPH_H

#include <stdint.h>
#include "Iterator.h"

template <typename V, typename E>
class IGraph;

template <typename V, typename E>
class IVertex {
  public:
    virtual ~IVertex() { };
    virtual const V& GetValue() const = 0;
    virtual IGraph<V, E>* GetOwner() const = 0;
};

template <typename V, typename E>
class IEdge {
  public:
    virtual ~IEdge() { };
    virtual const E& GetValue() const = 0;
    virtual IGraph<V, E>* GetOwner() const = 0;
    virtual const IVertex<V, E>& GetSourceVertex() const = 0;
    virtual const IVertex<V, E>& GetTargetVertex() const = 0;
    virtual const V& GetSourceValue() const {
      return GetSourceVertex().GetValue();
    }
    virtual const V& GetTargetValue() const {
      return GetTargetVertex().GetValue();
    }
};

template <typename V, typename E>
class IGraph {
  public:
    typedef const IVertex<V, E> Vertex;
    typedef const IEdge<V, E> Edge;
    typedef const IVertex<V, E>* VertexPtr;
    typedef const IVertex<V, E>& VertexRef;
    typedef Iterator< const VertexRef > VertexIterator;
    typedef const IEdge<V, E>& EdgeRef;
    typedef Iterator< const EdgeRef > EdgeIterator;

    virtual ~IGraph() { };
    virtual const E& NoEdge() const = 0; //
    virtual bool IsDirected() = 0; //
    virtual bool IsParallel() = 0; //
    virtual bool IsLoop() = 0; //
    virtual uint32_t VerticesSize() const = 0; //
    virtual uint32_t EdgesSize() const = 0; //
    /** Gets the vertex descriptor, return 0 if it is not found. IVertex is an object managed by the graph, do not delete! */
    virtual Vertex* GetVertex ( const V& v ) const = 0; //
    virtual const V& GetVertexValue ( const IVertex<V, E>& vertex ) const = 0; //
    virtual const E& GetEdgeValue ( const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex ) const = 0; //
    virtual const E& GetEdgeValue ( const V& source, const V& target ) const = 0; //

    virtual VertexIterator GetVertices() const = 0; //
    virtual EdgeIterator GetEdges() const = 0; //

    //virtual EdgeIterator GetAllEdges ( const IVertex<V, E>& vertex ) const = 0;
    virtual EdgeIterator GetInEdges ( const IVertex<V, E>& vertex ) const = 0;
    virtual EdgeIterator GetOutEdges ( const IVertex<V, E>& vertex ) const = 0;
    //virtual VertexIterator GetAllAdjVertices ( const IVertex<V, E>& vertex ) const = 0;
    virtual VertexIterator GetAdjVertices ( const IVertex<V, E>& vertex ) const = 0; // provided for convenient
    virtual VertexIterator GetPredecessor ( const IVertex<V, E>& vertex ) const = 0; //in digraph predecessor is the same with successor
    virtual VertexIterator GetSuccessor ( const IVertex<V, E>& vertex ) const = 0;
    virtual uint32_t InDegree(const IVertex<V, E>& vertex) const = 0;
    virtual uint32_t OutDegree(const IVertex<V, E>& vertex) const = 0;
    //virtual const Vertex& GetOpposite( const Vertex& vertex, const Edge& edge ) = 0;
};

#endif
