#ifndef HEAP_BASED_PRIORITY_QUEUE_H
#define HEAP_BASED_PRIORITY_QUEUE_H

//#include <vector>
#include <ext/pb_ds/priority_queue.hpp>
//#include "IPriorityQueue.h"

//using namespace __gnu_pbds;
//STATUS: Need more testing

template <typename K, typename Comparator = std::less<K>, typename HeapTag=__gnu_pbds::pairing_heap_tag>
class PriorityQueue {
  public:
    typedef __gnu_pbds::priority_queue<K, Comparator, HeapTag> t_pq;
    typedef typename t_pq::point_iterator Iterator;
    typedef typename t_pq::size_type size_t;
    
    PriorityQueue() : _pq() { }
    virtual ~PriorityQueue() { }

    inline bool Empty() const { return _pq.empty(); }
    inline size_t Size() const { return _pq.size(); }
    //n log(n)
    inline const K& Peek() const { return _pq.top(); }
    inline Iterator Push(const K& k) {
      return _pq.push(k);
    }
    inline bool Pop() {
      if (Empty()) {
        return false;
      } else {
        _pq.pop();
        return true;
      }
    }
    inline K Poll() {
      K k = _pq.top();
      _pq.pop();
      return k;
    }
    inline void Update(Iterator i, const K& k) {
      _pq.modify(i, k);
    }
    virtual void Clear() {
      _pq.clear();
    }
    
  private:
    t_pq _pq;
};


#endif // HEAP_BASED_PRIORITY_QUEUE_H

