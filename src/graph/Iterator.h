#ifndef ITERATOR_H
#define ITERATOR_H
#include <iostream>

template <typename T>
class IIterator {
public:
    virtual ~IIterator() { };
    virtual bool HasNext(void) = 0;
    virtual T Next(void) = 0;
    /** Returns a new object which is the exact copy of this object */
    virtual IIterator<T>* Clone(void) = 0;
};

// template<typename T>
// struct IteratorRef
// {
//     IIterator<T>* _iterPtr;
// 
//     explicit
//     IteratorRef(IIterator<T>* __p): _iterPtr(__p) { }
// };

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
        std::cout << "operator= called" << std::endl;
        if (_iterPtr == 0) {
            _iterPtr = iter._iterPtr->Clone();
        } else if (this != &iter) {
            *_iterPtr = *(iter._iterPtr);
        }
        return *this;
    }

    ~Iterator() {
        delete _iterPtr;
//         std::cout << "delete ref" << std::endl;
    }

    void Reset(element_type* __p = 0) throw() {
        if (__p != _iterPtr)
        {
            delete _iterPtr;
            _iterPtr = __p;
        }
    }

    bool HasNext(void) {
        return _iterPtr->HasNext();
    }

    T Next(void) {
        return _iterPtr->Next();
    }



protected:
    element_type* _iterPtr;

};

#endif // ITERATOR_H