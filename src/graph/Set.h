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

#ifndef SET_H
#define SET_H

#include <set>
#include <stdint.h>
#include "Iterator.h"

template <typename T>
class Set {
  public:
    virtual bool Contains(const T& t) const = 0;
    virtual uint32_t Size() const = 0;
    virtual void Add(const T& t) = 0;
    virtual Iterator<T> GetIterator() const = 0;
    //virtual const Iterator<T> Iterator() const = 0;
};

/** TreeSet is implemented using std::set 
 * TODO: Comparator is problematic, must be able to reference a same object: find a way to get or set comparator <- solve by not including the comparator in template parameters
 */
template <typename T>
class TreeSet : public Set<T> {
  public:
    explicit TreeSet() : _set() {}
    explicit TreeSet(const Set<T>& set) : _set() {
      Iterator<T> iter = set.GetIterator();
      while (iter.HasNext()) {
        _set.insert(iter.Next());
      }
    }
    explicit TreeSet(const TreeSet<T>& set) : _set(set._set) {}
    
    void Add(const T& t) {
      _set.insert(t);
    }
    virtual bool Contains(const T& t) const {
      return _set.find(t)!=_set.end();
    }
    virtual uint32_t Size() const {
      return _set.size();
    }
/*    virtual const Iterator<T> Iterator() const {
      return ( Iterator<T>(new StlIteratorWrapper<T, std::set<T>::const_iterator>(_set.begin(), _set.end())) );
    }*/
    virtual Iterator<T> GetIterator() const {
      return ( Iterator<T>(new StlIteratorWrapper<T,TSetIter>(_set.begin(), _set.end())) );
    }
    
  private:
    typedef typename std::set<T>::iterator TSetIter;
    std::set<T> _set;
};
#endif // SET_H
