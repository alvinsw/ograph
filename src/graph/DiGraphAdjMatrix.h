#ifndef DIGRAPHADJMATRIX_H
#define DIGRAPHADJMATRIX_H

#include "AbstractGraph.h"
#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>

/**
 * characteristics: directional, no parallel edge
 */
template <typename V, typename E>
class DiGraphAdjMatrix: public AbstractGraph<V, E> {
public:
    static uint32_t const DEFAULT_CAPACITY = 16;
    //bool const MAINTAIN_VERTICES_ORDER;
    bool checkAddedVertex; // = true

    DiGraphAdjMatrix(uint32_t capacity = DEFAULT_CAPACITY) :
            _verticesSize(0), _edgesSize(0), _capacity(capacity), checkAddedVertex(true), _edgesMatrix(capacity, std::vector<E>(capacity))
    {
    }
    ~DiGraphAdjMatrix() {
        //std::cout << "destructor" << std::endl;
    }

    uint32_t VerticesSize() const {
        return _verticesSize;
    }

    uint32_t EdgesSize() const {
        return _edgesSize;
    }

    const IVertex<V, E>* GetVertex(const V& v) const {
        typename std::map<V, Vertex>::const_iterator i = _vertices.find(v);

        if (i != _vertices.end()) {
            return &(i->second);
        }
        return 0;
    }

    const E& GetEdgeValue(const IVertex<V, E>& source, const IVertex<V, E>& target) const {
        if (source.GetOwner() == this && target.GetOwner() == this) {
            const Vertex& s = dynamic_cast<const Vertex&>( source );
            const Vertex& t = dynamic_cast<const Vertex&>( target );
            return _edgesMatrix[s._index][t._index];
        } else {
            throw std::invalid_argument("Source or target vertex does not belong to this graph.");
        }
    }

    Iterator< IVertex<V, E>& > Vertices(void) const {
        //VertexIterator* vi = new VertexIterator(this);
        VertexIterator* vi = new VertexIterator(const_cast<DiGraphAdjMatrix*>(this));
        return (Iterator< IVertex<V, E>& >(vi));
    }

    Iterator< IVertex<V, E>& > AdjVertices(const IVertex<V, E>& vertex) const {
//       VertexIterator* vi = new VertexIterator(const_cast<DiGraphAdjMatrix*>(this));
//       return (Iterator< IVertex<V, E>& >(vi));
    }

    const IVertex<V, E>& AddVertex(const V& v) {
        typename std::pair<typename std::map<V, Vertex>::iterator, bool> result = _vertices.insert( std::pair<V, Vertex>(v, Vertex(this, _verticesSize, v)) );
        _verticesSize++;
        return result.first->second;
    }

    bool AddEdge(const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex, const E& edge) {
        return false;
    }
    virtual void Clear(void) {};
    virtual bool RemoveEdge(IEdge<V, E>& edge) {
        return false;
    }
    virtual bool RemoveEdge(const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex, const E& e) {
        return false;
    }
    virtual bool RemoveEdges(const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex) {
        return false;
    }
    virtual bool RemoveVertex(IVertex<V, E>& v) {
        return false;
    }


private:
    class Vertex;
    friend class DiGraphAdjMatrix::Vertex;
    class Vertex : public IVertex<V, E> {
    public:
        Vertex(DiGraphAdjMatrix* const owner, uint32_t index, const V& value) : _owner(owner), _index(index), _value(value) { }
        ~Vertex() { }
        const V& GetValue(void) const {
            return _value;
        }
        IGraph<V, E>* GetOwner(void) const {
            return _owner;
        }
    private:
        V _value;
        uint32_t _index;
        DiGraphAdjMatrix* _owner;
        friend class DiGraphAdjMatrix;
    };

    class VertexIterator;
    friend class DiGraphAdjMatrix::VertexIterator;
    class VertexIterator : public IIterator< IVertex<V, E>& > {
    public:
        VertexIterator(DiGraphAdjMatrix* owner) : _cursor(owner->_vertices.begin()), _end(owner->_vertices.end()) { }
        ~VertexIterator() { }
        bool HasNext(void) {
            return (_cursor != _end);
        }
        IVertex<V, E>& Next(void) {
            IVertex<V, E>* v = &_cursor->second;
            ++_cursor;
            return *v;
        }
        /** Returns a new object which is the exact copy of this object */
        IIterator< IVertex<V, E>& >* Clone(void) {
            return (new VertexIterator(*this));
        }
    private:
        //const DiGraphAdjMatrix* _owner;
        typename std::map<V, Vertex>::iterator _cursor;
        typename std::map<V, Vertex>::iterator _end;
    };

    /** number of vertices */
    uint32_t _verticesSize;
    /** number of edges */
    uint32_t _edgesSize;
    /** number of max vertices */
    uint32_t _capacity;

    /** the adj matrix */
    std::vector< std::vector<E> > _edgesMatrix;
    std::map<V, Vertex> _vertices;
    typedef typename std::map<V, Vertex> VerticesMap;
    typedef typename std::map<V, Vertex>::iterator VerticesMapIter;

};
#endif // DIGRAPHADJMATRIX_H
