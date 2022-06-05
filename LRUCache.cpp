// #include "LRUCache.hpp"

// namespace ECE141 {
// template <typename KeyT, typename ValueT>
// LRUCache<KeyT, ValueT>::LRUCache(size_t aSize) {
//     this->maxsize = aSize;
// }

// template <typename KeyT, typename ValueT>
// ValueT& LRUCache<KeyT, ValueT>::get(const KeyT& key) {
//     // move the key to the front
//     touch(theValMap[key]);
//     return theKeyList.begin()->second;
// }

// template <typename KeyT, typename ValueT>
// bool LRUCache<KeyT, ValueT>::contains(const KeyT& key) const {
//     return theValMap.find(key) != theValMap.end();
// }

// template <typename KeyT, typename ValueT>
// void LRUCache<KeyT, ValueT>::put(const KeyT& key, const ValueT& value) {
//     if (theValMap.find(key) != theValMap.end()) {
//         touch(theValMap[key]);
//         theKeyList.begin()->second = value;
//     } else {
//         if (theValMap.size() == maxsize) {
//             int theKey = theKeyList.back().first;
//             theValMap.erase(theKey);
//             theKeyList.pop_back();
//         }
//         theKeyList.push_front({key, value});
//         theValMap[key] = theKeyList.begin();
//     }
// }

// template <typename KeyT, typename ValueT>
// void touch(typename LRUCache<KeyT, ValueT>::theKeyValue::iterator it) {
//     KeyT   key = it->first;
//     ValueT value = it->second;
//     LRUCache<KeyT, ValueT>::theKeyList.erase(it);
//     LRUCache<KeyT, ValueT>::theKeyList.push_front({key, value});
//     LRUCache<KeyT, ValueT>::theValMap[key] = LRUCache<KeyT, ValueT>::theKeyList.begin();
//     return;
// }
// }  // namespace ECE141