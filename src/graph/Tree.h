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

#ifndef TREE_H
#define TREE_H

#include "IGraph.h"
template <typename V,typename E>
class Tree : public IGraph<V,E> {
  public:
    typedef V VType;
    typedef E EType;
    typedef IVertex<V, E> Vertex;
    typedef IEdge<V, E> Edge;
    typedef const IVertex<V, E>* VertexPtr;
    typedef const IVertex<V, E>& VertexRef;
    typedef Iterator< VertexPtr > VertexIterator;
    typedef const IEdge<V, E>* EdgePtr;
    typedef const IEdge<V, E>& EdgeRef;
    typedef Iterator< EdgePtr > EdgeIterator;
    
    virtual ~Tree() { }
    // Methods inherited from IGraph
    virtual const E& NoEdge() const = 0;
    virtual bool IsDirected() const { return true; }
    virtual bool IsParallel() const { return false; }
    virtual bool IsLoop() const { return false; }
    virtual uint32_t VerticesSize() const = 0;
    virtual uint32_t EdgesSize() const = 0;
    virtual VertexPtr GetVertex (const V& v) const = 0;
    virtual const V& GetVertexValue (VertexRef vertex) const = 0;
    virtual const E& GetEdgeValue (VertexRef sourceVertex, VertexRef targetVertex) const = 0;
    virtual const E& GetEdgeValue (const V& source, const V& target) const = 0; //
    virtual VertexIterator GetVertices() const = 0; //
    virtual EdgeIterator GetEdges() const = 0; //
    virtual EdgeIterator GetInEdges (VertexRef vertex ) const = 0;
    virtual EdgeIterator GetOutEdges (VertexRef vertex ) const = 0;
    virtual VertexIterator GetAdjVertices (VertexRef vertex ) const = 0; // provided for convenient
    virtual VertexIterator GetPredecessor (VertexRef vertex ) const = 0; //in digraph predecessor is the same with successor
    virtual VertexIterator GetSuccessor (VertexRef vertex ) const = 0;
    virtual uint32_t InDegree(VertexRef vertex) const = 0;
    virtual uint32_t OutDegree(VertexRef vertex) const = 0;
    virtual VertexPtr GetOpposite(VertexRef vertex, EdgeRef edge ) const = 0;
    virtual const Indexer<VertexPtr>& GetVertexIndexer() const = 0;
    virtual const Indexer<EdgePtr>& GetEdgeIndexer() const = 0;
    virtual bool Contains(VertexRef vertex) const = 0;
    virtual bool Contains(VertexRef sourceVertex, VertexRef targetVertex) const = 0;
  
    virtual VertexPtr AddChild(VertexRef parentNode, const V& childElement) = 0;
};

#endif // TREE_H
