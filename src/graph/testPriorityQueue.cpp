#include <iostream>
#include "IndexedArrayPriorityQueue.h"

using namespace std;

struct Key {
  explicit Key(int _id = 0, int _val = 0) : id(_id), val(_val) {}
  int id;
  int val;
  operator uint32_t () const { return id; }
  void swap(Key& k) {}
};
bool operator<(const Key& lhs, const Key& rhs) {
  return lhs.val < rhs.val;
};
bool operator>(const Key& lhs, const Key& rhs) {
  return lhs.val > rhs.val;
};

class KeyIndexer: public Indexer<Key> {
  public:
    KeyIndexer() {}
    uint32_t operator()(const Key& t) const {
      return t.id;
    }
    Key operator()(const uint32_t& t) const {
      return keys[t];
    }
    virtual uint32_t GetLastIndex() const { return 0; }
    vector<Key> keys;
};

template <typename Q = IndexedArrayPriorityQueue<Key> >
class TestNewQueue {
  typedef typename Q::instanceOfPriorityQueue Q_extends_IPriorityQueue;
};

int main() {
  KeyIndexer indexer;
  for (int i=0; i<10; ++i) {
    indexer.keys.push_back(Key(i,i));
  }
  IndexedArrayPriorityQueue<Key, std::greater<Key> > Q(10, &indexer);
  //BinaryHeapPriorityQueue<Key> Q;
  //Q1.Push(1);
  Q.Push(indexer(0));
  Q.Push(indexer(1));
  Q.Push(indexer(2));
  Q.Push(indexer(3));
  Q.Push(indexer(4));
  Q.Push(indexer(5));
  Q.Push(indexer(6));
  indexer.keys[0].val = 10;
  indexer.keys[1].val = 11;
  Q.Update(indexer(0));
  Q.Update(indexer(1));
  //Q.Update(Key(1,100));
  while (!Q.Empty()) {
    cout << "test:" << Q.Poll().id << endl;
  }
  Q.Push(Key(1,10));
  Q.Push(Key(2,20));
  Q.Clear();
  cout << "size:" << Q.Size() << endl;
  while (!Q.Empty()) {
    cout << "test a:" << Q.Poll().id << endl;
  }
  return 0;
}