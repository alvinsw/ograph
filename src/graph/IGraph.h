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
    virtual const V& GetSourceValue() const { return GetSourceVertex().GetValue(); }
    virtual const V& GetTargetValue() const { return GetTargetVertex().GetValue(); }
};

template <typename V, typename E>
class IGraph {
  public:
    virtual ~IGraph() { };
    virtual const E& NoEdge() = 0;
    virtual bool IsDirected() = 0;
    virtual bool IsParallel() = 0;
    virtual uint32_t VerticesSize() const = 0;
    virtual uint32_t EdgesSize() const = 0; 
    /** Gets the vertex descriptor, return 0 if it is not found. IVertex is an object managed by the graph */
    virtual const IVertex<V, E>* GetVertex(const V& v) const = 0; 
    virtual const V& GetVertexValue(const IVertex<V, E>& vertex) const = 0; 
    virtual const E& GetEdgeValue(const IVertex<V, E>& source, const IVertex<V, E>& target) const = 0; 
    virtual const E& GetEdgeValue(const V& source, const V& target) const = 0; 
    virtual Iterator< IVertex<V, E>& > Vertices(void) const = 0;
    virtual Iterator< IVertex<V, E>& > AdjVertices(const IVertex<V, E>& vertex) const = 0;
};

#endif
