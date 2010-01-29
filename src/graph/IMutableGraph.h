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
class IMutableGraph : public virtual IGraph<V, E> {
  public:
    typedef typename IGraph<V, E>::VertexPtr VertexPtr;
    typedef typename IGraph<V, E>::VertexRef VertexRef;
    typedef typename IGraph<V, E>::EdgePtr EdgePtr;
    typedef typename IGraph<V, E>::EdgeRef EdgeRef;

    virtual ~IMutableGraph() { };
    
    virtual void SetNoEdgeValue(const E& e) = 0;
    
    /** Returns a pointer to Vertex handler, do not delete the object    */
    virtual VertexPtr AddVertex(const V& v) = 0;
    
    /** 
      * Returns a pointer to a dummy internal edge object. It is only used as a mean to get the VertexPtr of source and target and the value
      * Copy the edge object values to other variables before calling any other methods that involve returning an edge.
      */
    virtual EdgePtr AddEdge(const V& source, const V& target, const E& edge) = 0;
    virtual EdgePtr AddEdge(VertexRef sourceVertex, VertexRef targetVertex, const E& edge) = 0;
    
    /** Clear all vertices and edges */
    virtual void Clear() = 0;
    virtual void RemoveAllEdges() = 0;
    virtual bool RemoveEdge(EdgeRef edge) = 0;
    
    /** For graph with parallel edges */
    virtual bool RemoveEdge(const V& source, const V& target, const E& e) = 0;
    /** For graph with parallel edges */
    virtual bool RemoveEdge(VertexRef sourceVertex, VertexRef targetVertex, const E& e) = 0;
    
    /** Remove all the edges from source to target */
    virtual bool RemoveEdges(const V& source, const V& target) = 0;
    /** Remove all the edges from source to target */
    virtual bool RemoveEdges(VertexRef sourceVertex, VertexRef targetVertex) = 0;
    
    virtual bool RemoveVertex(const V& v) = 0;
    virtual bool RemoveVertex(VertexRef vertex) = 0;
    
};

#endif // I_MUTABLEGRAPH_H
