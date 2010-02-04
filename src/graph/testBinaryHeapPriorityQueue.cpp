#include <iostream>
#include "BinaryHeapPriorityQueue.h"
#include "PriorityQueue.h"
#include <queue>

using namespace std;

int main() {
  BinaryHeapPriorityQueue<int> pq;
  pq.Push(5);
  pq.Push(1);
  pq.Push(3);
  pq.Push(8);
  pq.Push(4);
  
  while (!pq.Empty()) {
    cout << "poll:" << pq.Poll() << endl;
  }

  cout <<  endl;
  
  priority_queue<int> pq1;
  pq1.push(5);
  pq1.push(1);
  pq1.push(3);
  pq1.push(8);
  pq1.push(4);
  while (!pq1.empty()) {
    cout << "poll:" << pq1.top() << endl;
    pq1.pop();
  }
  
  cout <<  endl;
  
  PriorityQueue<int> pq2;
  pq2.Push(5);
  pq2.Push(1);
  pq2.Push(3);
  pq2.Push(8);
  pq2.Push(4);
  
  while (!pq2.Empty()) {
    cout << "poll:" << pq2.Poll() << endl;
  }
  
}
