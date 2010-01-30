#include <iostream>
#include "IPriorityQueue.h"

using namespace std;

struct Key {
  Key(int _id = 0, int _val = 0) : id(_id), val(_val) {}
  int id;
  int val;
  operator uint32_t () const { return id; }
};

template <typename Q = IndexedArrayPriorityQueue<Key, int> >
class TestNewQueue {
  typedef typename Q::instanceOfPriorityQueue Q_extends_IPriorityQueue;
};

int main() {
  IndexedArrayPriorityQueue<Key, int> Q(10);
  Q.Push(Key(1,10), 11);
  Q.Push(Key(2,20), 10);
  Q.Push(Key(3,30), 2);
  Q.Push(Key(4,40), 4);
  Q.Push(Key(5,50), 1);
  Q.Push(Key(6,60), 6);
  Q.Update(Key(5,50), 20);
  while (!Q.Empty()) {
    cout << "test:" << Q.Poll().id << endl;
  }
  Q.Push(Key(1,10), 11);
  Q.Push(Key(2,20), 10);
  Q.Clear();
  cout << "size:" << Q.Size() << endl;
  while (!Q.Empty()) {
    cout << "test a:" << Q.Poll().id << endl;
  }
  return 0;
}