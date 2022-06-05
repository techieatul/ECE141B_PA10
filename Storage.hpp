//
//  Storage.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <queue>
#include "BlockIO.hpp"
#include "Errors.hpp"

namespace ECE141 {
using Queue = std::queue<uint32_t>;
struct CreateDB {};  // tags for db-open modes...
struct OpenDB {};
using BlockVisitor = std::function<bool(const Block&, uint32_t)>;

// not required -- but we discussed in class...
class Storable {
   public:
    virtual StatusResult encode(std::ostream &anOutput) = 0;
    virtual StatusResult decode(std::istream &anInput) = 0;
};

struct BlockIterator {
    virtual bool each(const BlockVisitor& aVisitor)=0;
  };



struct StorageInfo {
    StorageInfo(uint32_t anId, size_t aSize, uint32_t aStart=0, BlockType aType=BlockType::data_block)
    : id(anId), size(aSize), start(aStart), type(aType)  {}
    uint32_t  start;
    size_t    size;
    uint32_t  id;
    BlockType type;
  };



// USE: A storage class, might be helpful...
class Storage : public BlockIO {
   public:
    Storage(std::iostream &aStream);
    StatusResult freeBlocks(uint32_t &aBlockCount);
    StatusResult getTables(uint32_t &aBlockCount,std::vector<std::string>& TableVec);
    StatusResult getTableByName(uint32_t &aBlockCount,const std::string &aName,Block &aBlock);
    Block encodeMetaBlock(std::map<std::string,uint32_t>& anIdxMap, uint32_t &anEntityId);
    StatusResult decodeMetaBlock(std::map<std::string,uint32_t>& anIdxMap, uint32_t &anEntityId);
    void         populateFreeBlockQ();
    Queue& getQueue(){return thefreeBlockQ;}
    void setQueue(uint32_t &aBlockNum){thefreeBlockQ.push(aBlockNum);}
   protected:
    Queue thefreeBlockQ;
};

}  // namespace ECE141

#endif /* Storage_hpp */
