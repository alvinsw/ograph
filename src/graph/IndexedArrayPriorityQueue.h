#ifndef INDEXED_ARRAY_PRIORITY_QUEUE_H
#define INDEXED_ARRAY_PRIORITY_QUEUE_H

#include <vector>
#include <cassert>
#include "IPriorityQueue.h"

//Do not use IndexedArrayPriorityQueueNoStorage with value-type data type such as int, float, char, etc
// template <typename T>
// struct IndexedArrayPriorityQueueNoStorage {
//   explicit IndexedArrayPriorityQueueNoStorage(uint32_t s = 0) {}
//   inline void SetSize(uint32_t s) {}
//   inline void Set(uint32_t s, const T& t) { }
//   inline const T& Get(uint32_t s, const Indexer<const T&>& indexer) const { 
//     return indexer(s);
//   }
// };

template <typename T>
struct IndexedArrayPriorityQueueVectorStorage {
  explicit IndexedArrayPriorityQueueVectorStorage(uint32_t s = 0) : _elements(s) {}
  inline void SetSize(uint32_t s) {
    _elements.resize(s);
  }
  inline void Set(uint32_t s, const T& t) {
    _elements[s] = t; 
  }
  inline const T& Get(uint32_t s, const Indexer<T>& indexer) const {
    return _elements[s];
  }
  std::vector<T> _elements;
};

template <typename K, typename Comparator = std::less<K>, typename OwnStorage=IndexedArrayPriorityQueueVectorStorage<K> >
class IndexedArrayPriorityQueue : virtual public IPriorityQueue<K> {
  public:
    struct Parameters {
      Parameters(const Indexer<K>& i) : indexer(&i), comp() {
        capacity = i.GetLastIndex()+1;
      }
      const Indexer<K>* indexer;
      Comparator comp;
      uint32_t capacity;
    };

    /** transform K to int*/
//     IndexedArrayPriorityQueue(uint32_t size, const ITransformer<K, uint32_t>& to_int, const Comparator& comp = Comparator()) :
//       _keys(size), _items(size), _flags(size), _fsize(size), _num(0), _lastTop(size), _to_int(to_int), _comp(comp) {
//     }
/*    explicit IndexedArrayPriorityQueueBase(const Parameters& params) :
        _flags(params.capacity), _fsize(params.capacity), _data(),
        _num(0), _lastTop(params.capacity), _indexer(params.indexer), _comp(params.comp) { Init(); }*/
    explicit IndexedArrayPriorityQueue(uint32_t capacity = 0, const Indexer<K>* indexer=0, const Comparator& comp = Comparator()) :
        _flags(capacity), _data(capacity), _fsize(capacity), _num(0), _lastTop(capacity), _indexer(indexer), _comp(comp), _defIndexer() 
    {
      if (_indexer==0) {
        _indexer = &_defIndexer;
      }
    }
    /*    IndexedArrayPriorityQueue(uint32_t capacity, const Transformer& to_int = Transformer(), const Comparator& comp = Comparator()) :
          _keys(capacity), _items(capacity), _flags(capacity), _fsize(capacity), _num(0), _lastTop(capacity), _to_int(to_int), _comp(comp) {
        }*/

    bool Empty() const { return (_num == 0); }

    uint32_t Size() const { return _num; }

    // Complexity: O(n)
    const K& Peek() const {
      assert(!Empty()); //if (Empty()) throw std::out_of_range("Peek an empty queue is not allowed.");
      return findTop();
    }

    bool Pop() {
      if (Empty()) return false;
      findTop(); //this will init _lastTop
      _flags[_lastTop] = false;
      _lastTop = _fsize;
      --_num;
      return true;
    }

    K Poll() {
      assert(!Empty()); // if (Empty()) throw std::out_of_range("queue is empty.");
      const K& k = findTop();
      _flags[_lastTop] = false;
      _lastTop = _fsize;
      --_num;
      return k;
    }
    
