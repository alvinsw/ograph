#ifndef TRANSFORMER_H
#define TRANSFORMER_H

/** transform an object of type T1 to an object of type T2 */
template <typename T1, typename T2>
class ITransformer {
  virtual T2 operator()(const T1& t1) = 0;
  //virtual T2 Transform(const T1& t1) = 0;
};

#endif // ITERATOR_H