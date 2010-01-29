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
#include <queue>
#include <functional>
#include <stdexcept>

#include "Transformer.h"
#include "IGraph.h"

template <typename K, typename V>
class IPriorityQueue {
  public:
    typedef int instanceOfPriorityQueue; //for template typechecking

    virtual bool Empty() const = 0;
    virtual uint32_t Size() const = 0;
    virtual const K& Peek() const = 0;
    virtual bool Push(const K& k, const V& v) =0;
    virtual bool Pop() = 0;
    /** Retrieves the top element and remove it from this queue */
    virtual K Poll() = 0;
    /** Update the key k with a new value v */
    virtual bool Update(const K& k, const V& v) = 0;
    virtual void Clear() = 0;
};

//template <typename K, typename V, typename Transformer = AnyToUInt32Transformer<K>, typename Comparator = std::less<V> >
template <typename K, typename V, typename Comparator = std::less<V> >
class IndexedArrayPriorityQueue : virtual public IPriorityQueue<K, V> {
  public:
    //static AnyToUInt32Transformer<K> anyToUInt32Transformer;

    struct Parameters {
      Parameters(const Indexer<K>& i) : indexer(&i) {
        capacity = i.GetLastIndex()+1;
      }
      const Indexer<K>* indexer;
      Comparator comp;
      uint32_t capacity;
    };

    AnyToUInt32Transformer<K> defaultTransformer;

    /** transform K to int*/
//     IndexedArrayPriorityQueue(uint32_t size, const ITransformer<K, uint32_t>& to_int, const Comparator& comp = Comparator()) :
//       _keys(size), _items(size), _flags(size), _fsize(size), _num(0), _lastTop(size), _to_int(to_int), _comp(comp) {
//     }
    IndexedArrayPriorityQueue() : _keys(0), _items(0), _flags(0), _fsize(0), _num(0), _lastTop(0), _to_int(0), _comp(){}
    explicit IndexedArrayPriorityQueue(const Parameters& params) :
        _keys(params.capacity), _items(params.capacity), _flags(params.capacity), _fsize(params.capacity),
        _num(0), _lastTop(params.capacity), _to_int(params.indexer), _comp(params.comp) {
    }
    IndexedArrayPriorityQueue(uint32_t capacity, const ITransformer<K, uint32_t>* to_int=0, const Comparator& comp = Comparator()) :
        _keys(capacity), _items(capacity), _flags(capacity), _fsize(capacity), _num(0), _lastTop(capacity), _to_int(to_int), _comp(comp) {
      if (_to_int==0) {
        _to_int = &defaultTransformer;
      }
    }
    /*    IndexedArrayPriorityQueue(uint32_t capacity, const Transformer& to_int = Transformer(), const Comparator& comp = Comparator()) :
          _keys(capacity), _items(capacity), _flags(capacity), _fsize(capacity), _num(0), _lastTop(capacity), _to_int(to_int), _comp(comp) {
        }*/

    bool Empty() const { return (_num == 0); }

    uint32_t Size() const { return _num; }

    const K& Peek() const {
      if (Empty()) throw std::out_of_range("Peek an empty queue is not allowed.");
      return _keys[findTop()];
    }

    bool Push(const K& k, const V& v) {
      uint32_t i = (*_to_int)(k);
      if (i >= _fsize) throw std::out_of_range("Out of capacity.");
      if (_flags[i]==false) {
        _keys[i] = k;
        _items[i] = v;
        _flags[i] = true;
        ++_num;
        _lastTop = _fsize;
        return true;
      }
      return false;
    }

    bool Pop() {
      if (Empty()) return false;
      _flags[findTop()] = false;
      _lastTop = _fsize;
      --_num;
      return true;
    }

    K Poll() {
      if (Empty()) throw std::out_of_range("queue is empty.");
      int i = findTop();
      _flags[i] = false;
      _lastTop = _fsize;
      --_num;
      return _keys[i];
    }

    bool Update(const K& k, const V& v) {
      uint32_t i = (*_to_int)(k);
      if (i >= _fsize) throw std::out_of_range("Out of capacity.");
      if (_flags[i] == true) {
        _keys[i] = k;
        _items[i] = v;
        _lastTop = _fsize;
        return true;
      }
      return false;
    }

    void Clear() {
      while (Pop()) { };
    }

  private:
    /** The caller must guarantee that _num > 0 */
    uint32_t findTop() const {
      if (_lastTop == _fsize) {
        int tempIndex = 0;
        while (_flags[tempIndex]==false && tempIndex < _fsize) {
          ++tempIndex;
        }
        int tempValue = _items[tempIndex];
        for (int i=tempIndex+1; i<_fsize; ++i) {
          if (_flags[i] && _comp(_items[i], tempValue)) {
            tempValue = _items[i];
            tempIndex = i;
          }
        }
        _lastTop = tempIndex;
      }
      return _lastTop;
    }

    std::vector<K> _keys;
    std::vector<V> _items;
    std::vector<bool> _flags;
    uint32_t _fsize;// a fixed size
    uint32_t _num;
    mutable uint32_t _lastTop;
    //ITransformer<K, uint32_t> _to_int;
    const ITransformer<K, uint32_t>* _to_int;
    //Transformer _to_int;
    Comparator _comp;
};
#endif // IPRIORITYQUEUE_H