    bool Push(const K& k) {
      uint32_t i = (*_indexer)(k);
      assert(i < _fsize); //if (i >= _fsize) throw std::out_of_range("Out of capacity.");
      if (_flags[i]==false) {
        _flags[i] = true;
        _data.Set(i, k);// _elements[i] = k;
        ++_num;
        _lastTop = _fsize;
        return true;
      }
      return false;
    }
    
    bool Update(const K& k) {
      uint32_t i = (*_indexer)(k);
      assert(i < _fsize); //if (i >= _fsize) throw std::out_of_range("Out of capacity.");
      if (_flags[i] == true) {
        _data.Set(i, k);//_elements[i] = k;
        _lastTop = _fsize;
        return true;
      }
      return false;
    }

    void Clear() {
      for (int i=0; i<_fsize; ++i) {
        _flags[i] = false;
      }
      _lastTop = _fsize;
      _num = 0;
    }

    uint32_t Capacity() const {
      return _fsize;
    }
    
    Comparator& GetComparator() {
      return _comp;
    }
  protected:
    /** The caller must guarantee that _num > 0 */
    const K& findTop() const {
      if (_lastTop == _fsize) { // cache _lastTop is not valid
        int tempIndex = 0;
        while (_flags[tempIndex]==false && tempIndex < _fsize) {
          ++tempIndex;
        }
        for (int i=tempIndex+1; i<_fsize; ++i) {
          //std::cout << "i:" << i<< " - " << _data.Get(i, *_indexer) << " - " << _data.Get(tempIndex, *_indexer) << std::endl;
          if (_flags[i] && !_comp(_data.Get(i, *_indexer), _data.Get(tempIndex, *_indexer))) {
            //std::cout << "tempIndex:" << tempIndex << std::endl;
            tempIndex = i;
          }
        }
        _lastTop = tempIndex;
      }
      //std::cout << "_lastTop:" << _lastTop << std::endl;
      return _data.Get(_lastTop, *_indexer);
    }

    std::vector<bool> _flags;
    const uint32_t _fsize;// a fixed size
    uint32_t _num;
    mutable uint32_t _lastTop;
    const Indexer<K>* _indexer;
    Comparator _comp;
    const AnyIndexer<K> _defIndexer;
    OwnStorage _data;
};

// template <typename K, typename Comparator, typename OwnStorage>
// friend std::ostream& operator<< (std::ostream& out, const IndexedArrayPriorityQueue<K, Comparator, OwnStorage>& p) {
//   out << "IndexedArrayPriorityQueue=[" <<  e.GetSourceValue() << "-->" << e.GetTargetValue() << ": " << e.GetValue() << "]";
//   return out;
// }


// template <typename K, bool CreateOwnStorage = true, typename Comparator = std::less<K> >
// class IndexedArrayPriorityQueue { };
// 
// template <typename K, typename Comparator>
// class IndexedArrayPriorityQueue<K, true, Comparator> : public IndexedArrayPriorityQueueBase<K, Comparator> {
//   public:  
//     explicit IndexedArrayPriorityQueue(uint32_t capacity = 0, const Indexer<const K&>* indexer=0, const Comparator& comp = Comparator()) :
//         IndexedArrayPriorityQueueBase<K, Comparator>(capacity, indexer, comp) { }
// };
// 
// template <typename K, typename Comparator>
// class IndexedArrayPriorityQueue<K, false, Comparator> : public IndexedArrayPriorityQueueBase<K, Comparator, IndexedArrayPriorityQueueNoStorage<K> > {
//   public:  
//     explicit IndexedArrayPriorityQueue(uint32_t capacity = 0, const Indexer<const K&>* indexer=0, const Comparator& comp = Comparator()) :
//         IndexedArrayPriorityQueueBase<K, Comparator, IndexedArrayPriorityQueueNoStorage<K> >(capacity, indexer, comp) { }
// };

#endif // INDEXED_ARRAY_PRIORITY_QUEUE_H

