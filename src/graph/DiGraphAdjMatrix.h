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
    bool checkAddedVertex; // = true
    //bool const MAINTAIN_VERTICES_ORDER;
//     using IGraph<V, E>::VertexRef;
//     using IGraph<V, E>::VertexIterator;
//     using IGraph<V, E>::EdgeRef;
//     using IGraph<V, E>::EdgeIterator;
    typedef typename IGraph<V, E>::Vertex Vertex;
    typedef typename IGraph<V, E>::Edge Edge;
    typedef typename IGraph<V, E>::VertexPtr VertexPtr;
    typedef typename IGraph<V, E>::VertexRef VertexRef;
    typedef typename IGraph<V, E>::VertexIterator VertexIterator;
    typedef typename IGraph<V, E>::EdgePtr EdgePtr;
    typedef typename IGraph<V, E>::EdgeRef EdgeRef;
    typedef typename IGraph<V, E>::EdgeIterator EdgeIterator;

    using AbstractDirectedGraph<V, E>::EDGE_NONE;
    //using AbstractDirectedGraph<V, E>::AbstractDirectedGraph;
    DiGraphAdjMatrix ( uint32_t capacity = DEFAULT_CAPACITY, E NoEdgeValue = E() ) :
        AbstractDirectedGraph<V, E>(NoEdgeValue),
        _verticesSize ( 0 ), _edgesSize ( 0 ), _capacity ( capacity ), checkAddedVertex ( true ),
        _vertexIndexer(this), _edgeIndexer(this), _edgeDummy(this), 
        _edgesMatrix ( capacity, std::vector<E> ( capacity, NoEdgeValue ) ),
        edgeSourceVertexTransformer(), edgeTargetVertexTransformer() 
    {
      _dirEdgesCount[IN] = std::vector<uint32_t>(capacity);
      _dirEdgesCount[OUT] = std::vector<uint32_t>(capacity);
    }

    ~DiGraphAdjMatrix() {
    }

    uint32_t VerticesSize() const {
      return _verticesSize;
    }

    uint32_t EdgesSize() const {
      return _edgesSize;
    }

    VertexPtr GetVertex ( const V& v ) const {
      VerticesMapConstIter i = _vertices.find (v);
      if ( i != _vertices.end() ) {
        return & ( i->second );
      }
      return 0;
    }

    const E& GetEdgeValue (VertexRef sourceVertex, VertexRef targetVertex) const {
      if ( sourceVertex.GetOwner() == this && targetVertex.GetOwner() == this ) {
        VertexImplRef s = static_cast<VertexImplRef>(sourceVertex);
        VertexImplRef t = static_cast<VertexImplRef>(targetVertex);
        return _edgesMatrix[s._index][t._index];
      } else {
        throw std::invalid_argument ( "Source or target vertex does not belong to this graph." );
      }
    }

    VertexIterator GetVertices() const {
      VertexIteratorImpl* vi = new VertexIteratorImpl ( this );
      return ( VertexIterator ( vi ) );
    }

    EdgeIterator GetEdges() const {
      IIterator<EdgePtr>* ei;
      if (_edgesSize == 0) {
        ei = new EdgeEmptyIteratorImpl();
      } else {
        ei = new EdgeIteratorImpl ( this );
      }
      return ( EdgeIterator ( ei ) );
    }

    //EdgeIterator GetAllEdges ( const IVertex<V, E>& vertex ) const {}
    EdgeIterator GetInEdges (VertexRef vertex) const {
      const VertexImpl& v = AssertVertex( vertex );
      DirEdgeIteratorImpl* ei = new DirEdgeIteratorImpl (this, v, IN);
      return ( EdgeIterator ( ei ) );
    }

    EdgeIterator GetOutEdges (VertexRef vertex) const {
      const VertexImpl& v = AssertVertex( vertex );
      DirEdgeIteratorImpl* ei = new DirEdgeIteratorImpl (this, v, OUT);
      return ( EdgeIterator ( ei ) );
    }

    //VertexIterator GetAllAdjVertices ( const IVertex<V, E>& vertex ) const {}
    VertexIterator GetPredecessor (VertexRef vertex) const {
      const VertexImpl& v = AssertVertex( vertex );
      DirEdgeIteratorImpl* ei = new DirEdgeIteratorImpl (this, v, IN);
      IteratorTransform<EdgePtr, VertexPtr>* it = new IteratorTransform<EdgePtr, VertexPtr>(ei, edgeSourceVertexTransformer);
      return ( VertexIterator ( it ) );
    }
    VertexIterator GetSuccessor (VertexRef vertex) const {
//       VertexIterator* vi = new VertexIterator(const_cast<DiGraphAdjMatrix*>(this));
//       return (Iterator< IVertex<V, E>& >(vi));
      //VertexIteratorImpl* vi = new EdgeIteratorImpl ( this );
      //return VertexIterator(avi);
    }

    uint32_t InDegree(VertexRef vertex) const {
      VertexImplRef v = AssertVertex( vertex );
      return _dirEdgesCount[IN][v._index];
//       uint32_t c = 0;
//       VertexImplRef v = AssertVertex( vertex );
//       for (int i=0; i<_verticesSize; ++i) {
//         if (_edgesMatrix[i][v._index] != EDGE_NONE) {
//           ++c;
//         }
//       }
//       return c;
    }
    
    uint32_t OutDegree(VertexRef vertex) const {
      VertexImplRef v = AssertVertex( vertex );
      return _dirEdgesCount[OUT][v._index];
//       uint32_t c = 0;
//       VertexImplRef v = AssertVertex( vertex );
//       for (int i=0; i<_verticesSize; ++i) {
//         if (_edgesMatrix[v._index][i] != EDGE_NONE) {
//           ++c;
//         }
//       }
//       return c;
    }

    const Indexer<VertexPtr>& GetVertexIndexer() const {
      return _vertexIndexer;
    }
    const Indexer<EdgePtr>& GetEdgeIndexer() const {
      return _edgeIndexer;
    }

