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

#ifndef IPATH_H
#define IPATH_H

#include "IGraph.h"


class IPath : public IGraph {
  public:
    virtual ~IPath();
    virtual bool Contains(VertexRef sourceVertex, VertexRef targetVertex) const {return false;}
    virtual bool Contains(VertexRef vertex) const {return false;}
    virtual const Indexer< EdgePtr >& GetEdgeIndexer() const;
    virtual const Indexer< VertexPtr >& GetVertexIndexer() const;
    virtual VertexPtr GetOpposite(VertexRef vertex, EdgeRef edge);
    virtual uint32_t OutDegree(VertexRef vertex) const;
    virtual uint32_t InDegree(VertexRef vertex) const;
    virtual VertexIterator GetSuccessor(VertexRef vertex) const;
    virtual VertexIterator GetPredecessor(VertexRef vertex) const;
    virtual VertexIterator GetAdjVertices(VertexRef vertex) const;
    virtual EdgeIterator GetOutEdges(VertexRef vertex) const;
    virtual EdgeIterator GetInEdges(VertexRef vertex) const;
    virtual EdgeIterator GetEdges() const;
    virtual VertexIterator GetVertices() const;
    virtual const E& GetEdgeValue(const V& source, const V& target) const;
    virtual const E& GetEdgeValue(VertexRef sourceVertex, VertexRef targetVertex) const;
    virtual const V& GetVertexValue(VertexRef vertex) const;
    virtual VertexPtr GetVertex(const V& v) const;
    virtual uint32_t EdgesSize() const;
    virtual uint32_t VerticesSize() const;
    virtual bool IsLoop();
    virtual bool IsParallel();
    virtual bool IsDirected();
    virtual const E& NoEdge() const;
};

#endif // IPATH_H
