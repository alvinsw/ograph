#ifndef DEPTH_FIRST_ITERATOR_H
#define DEPTH_FIRST_ITERATOR_H

#include <list>
#include <cassert>
#include "IGraph.h"

template <typename V, typename E>
class DepthFirstVertexIterator : IIterator<const IVertex<V, E>*> {
  public:
    typedef typename IGraph<V,E>::VertexPtr VertexPtr;
    typedef typename IGraph<V,E>::VertexIterator VertexIterator;
    typedef IndexedArrayMarker<VertexPtr, bool> TMarker;
    
    DepthFirstVertexIterator(const IGraph<V, E>& g, const IVertex<V,E>& start) : _g(g), _s(start), _visited(g.VerticesSize(), &g.GetVertexIndexer()) {
      VertexIterator iter = _g.GetSuccessor(start);
      while (iter.HasNext()) {
        VertexPtr uptr = iter.Next();
        _stack.push_back(uptr);
        _visited.Set(uptr, true);
      }
    }
    virtual ~DepthFirstVertexIterator() {}
    
    virtual bool HasNext() {
      return !_stack.empty();
    }
    
    virtual VertexPtr Next() {
      assert(!_stack.empty());
      VertexPtr vptr = _stack.back();
      _stack.pop_back();
      VertexIterator iter = _g.GetSuccessor(*vptr);
      while (iter.HasNext()) {
        VertexPtr uptr = iter.Next();
        if (!_visited.Get(uptr)) { // if uptr not visited
          _stack.push_back(uptr);
          _visited.Set(uptr, true);
        }
      }
      return vptr;
    }
    
    virtual IIterator<VertexPtr>* Clone() {
      return ( new DepthFirstVertexIterator ( *this ) );
    }
  
  private:
    const IGraph<V, E>& _g;
    const IVertex<V,E>& _s;
    TMarker _visited; // distance marker
    std::list<VertexPtr> _stack;
};

#endif // DEPTH_FIRST_ITERATOR_H