//////////////////////-------MUTABLE OPERATIONS----------///////////////

    VertexPtr AddVertex (const V& v) {
      if (_verticesSize == _capacity) { //need resize
        //_vertices.resize();
        _capacity = _capacity * 2;
        //const E& edgeNone = EDGE_NONE;
        for (int i = 0; i < _edgesMatrix.size(); ++i) {
          _edgesMatrix[i].resize(_capacity, EDGE_NONE);
        }
        _edgesMatrix.resize(_capacity, std::vector<E>(_capacity, EDGE_NONE));
        _dirEdgesCount[IN].resize(_capacity);
        _dirEdgesCount[OUT].resize(_capacity);
      }
      //typename std::pair<typename std::map<V, VertexImpl>::iterator, bool> result = _vertices.insert ( std::pair<V, VertexImpl> ( v, VertexImpl ( this, _verticesSize, v ) ) );
      typename std::pair<VerticesMapIter, bool> result = _vertices.insert ( std::pair<V, VertexImpl> ( v, VertexImpl ( this, _verticesSize, v ) ) );
      VertexPtr retptr = 0;
      if (result.second == true) {
        ++_verticesSize;
        _lastVertexIter = result.first;
        result.first->second._mapiter = result.first;
        retptr = &(result.first->second);
      }
      return retptr;
    }

    EdgePtr AddEdge (VertexRef sourceVertex, VertexRef targetVertex, const E& edge ) {
      EdgePtr ep = 0;
      VertexImplRef s = AssertVertex( sourceVertex );
      VertexImplRef t = AssertVertex( targetVertex );
      E& e = _edgesMatrix[s._index][t._index];
      if ( e == EDGE_NONE && e != edge) {
        _edgesMatrix[s._index][t._index] = edge;
        _edgeDummy._sv = &s; //VertexImplPtr
        _edgeDummy._tv = &t;
        _edgeDummy._value = &e;
        ep = &_edgeDummy;
        ++_dirEdgesCount[IN][t._index];
        ++_dirEdgesCount[OUT][s._index];
        ++_edgesSize;
      }
      return ep;
    }

    virtual void Clear() {
      RemoveAllEdges();
      _vertices.clear();
      _verticesSize = 0;
    };

    virtual void RemoveAllEdges() {
      for ( int i = 0; i < _verticesSize; ++i ) {
        _dirEdgesCount[IN][i] = 0;
        _dirEdgesCount[OUT][i] = 0;
        for ( int j = 0; j < _verticesSize; ++j ) {
          _edgesMatrix[i][j] = EDGE_NONE;
        }
      }
      _edgesSize = 0;
    }

    virtual bool RemoveEdge (EdgeRef edge) {
      return RemoveEdges(*edge.GetSourceVertex(), *edge.GetTargetVertex());
    }
    /** Not implemented because parallel edge is not supported */
    virtual bool RemoveEdge (VertexRef sourceVertex, VertexRef targetVertex, const E& e ) {
      return RemoveEdges(sourceVertex, targetVertex);
    }
    virtual bool RemoveEdges (VertexRef sourceVertex, VertexRef targetVertex) {
      VertexImplRef s = AssertVertex( sourceVertex );
      VertexImplRef t = AssertVertex( targetVertex );
      return RemoveEdge(s._index, t._index);
    }
    
    /** Removes all edges connected to or from the specified vertex. */
    virtual bool removeEdges(VertexRef vertex) {
      VertexImplRef v = AssertVertex( vertex );
      //ASSERT(vertexId >= 0 && vertexId < verticesCount) throw new IndexOutOfBoundsException();
      if (_dirEdgesCount[IN][v._index]==0 && _dirEdgesCount[OUT][v._index]==0) return false;
      for (int i=0; i<_verticesSize; ++i) {
        RemoveEdge(v._index, i);
        RemoveEdge(i, v._index);
      }
      return true;
    }
    /** This method will remove the row and col from the adj matric by swapping it with the element on last index, O(|V|)*/
    virtual bool RemoveVertex (VertexRef vertex) {
      VertexImplRef v = AssertVertex(vertex);
      VertexImplRef lv = _lastVertexIter->second;
      uint32_t id = v._index;
      uint32_t lid = _verticesSize - 1; //lv._index;
      removeEdges(vertex);
      if (id<lid) {
        for (int i=0; i < id; ++i) {
          _edgesMatrix[id][i] = _edgesMatrix[lid][i];
          _edgesMatrix[i][id] = _edgesMatrix[i][lid];
          _edgesMatrix[lid][i] = EDGE_NONE;
          _edgesMatrix[i][lid] = EDGE_NONE;
        }
        _edgesMatrix[id][id] = _edgesMatrix[lid][lid];
        _edgesMatrix[lid][lid] = EDGE_NONE;
        for (int i=id+1; i < lid; ++i) {
          _edgesMatrix[id][i] = _edgesMatrix[lid][i];
          _edgesMatrix[i][id] = _edgesMatrix[i][lid];
          _edgesMatrix[lid][i] = EDGE_NONE;
          _edgesMatrix[i][lid] = EDGE_NONE;
        }
      }
      _vertices.erase(v._mapiter);
      --_verticesSize;
      return true;
    }

  private:

    class VertexImpl;
    friend class DiGraphAdjMatrix::VertexImpl;
    class VertexImpl : public IVertex<V, E> {
      public:
        typedef typename DiGraphAdjMatrix<V, E>::VerticesMapIter VerticesMapIter;
        VertexImpl (DiGraphAdjMatrix* const owner, uint32_t index, const V& value ) : _owner ( owner ), _index ( index ), _value ( value ), _mapiter() { }
        ~VertexImpl() { }
        const V& GetValue () const { return _value; }
        IGraph<V, E>* GetOwner () const { return _owner; }
        VertexImpl* Clone() const { return new VertexImpl(*this); }
        bool Equals(const IBase& obj) const {
          const VertexImpl& v = _owner->AssertVertex(static_cast<const IVertex<V, E>&>(obj));
          return (_index==v._index && _value==v._value);
        };
        //operator uint32_t() const { return _index;}
      private:
        V _value;
        uint32_t _index;
        VerticesMapIter _mapiter;
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
        VertexPtr GetSourceVertex() const { return _sv; }
        VertexPtr GetTargetVertex() const { return _tv; }
        EdgeImpl* Clone() const { return new EdgeImpl(*this); }
      private:
        const E* _value;
        const VertexImpl* _sv;
        const VertexImpl* _tv;
        const DiGraphAdjMatrix* _owner;
        
        friend class DiGraphAdjMatrix;
        friend class DiGraphAdjMatrix::EdgeIteratorImpl;
        friend class DiGraphAdjMatrix::DirEdgeIteratorImpl;
    };

    class VertexIteratorImpl;
    friend class DiGraphAdjMatrix::VertexIteratorImpl;

    class VertexIteratorImpl : public IIterator<VertexPtr> {
      public:
        typedef typename DiGraphAdjMatrix<V, E>::VerticesMapConstIter VerticesMapConstIter;
        VertexIteratorImpl ( const DiGraphAdjMatrix* owner ) : _cursor ( owner->_vertices.begin() ), _end ( owner->_vertices.end() ) { }
        ~VertexIteratorImpl() { }
        bool HasNext () {
          return ( _cursor != _end );
        }
        VertexPtr Next () {
          VertexPtr v = &_cursor->second;
          ++_cursor;
          return v;
        }
        VertexIteratorImpl* Clone () {
          return ( new VertexIteratorImpl ( *this ) );
        }
      private:
        VerticesMapConstIter _cursor;
        VerticesMapConstIter _end;
    };

    /** An iterator implementation if no edge to iterate in the graph */
    class EdgeEmptyIteratorImpl : public IIterator<EdgePtr> {
      public:
        EdgeEmptyIteratorImpl(){ }
        ~EdgeEmptyIteratorImpl() { }
        bool HasNext () { return false; }
        EdgePtr Next () { return 0; }
        EdgeEmptyIteratorImpl* Clone ( ) { return ( new EdgeEmptyIteratorImpl ( *this ) ); }
    };
    
    class EdgeIteratorImpl;
    friend class DiGraphAdjMatrix::EdgeIteratorImpl;
    class EdgeIteratorImpl : public IIterator<EdgePtr> {
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
        EdgePtr Next () {
          _edge._sv = &(_cursor1->second);
          _edge._tv = &(_cursor2->second);
          _edge._value = &(_owner->_edgesMatrix[_cursor1->second._index][_cursor2->second._index]);
          ++_cursor2;
          if (_cursor2 == _end) {
            _cursor2 = _begin;
            ++_cursor1;
          }
          NextEdge();
          return &_edge;
        }
        /** Returns a new object which is the exact copy of this object */
        EdgeIteratorImpl* Clone ( ) {
          return ( new EdgeIteratorImpl ( *this ) );
        }
      private:
        void NextEdge() {
          while ((_cursor1 != _end) && (_owner->_edgesMatrix[_cursor1->second._index][_cursor2->second._index] == _owner->EDGE_NONE)) {
            ++_cursor2;
            if (_cursor2 == _end) {
              _cursor2 = _begin;
              ++_cursor1;
            }
          }
        }
        typename std::map<V, VertexImpl>::const_iterator _begin;
        typename std::map<V, VertexImpl>::const_iterator _cursor1;
        typename std::map<V, VertexImpl>::const_iterator _cursor2;
        typename std::map<V, VertexImpl>::const_iterator _end;
        EdgeImpl _edge;
        const DiGraphAdjMatrix* _owner;
    };

    class DirEdgeIteratorImpl;
    friend class DiGraphAdjMatrix::DirEdgeIteratorImpl;
    class DirEdgeIteratorImpl : public IIterator<EdgePtr> {
      public:
        typedef typename DiGraphAdjMatrix<V, E>::EdgeDirection EdgeDirection;
        typedef typename DiGraphAdjMatrix<V, E>::VertexImplPtr VertexImplPtr;
        //enum EdgeDirection {IN = 0, OUT = 1};
        DirEdgeIteratorImpl (const DiGraphAdjMatrix* owner, const VertexImpl& vertex, EdgeDirection dir)
            : _begin ( owner->_vertices.begin() ), _end ( owner->_vertices.end() ), _cursor(_begin),
            _owner(owner), _source(vertex), _edge(owner), _dir(dir) {
          Init(&_source);
          NextEdge();
        }
        /** Copy Constructor */
        DirEdgeIteratorImpl ( DirEdgeIteratorImpl* e ) : _begin(e->_begin), _end(e->_end), _cursor(e->_cursor),
            _owner(e->_owner), _source(e->_source), _edge(e->_edge), _dir(e->_dir) {
          Init(&_source);
          NextEdge();
        }
        ~DirEdgeIteratorImpl() { }

        bool HasNext () {
          return ( _cursor != _end );
        }
        EdgePtr Next () {
//           VertexImplPtr row[] = {&(_cursor->second), &_source};
//           VertexImplPtr col[] = {&_source, &(_cursor->second)};
          _rowid[IN] = &_cursor->second;
          _colid[OUT] = &_cursor->second;
          _edge._sv = _rowid[_dir];
          _edge._tv = _colid[_dir];
          _edge._value = &(_owner->_edgesMatrix[_rowid[_dir]->_index][_colid[_dir]->_index]);
//           _edge._sv = &_source;
//           _edge._tv = &(_cursor->second);
//          _edge._value = &(_owner->_edgesMatrix[_source._index][_cursor->second._index]);
          ++_cursor;
          NextEdge();
          return &_edge;
        }
        /** Returns a new object which is the exact copy of this object */
        DirEdgeIteratorImpl* Clone ( ) {
          return ( new DirEdgeIteratorImpl ( *this ) );
        }
      private:
        void Init(VertexImplPtr v) {
          _colid[IN] = v;
          _rowid[OUT] = v;
        }
        void NextEdge() {
          _rowid[IN] = &_cursor->second;
          _colid[OUT] = &_cursor->second;
          while ((_cursor != _end) && (_owner->_edgesMatrix[_rowid[_dir]->_index][_colid[_dir]->_index] == _owner->EDGE_NONE)) {
            ++_cursor;
          }
        }
        typename std::map<V, VertexImpl>::const_iterator _begin;
        typename std::map<V, VertexImpl>::const_iterator _end;
        typename std::map<V, VertexImpl>::const_iterator _cursor;
        const DiGraphAdjMatrix* _owner;
        const VertexImpl& _source;
        EdgeImpl _edge;
        VertexImplPtr _rowid[2];
        VertexImplPtr _colid[2];
        EdgeDirection _dir;
    };


    class VertexIndexer : public Indexer<VertexPtr> {
      public:
        VertexIndexer(const DiGraphAdjMatrix* g) : _owner(g) {}
        uint32_t operator()(const VertexPtr& t) const {
          const VertexImpl& v = _owner->AssertVertex(*t);
          return v._index;
        }
        uint32_t GetLastIndex() const {
          return (_owner->_verticesSize)-1;
        }
        const DiGraphAdjMatrix* _owner;
    };
    const VertexIndexer _vertexIndexer;

    class EdgeIndexer : public Indexer<EdgePtr> {
      public:
        EdgeIndexer(const DiGraphAdjMatrix* g) : _owner(g) {}
        uint32_t operator()(const EdgePtr& t) const {
          const VertexImpl& v1 = _owner->AssertVertex(*(t->GetSourceVertex()));
          const VertexImpl& v2 = _owner->AssertVertex(*(t->GetTargetVertex()));
          return (v1._index * _owner->_verticesSize) + v2._index;
        }
        uint32_t GetLastIndex() const {
          return ((_owner->_verticesSize) * (_owner->_verticesSize))-1;
        }
        const DiGraphAdjMatrix* _owner;
    };
    const EdgeIndexer _edgeIndexer;

    class EdgeSourceVertexTransformer : public ITransformer<EdgePtr, VertexPtr> {
      public:
        VertexPtr operator()(const EdgePtr& t1) const {
          return t1->GetSourceVertex();
        }
    };
    const EdgeSourceVertexTransformer edgeSourceVertexTransformer;

    class EdgeTargetVertexTransformer : public ITransformer<EdgePtr, VertexPtr> {
      public:
        VertexPtr operator()(const EdgePtr& t1) const {
          return t1->GetTargetVertex();
        }
    };
    const EdgeTargetVertexTransformer edgeTargetVertexTransformer;

    const VertexImpl& AssertVertex(VertexRef v) const {
      if (v.GetOwner() == this ) {
        return static_cast<VertexImplRef>(v);
      } else {
        throw std::invalid_argument ("Vertex does not belong to this graph." );
      }
    }
    const VertexImpl& AssertVertex(VertexPtr v) const {
      if (v != 0 && v.GetOwner() == this ) {
        return *(static_cast<VertexImplPtr>(v));
      } else {
        throw std::invalid_argument ("Null pointer or vertex does not belong to this graph." );
      }
    }
    
    inline bool EdgeExist(uint32_t i, uint32_t j) {
        return (_edgesMatrix[i][j] != EDGE_NONE);
    }
    /** make sure that the index i and j is valid */
    inline bool RemoveEdge(uint32_t i, uint32_t j) {
        if (_edgesMatrix[i][j] != EDGE_NONE) {
          _edgesMatrix[i][j] = EDGE_NONE;
          --_dirEdgesCount[OUT][i];
          --_dirEdgesCount[IN][j];
          --_edgesSize;
          return true;
        }
    }

    typedef const VertexImpl* VertexImplPtr;
    typedef const VertexImpl& VertexImplRef;
    typedef std::map<V, VertexImpl> VerticesMap;
    typedef typename std::map<V, VertexImpl>::iterator VerticesMapIter;
    typedef typename std::map<V, VertexImpl>::const_iterator VerticesMapConstIter;

    ////////////////Private data members//////////////////
    /** number of vertices */
    uint32_t _verticesSize;
    /** number of edges */
    uint32_t _edgesSize;
    /** number of max vertices */
    uint32_t _capacity;
    EdgeImpl _edgeDummy;

    /// STORAGE
    /** the adj matrix */
    std::vector<uint32_t>  _dirEdgesCount[2];
    std::vector< std::vector<E> > _edgesMatrix;
    VerticesMap _vertices;
    VerticesMapIter _lastVertexIter;

    enum EdgeDirection {IN = 0, OUT = 1};

};

#endif // DIGRAPHADJMATRIX_H
