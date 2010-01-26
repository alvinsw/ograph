#ifndef DIGRAPHADJMATRIX_H
#define DIGRAPHADJMATRIX_H

#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include "AbstractDirectedGraph.h"

/**
 * characteristics: directional, no parallel edge
 */
template <typename V, typename E>
class DiGraphAdjMatrix: public AbstractDirectedGraph<V, E> {
  public:
    static uint32_t const DEFAULT_CAPACITY = 16;
    //bool const MAINTAIN_VERTICES_ORDER;
    bool checkAddedVertex; // = true
    //using IGraph<V, E>::VertexRef;
    //using IGraph<V, E>::VertexIterator;
    //using IGraph<V, E>::EdgeRef;
    //using IGraph<V, E>::EdgeIterator;
    typedef typename IGraph<V, E>::Vertex Vertex;
    typedef typename IGraph<V, E>::Edge Edge;
    typedef typename IGraph<V, E>::VertexRef VertexRef;
    typedef typename IGraph<V, E>::VertexPtr VertexPtr;
    typedef typename IGraph<V, E>::VertexIterator VertexIterator;
    typedef typename IGraph<V, E>::EdgeRef EdgeRef;
    typedef typename IGraph<V, E>::EdgeIterator EdgeIterator;
    //using AbstractDirectedGraph<V, E>::EDGE_NONE;

    DiGraphAdjMatrix ( uint32_t capacity = DEFAULT_CAPACITY, E NoEdgeValue = E() ) : AbstractDirectedGraph<V, E> ( NoEdgeValue ),
        _verticesSize ( 0 ), _edgesSize ( 0 ), _capacity ( capacity ), checkAddedVertex ( true ), _edgesMatrix ( capacity, std::vector<E> ( capacity, NoEdgeValue ) ) { }

    ~DiGraphAdjMatrix() {
      //std::cout << "destructor" << std::endl;
    }

    uint32_t VerticesSize() const {
      return _verticesSize;
    }

    uint32_t EdgesSize() const {
      return _edgesSize;
    }

    VertexPtr GetVertex ( const V& v ) const {
      typename std::map<V, VertexImpl>::const_iterator i = _vertices.find ( v );

      if ( i != _vertices.end() ) {
        return & ( i->second );
      }
      return 0;
    }

    const E& GetEdgeValue ( const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex) const {
      if ( sourceVertex.GetOwner() == this && targetVertex.GetOwner() == this ) {
        const VertexImpl& s = dynamic_cast<const VertexImpl&> ( sourceVertex );
        const VertexImpl& t = dynamic_cast<const VertexImpl&> ( targetVertex );
        return _edgesMatrix[s._index][t._index];
      } else {
        throw std::invalid_argument ( "Source or target vertex does not belong to this graph." );
      }
    }

    VertexIterator GetVertices() const {
      //VertexIterator* vi = new VertexIterator(this);
      //VertexIteratorImpl* vi = new VertexIteratorImpl(const_cast<DiGraphAdjMatrix*>(this));
      VertexIteratorImpl* vi = new VertexIteratorImpl ( this );
      return ( VertexIterator ( vi ) );
    }

    EdgeIterator GetEdges() const {
      EdgeIteratorImpl* ei = new EdgeIteratorImpl ( this );
      return ( EdgeIterator ( ei ) );
    }
    
    //EdgeIterator GetAllEdges ( const IVertex<V, E>& vertex ) const {}
    EdgeIterator GetInEdges ( const IVertex<V, E>& vertex ) const {}
    EdgeIterator GetOutEdges ( const IVertex<V, E>& vertex ) const {}

    //VertexIterator GetAllAdjVertices ( const IVertex<V, E>& vertex ) const {}
    VertexIterator GetPredecessor ( const IVertex<V, E>& vertex ) const {
    }
    VertexIterator GetSuccessor ( const IVertex<V, E>& vertex ) const {
//       VertexIterator* vi = new VertexIterator(const_cast<DiGraphAdjMatrix*>(this));
//       return (Iterator< IVertex<V, E>& >(vi));
      //VertexIteratorImpl* vi = new EdgeIteratorImpl ( this );
      //return VertexIterator(avi);
    }

