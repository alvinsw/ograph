#ifndef ITERATOR_H
#define ITERATOR_H
#include <iostream>
#include "Transformer.h"

template <typename T>
class IIterator {
  public:
    virtual ~IIterator() { }
    virtual bool HasNext() = 0;
    virtual T Next() = 0;
    /** Returns a new object which is the exact copy of this object */
    virtual IIterator<T>* Clone() = 0;
};

/** transform from T1 to T2
 *  Will delete the iter object, but NOT the transformer object supplied at the constructor
 */
template <typename T1, typename T2>
class IteratorTransform : public virtual IIterator<T2> {
  public:
    explicit IteratorTransform(IIterator<T1>* iter, ITransformer<T1,T2>& t) : _iter(iter), _t(t) {
    }
    IteratorTransform(const IteratorTransform& rhs) : _iter(rhs._iter->Clone()), _t(rhs._t) {
    }
    virtual ~IteratorTransform() {
      delete _iter;
    }
    virtual bool HasNext() {
      return _iter->HasNext();
    }
    virtual T2 Next() {
      _t(_iter->Next());
    }
    virtual IIterator<T2>* Clone() {
      return ( new IteratorTransform(*this) );
    }
  private:
    IteratorTransform() { }
    
    IIterator<T1>* _iter;
    ITransformer<T1,T2>& _t;
};
// template<typename T>
// struct IteratorRef
// {
//     IIterator<T>* _iterPtr;
//
//     explicit
//     IteratorRef(IIterator<T>* __p): _iterPtr(__p) { }
// };

/** A wrapper class for IIterator object that acts as a smart pointer */
template <typename T>
class Iterator {
  public:
    /// The pointed-to type.
    typedef IIterator<T> element_type;

    explicit
    Iterator(element_type* __p = 0) throw() : _iterPtr(__p) {
//         std::cout << "ctor called" << std::endl;
    }

    Iterator(const Iterator& iter) throw() : _iterPtr(iter._iterPtr->Clone()) {
//         std::cout << "Copy ctor1 called" << std::endl;
    }
//     Iterator(IteratorRef<T> iter) throw() : _iterPtr(iter._iterPtr) {
//         std::cout << "Copy ctor2 called" << std::endl;
//     }
//
//     Iterator& operator=(IteratorRef<T> __ref) throw() {
//         if (__ref._iterPtr != _iterPtr) {
//             delete _iterPtr;
//             _iterPtr = __ref._iterPtr;
//         }
//         return *this;
//     }

    Iterator& operator=(const Iterator& iter) throw() {
      //std::cout << "operator= called" << std::endl;
      if (this != &iter) {
        delete _iterPtr;
        _iterPtr = iter._iterPtr->Clone();
      }
      return *this;
    }

    ~Iterator() {
      delete _iterPtr;
//         std::cout << "delete ref" << std::endl;
    }

    void Reset(element_type* __p = 0) throw() {
      if (__p != _iterPtr) {
        delete _iterPtr;
        _iterPtr = __p;
      }
    }

    bool HasNext() {
      return _iterPtr->HasNext();
    }

    T Next() {
      return _iterPtr->Next();
    }

  private:
    element_type* _iterPtr;

};

#endif // ITERATOR_H