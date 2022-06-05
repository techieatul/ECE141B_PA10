#pragma once
#include <list>
#include <unordered_map>

namespace ECE141 {

template <typename KeyT, typename ValueT>
class LRUCache {
   public:
    // OCF
    LRUCache(size_t aSize) {
        maxsize = aSize;
    }
    LRUCache() {}
    ~LRUCache() {}
    void put(const KeyT& key, const ValueT& value) {
        if (theValMap.find(key) != theValMap.end()) {
            touch(theValMap[key]);
            theKeyList.begin()->second = value;
        } else {
            if (theValMap.size() == maxsize) {
                int theKey = theKeyList.back().first;
                theValMap.erase(theKey);
                theKeyList.pop_back();
            }
            theKeyList.push_front({key, value});
            theValMap[key] = theKeyList.begin();
        }
    }
    ValueT& get(const KeyT& key) {
        touch(theValMap[key]);
        return theKeyList.begin()->second;
    }
    bool contains(const KeyT& key) const {
        return theValMap.find(key) != theValMap.end();
    }

    void touch(typename std::list<std::pair<KeyT, ValueT>>::iterator it) {
        KeyT   key = it->first;
        ValueT value = it->second;
        theKeyList.erase(it);
        theKeyList.push_front({key, value});
        theValMap[key] = theKeyList.begin();
        return;
    }

    void setsize(size_t aSize) { maxsize = aSize; };  // current size

    void remove(const KeyT& aKey) {
        theValMap.erase(aKey);
        return;
    }

   protected:
    typedef std::list<std::pair<KeyT, ValueT>>               theKeyValue;
    size_t                                                   maxsize;  // prevent cache from growing past this size...
    theKeyValue                                              theKeyList;
    std::unordered_map<KeyT, typename theKeyValue::iterator> theValMap;
    // data members here...
};
}  // namespace ECE141