    uint32_t InDegree(const IVertex<V, E>& vertex) const {
      uint32_t c = 0;
      const VertexImpl& v = AssertVertex( vertex );
      for (int i=0; i<_verticesSize; i++) {
	if (_edgesMatrix[i][v._index]!=this->NoEdge()) {
	  ++c;
	}
      }
      return c;
    }
    uint32_t OutDegree(const IVertex<V, E>& vertex) const {
      uint32_t c = 0;
      const VertexImpl& v = AssertVertex( vertex );
      for (int i=0; i<_verticesSize; i++) {
	if (_edgesMatrix[v._index][i]!=this->NoEdge()) {
	  ++c;
	}
      }
      return c;
    }



    VertexRef AddVertex ( const V& v ) {
      //typename std::pair<typename std::map<V, VertexImpl>::iterator, bool> result = _vertices.insert ( std::pair<V, VertexImpl> ( v, VertexImpl ( this, _verticesSize, v ) ) );
      typename std::pair<typename VerticesMap::iterator, bool> result = _vertices.insert ( std::pair<V, VertexImpl> ( v, VertexImpl ( this, _verticesSize, v ) ) );

      _verticesSize++;
      return result.first->second;
    }

    bool AddEdge ( const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex, const E& edge ) {
      const VertexImpl& s = AssertVertex( sourceVertex );
      const VertexImpl& t = AssertVertex( targetVertex );
      E& e = _edgesMatrix[s._index][t._index];
      if ( e==this->NoEdge() && e!=edge) {
        _edgesMatrix[s._index][t._index] = edge;
        return true;
      }
      return false;
    }

    virtual void Clear() {
      RemoveAllEdges();
      _vertices.clear();
      _verticesSize = 0;

    };

    virtual void RemoveAllEdges() {
      for ( int i = 0; i < _edgesSize; i++ ) {
        for ( int j = 0; j < _edgesSize; j++ ) {
          _edgesMatrix[i][j] = this->NoEdge();
        }
      }
      _edgesSize = 0;
    }

    virtual bool RemoveEdge ( IEdge<V, E>& edge ) {
      return false;
    }
    virtual bool RemoveEdge ( const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex, const E& e ) {
      return false;
    }
    virtual bool RemoveEdges ( const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex ) {
      return false;
    }
    virtual bool RemoveVertex ( IVertex<V, E>& v ) {
      return false;
    }


  private:
    class VertexImpl;
    friend class DiGraphAdjMatrix::VertexImpl;
    class VertexImpl : public IVertex<V, E> {
      public:
        VertexImpl (DiGraphAdjMatrix* const owner, uint32_t index, const V& value ) : _owner ( owner ), _index ( index ), _value ( value ) { }
        ~VertexImpl() { }
        const V& GetValue () const {
          return _value;
        }
        IGraph<V, E>* GetOwner () const {
          return _owner;
        }
      private:
        V _value;
        uint32_t _index;
        DiGraphAdjMatrix* _owner;
        friend class DiGraphAdjMatrix;
    };

    class EdgeImpl;
    friend class DiGraphAdjMatrix::EdgeImpl;
    class EdgeImpl : public IEdge<V, E> {
      public:
        EdgeImpl (const DiGraphAdjMatrix* owner, VertexImpl* sv=0, VertexImpl* tv=0, const E* value=0 ) : _owner(owner), _sv(sv), _tv(tv), _value(value) { }
        ~EdgeImpl() { }
        const E& GetValue() const { return *_value; }
        IGraph<V, E>* GetOwner() const { return const_cast< DiGraphAdjMatrix* >(_owner); }
        const IVertex<V, E>& GetSourceVertex() const { return *_sv; }
        const IVertex<V, E>& GetTargetVertex() const { return *_tv; }
      private:
        const E* _value;
        const VertexImpl* _sv;
        const VertexImpl* _tv;
        const DiGraphAdjMatrix* _owner;
        friend class DiGraphAdjMatrix::EdgeIteratorImpl;
    };

