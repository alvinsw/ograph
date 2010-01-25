/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef I_MUTABLEGRAPH_H
#define I_MUTABLEGRAPH_H

#include "IGraph.h"
/**
 * A Graph interface that allows modification to the graph.
 */
template <typename V, typename E>
class IMutableGraph : public IGraph<V, E> {
  public:
    typedef typename IGraph<V, E>::VertexRef VertexRef;
    typedef typename IGraph<V, E>::VertexIterator VertexIterator;
    typedef typename IGraph<V, E>::EdgeRef EdgeRef;
    typedef typename IGraph<V, E>::EdgeIterator EdgeIterator;

    virtual ~IMutableGraph() { };
    virtual void SetNoEdgeValue(const E& e) = 0;
    /** Returns a pointer to Vertex handler, do not delete the object    */
    virtual VertexRef AddVertex(const V& v) = 0;
    virtual bool AddEdge(const V& source, const V& target, const E& edge) = 0;
    virtual bool AddEdge(const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex, const E& edge) = 0;
    /** Clear all vertices and edges */
    virtual void Clear() = 0;
    virtual void RemoveAllEdges() = 0;
    virtual bool RemoveEdge(IEdge<V, E>& edge) = 0;
    /** For graph with parallel edges */
    virtual bool RemoveEdge(const V& source, const V& target, const E& e) = 0;
    /** Remove all the edges from source to target */
    virtual bool RemoveEdges(const V& source, const V& target) = 0;
    /** For graph with parallel edges */
    virtual bool RemoveEdge(const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex, const E& e) = 0;
    /** Remove all the edges from source to target */
    virtual bool RemoveEdges(const IVertex<V, E>& sourceVertex, const IVertex<V, E>& targetVertex) = 0;
    virtual bool RemoveVertex(const V& v) = 0;
    virtual bool RemoveVertex(IVertex<V, E>& vertex) = 0;
    
    //using IGraph<V, E>::GetVertex;

};

#endif // I_MUTABLEGRAPH_H
