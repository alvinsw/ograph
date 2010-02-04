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

#ifndef IPRIORITYQUEUE_H
#define IPRIORITYQUEUE_H

#include <stdint.h>
#include <vector>
#include <functional>
#include <stdexcept>

#include "Transformer.h"

template <typename K>
class IPriorityQueue {
  public:
    typedef int instanceOfPriorityQueue; //for template typechecking
    
    virtual ~IPriorityQueue() { }

    virtual bool Empty() const = 0;
    virtual uint32_t Size() const = 0;
    /** Retrieves the top element without removing it from this queue */
    virtual const K& Peek() const = 0;
    /** Removes the top element from this queue */
    virtual bool Pop() = 0;
    /** Retrieves the top element and remove it from this queue */
    virtual K Poll() = 0;
    virtual bool Push(const K& k) = 0;
    /** Update the priority of key k, assuming that the underlaying value has been changed */
    virtual bool Update(const K& k) = 0;
    virtual void Clear() = 0;
};

#endif // IPRIORITYQUEUE_H
