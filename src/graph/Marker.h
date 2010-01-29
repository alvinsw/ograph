#ifndef MARKER_H
#define MARKER_H

#include <vector>
#include "Transformer.h"

template <typename T>
class Indexer;


/** Use to mark the vertex and edge */
template <typename K, typename V>
class IMarker {
  public:
    virtual V Get(const K& key) const = 0;
    virtual void Set(const K& key, const V& value) = 0;
};

template <typename K, typename V>
class IndexedArrayMarker : public IMarker<K, V> {
  public:
    
    struct Parameters {
      Parameters(const Indexer<K>& i) : indexer(&i) {
        capacity = i.GetLastIndex()+1;
      }
      const Indexer<K>* indexer;
      uint32_t capacity;
    };
    
    AnyToUInt32Transformer<K> defaultTransformer;
    
    IndexedArrayMarker(const Parameters& params) : _vals(params.capacity), _to_uint32(params.indexer), _def_val(V()) { }
    /** This object will not delete the transformer */
    IndexedArrayMarker(uint32_t size = 0, const ITransformer<K, uint32_t>* to_uint32 = 0) : _vals(size), _to_uint32(to_uint32), _def_val(V()) { 
      if (to_uint32 == 0) {
        _to_uint32 = &defaultTransformer;
      }
    }
    V Get(const K& key) const {
      uint32_t i = (*_to_uint32)(key);
      if (i >= _vals.size()) return _def_val;
      return _vals[i];
    }
    void Set(const K& key, const V& value) {
      uint32_t i = (*_to_uint32)(key);
      if (i >= _vals.size()) _vals.resize(i+1);
      _vals[i] = value;
    }
  private:
    V _def_val;
    std::vector<V> _vals;
    const ITransformer<K, uint32_t>* _to_uint32;
    
};


#endif // MARKER_H