    class VertexIteratorImpl;
    friend class DiGraphAdjMatrix::VertexIteratorImpl;
    class VertexIteratorImpl : public IIterator< const IVertex<V, E>& > {
      public:
        VertexIteratorImpl ( const DiGraphAdjMatrix* owner ) : _cursor ( owner->_vertices.begin() ), _end ( owner->_vertices.end() ) { }
        ~VertexIteratorImpl() { }
        bool HasNext () {
          return ( _cursor != _end );
        }
        const IVertex<V, E>& Next () {
          const IVertex<V, E>* v = &_cursor->second;
          ++_cursor;
          return *v;
        }
        /** Returns a new object which is the exact copy of this object */
        VertexIteratorImpl* Clone () {
          return ( new VertexIteratorImpl ( *this ) );
        }
      private:
        //const DiGraphAdjMatrix* _owner;
        /*        typename std::map<V, Vertex>::iterator _cursor;
                typename std::map<V, Vertex>::iterator _end;*/
        typename std::map<V, VertexImpl>::const_iterator _cursor;
        typename std::map<V, VertexImpl>::const_iterator _end;
    };

    class EdgeIteratorImpl;
    friend class DiGraphAdjMatrix::EdgeIteratorImpl;
    class EdgeIteratorImpl : public IIterator< EdgeRef > {
      public:
        EdgeIteratorImpl (const DiGraphAdjMatrix* owner ) : _begin ( owner->_vertices.begin() ), _cursor1(_begin), _cursor2(_begin), _end ( owner->_vertices.end() ), _edge(owner), _owner(owner) {
          NextEdge();
        }
        EdgeIteratorImpl ( EdgeIteratorImpl* e ) : _begin(e->_begin), _cursor1(e->_cursor1), _cursor2(e->_cursor2), _end(e->_end), _edge(e->_edge), _owner(e->_owner) {
          NextEdge();
        }
        ~EdgeIteratorImpl() { }
        bool HasNext () {
          return ( _cursor1 != _end );
        }
        EdgeRef Next () {
          _edge._sv = &(_cursor1->second);
          _edge._tv = &(_cursor2->second);
          _edge._value = &(_owner->_edgesMatrix[_cursor1->second._index][_cursor2->second._index]);
          ++_cursor2;
          if (_cursor2 == _end) {
            _cursor2 = _begin;
            ++_cursor1;
          }
          NextEdge();
          return _edge;
        }
        /** Returns a new object which is the exact copy of this object */
        EdgeIteratorImpl* Clone ( ) {
          return ( new EdgeIteratorImpl ( *this ) );
        }
      private:
        void NextEdge() {
          while ((_cursor1 != _end) && (_owner->_edgesMatrix[_cursor1->second._index][_cursor2->second._index] == _owner->NoEdge())) {
            ++_cursor2;
            if (_cursor2 == _end) {
              _cursor2 = _begin;
              ++_cursor1;
            }
          }
        }
        //const DiGraphAdjMatrix* _owner;
        /*        typename std::map<V, Vertex>::iterator _cursor;
                typename std::map<V, Vertex>::iterator _end;*/
        typename std::map<V, VertexImpl>::const_iterator _begin;
        typename std::map<V, VertexImpl>::const_iterator _cursor1;
        typename std::map<V, VertexImpl>::const_iterator _cursor2;
        typename std::map<V, VertexImpl>::const_iterator _end;
        EdgeImpl _edge;
        const DiGraphAdjMatrix* _owner;
    };

    const VertexImpl& AssertVertex(VertexRef v) const {
      if ( v.GetOwner() == this ) {
        return dynamic_cast<const VertexImpl&> ( v );
      } else {
        throw std::invalid_argument ( "Vertex does not belong to this graph." );
      }
    }

    typedef std::map<V, VertexImpl> VerticesMap;
    typedef typename std::map<V, VertexImpl>::iterator VerticesMapIter;

    ////////////////Private data members//////////////////
    /** number of vertices */
    uint32_t _verticesSize;
    /** number of edges */
    uint32_t _edgesSize;
    /** number of max vertices */
    uint32_t _capacity;

    /** the adj matrix */
    std::vector< std::vector<E> > _edgesMatrix;
    VerticesMap _vertices;

};

#endif // DIGRAPHADJMATRIX_H
