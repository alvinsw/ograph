#ifndef BINARY_HEAP_PRIORITY_QUEUE_H
#define BINARY_HEAP_PRIORITY_QUEUE_H

#include <vector>
#include <ext/pb_ds/priority_queue.hpp>
#include "IPriorityQueue.h"

//using namespace __gnu_pbds;
//STATUS: Need more testing
//std::les means pop the biggest value
template <typename K, typename Comparator = std::less<K> >
class BinaryHeapPriorityQueue : virtual public IPriorityQueue<K> {
  public:
    BinaryHeapPriorityQueue(Indexer<K>* indexer = 0) : _pq(), a_it(DEFAULT_CAPACITY), _indexer(0), _defIndexer() {
      if (indexer == 0) {
        _indexer = &_defIndexer;
      }
      if (_indexer->GetLastIndex() > 0) {
        a_it.resize(_indexer->GetLastIndex()+1);
      }
    }
    virtual ~BinaryHeapPriorityQueue() { }

    virtual bool Empty() const { return _pq.empty(); }
    virtual uint32_t Size() const { return _pq.size(); }
    //n log(n)
    virtual const K& Peek() const { return _pq.top(); }
    virtual bool Push(const K& k) {
      uint32_t i = (*_indexer)(k);
      if (i >= a_it.size()) {
        a_it.resize(a_it.size() * 2);
      }
      a_it[i] = _pq.push(k);
    }
    virtual bool Pop() {
      if (Empty()) {
        return false;
      } else {
        uint32_t i = (*_indexer)(_pq.top());
        a_it[i] = 0;
        _pq.pop();
        return true;
      }
    }
    virtual K Poll() {
      K k = _pq.top();
      uint32_t i = (*_indexer)(k);
      a_it[i] = 0;
      _pq.pop();
      return k;
    }
    virtual bool Update(const K& k) {
      uint32_t i = (*_indexer)(k);
      if (a_it[i] == 0) return false;
      _pq.modify(a_it[i], k);
      std::cout << "modified"<< std::endl;
      return true;
    }
    virtual void Clear() {
      _pq.clear();
    }
    
  private:
    typedef __gnu_pbds::priority_queue<K, Comparator, __gnu_pbds::binary_heap_tag> t_pq;
    //typedef __gnu_pbds::priority_queue<K, Comparator> t_pq;
    static int const DEFAULT_CAPACITY = 16;
    
    t_pq _pq;
    std::vector<typename t_pq::point_iterator> a_it;
    Indexer<K>* _indexer;
    AnyIndexer<K> _defIndexer;
};


#endif // BINARY_HEAP_PRIORITY_QUEUE_H

