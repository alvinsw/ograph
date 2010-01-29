#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <stdint.h>
#include <iostream>

/**
 * tes t
 */

/** transform an object of type T1 to an object of type T2 */
template <typename T1, typename T2>
class ITransformer {
  public:
    virtual ~ITransformer() { }
    typedef int instanceOfTransformer; //for template typechecking
    
    virtual T2 operator()(const T1& t1) const = 0;
    //virtual T2 Transform(const T1& t1) = 0;
};

template <typename T>
class AnyToUInt32Transformer : public ITransformer<T, uint32_t> {
  public:
    ~AnyToUInt32Transformer() {
      std::cout << "test destructor" << std::endl;
    }
    uint32_t operator()(const T& t) const {
      return uint32_t(t);
    }
    //static const AnyToUInt32Transformer<T> instance;
};


#endif // TRANSFORMER_H