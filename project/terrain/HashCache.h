#ifndef HASHCACHE_H
#define HASHCACHE_H

#include <unordered_map>
#include <list>
#include <functional>
#include <iostream>
#include <glm/vec3.hpp>

namespace rmg {

using namespace std;

template<class K>
size_t byteSize(const K& k) { //Instantiate template to update size count
    return 1;
}

template<>
inline size_t byteSize<glm::ivec3>(const glm::ivec3& k) { //Instantiate template to update size count
    return k.z*k.z*4;
}


template <class K, class V>
class HashCache
{
public:
    HashCache(size_t size, function<V(const K&)> prod) : mMap(size/2), mProd(prod), mMaxSize(size), mSize(0) {
    mMap.reserve(size);
    }

    const V& get(const K& key) {
    auto it = mMap.find(key);
    if(it != mMap.end()) {
        return it->second;
    }

    size_t bsize = byteSize(key);
    while(mSize+bsize > mMaxSize) {
        removeEldestEntry();
    }

    mList.push_back(key);
    mSize+=bsize;
    return mMap.emplace(key,mProd(key)).first->second;
    }

private:
    void removeEldestEntry() {
        const K& key = mList.front();
        mMap.erase(key);
        mSize -= byteSize(key);
        mList.pop_front();
    }

    function<V(const K&)> mProd;
    unordered_map<K,V> mMap;
    list<K> mList;
    size_t mSize;
    size_t mMaxSize;
};

}
#endif // HASHCACHE_H
