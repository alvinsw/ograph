#ifndef I_GRAPH_H
#define I_GRAPH_H

#include <stdint.h>
#include "Iterator.h"
#include "Marker.h"
#include "Transformer.h"

class IBase {
  public:
    virtual ~IBase() { };
    virtual IBase* Clone() const = 0;
    virtual bool Equals(const IBase& obj) const = 0;
};

template <typename V, typename E>
class IGraph;

template <typename V, typename E>
class IVertex : public IBase {
  public:
    virtual ~IVertex() { };
    virtual const V& GetValue() const = 0;
    virtual IGraph<V, E>* GetOwner() const = 0;
    //virtual operator uint32_t() const = 0;
    //virtual ostream& operator<< (ostream& out)
};

template <typename V, typename E>
class IEdge : public IBase {
  public:
    typedef const IVertex<V, E>* VertexPtr;

    virtual ~IEdge() { };
    virtual const E& GetValue() const = 0;
    virtual IGraph<V, E>* GetOwner() const = 0;
    virtual VertexPtr GetSourceVertex() const = 0;
    virtual VertexPtr GetTargetVertex() const = 0;
    virtual const V& GetSourceValue() const { return GetSourceVertex()->GetValue(); }
    virtual const V& GetTargetValue() const { return GetTargetVertex()->GetValue(); }
    virtual bool Equals(const IBase& obj) const { 
      const IEdge& e = static_cast<const IEdge&>(obj);
      return (GetValue()==e.GetValue() && GetSourceValue()==e.GetSourceValue() && GetTargetValue()==e.GetTargetValue());
    }

};

template <typename T>
class Indexer : public ITransformer<T, uint32_t> {
  public:
    //virtual uint32_t operator()(const T& t) = 0;
    virtual uint32_t GetLastIndex() const = 0;
};

template <typename V, typename E>
class IGraph {
  public:
    typedef IVertex<V, E> Vertex;
    typedef IEdge<V, E> Edge;
    typedef const IVertex<V, E>* VertexPtr;
    typedef const IVertex<V, E>& VertexRef;
    typedef Iterator< VertexPtr > VertexIterator;
    typedef const IEdge<V, E>* EdgePtr;
    typedef const IEdge<V, E>& EdgeRef;
    typedef Iterator< EdgePtr > EdgeIterator;
    //typedef std::tr1::shared_ptr< IMarker< V>

    virtual ~IGraph() { }
    
    virtual const E& NoEdge() const = 0; //
    virtual bool IsDirected() = 0; //
    virtual bool IsParallel() = 0; //
    virtual bool IsLoop() = 0; //
    virtual uint32_t VerticesSize() const = 0; //
    virtual uint32_t EdgesSize() const = 0; //
    /** Gets the vertex descriptor, return 0 if it is not found. IVertex is an object managed by the graph, do not delete! */
    virtual VertexPtr GetVertex (const V& v) const = 0; //
    virtual const V& GetVertexValue (VertexRef vertex) const = 0; //
    virtual const E& GetEdgeValue (VertexRef sourceVertex, VertexRef targetVertex ) const = 0; //
    virtual const E& GetEdgeValue (const V& source, const V& target) const = 0; //

    virtual VertexIterator GetVertices() const = 0; //
    virtual EdgeIterator GetEdges() const = 0; //

    //virtual EdgeIterator GetAllEdges ( const IVertex<V, E>& vertex ) const = 0;
    virtual EdgeIterator GetInEdges (VertexRef vertex ) const = 0;
    virtual EdgeIterator GetOutEdges (VertexRef vertex ) const = 0;
    //virtual VertexIterator GetAllAdjVertices ( const IVertex<V, E>& vertex ) const = 0;
    virtual VertexIterator GetAdjVertices (VertexRef vertex ) const = 0; // provided for convenient
    virtual VertexIterator GetPredecessor (VertexRef vertex ) const = 0; //in digraph predecessor is the same with successor
    virtual VertexIterator GetSuccessor (VertexRef vertex ) const = 0;
    virtual uint32_t InDegree(VertexRef vertex) const = 0;
    virtual uint32_t OutDegree(VertexRef vertex) const = 0;
    virtual VertexPtr GetOpposite(VertexRef vertex, EdgeRef edge ) = 0;
//    template <typename T>
//    virtual IMarker<Vertex, T>* CreateVertexMarker() const {};
    virtual const Indexer<VertexPtr>& GetVertexIndexer() const = 0;
    virtual const Indexer<EdgePtr>& GetEdgeIndexer() const = 0;
};

template <typename V, typename E>
std::ostream& operator<< (std::ostream& out, const IEdge<V,E>& e) {
  out << "IEdge<V,E>=[" <<  e.GetSourceValue() << "-->" << e.GetTargetValue() << ": " << e.GetValue() << "]";
  return out;
}

#endif
