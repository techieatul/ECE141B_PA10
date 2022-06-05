//
//  Row.hpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#ifndef Row_hpp
#define Row_hpp

#include <stdio.h>

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Attribute.hpp"

// feel free to use this, or create your own version...

namespace ECE141 {
class Block;
class Row {
   public:
    // Row(uint32_t entityId = 0);
    Row() {}
    Row(const Row &aRow);
    Row(KeyValues aKeyValueList);
    // Row(const Attribute &anAttribute); //maybe?

    ~Row();

    Row &operator=(const Row &aRow);
    bool operator==(Row &aCopy) const;

    // STUDENT: What other methods do you require?

    Row        &set(const std::string &aKey, const Value &aValue);

    KeyValues  &getData() { return data; }
    KeyValues   getValueData() const { return data; }
    Value       getValData(const std::string &aKey) { return data[aKey]; }
    uint32_t    entityId;  // hash value of entity?
    std::string tableName;
    uint32_t    rowId;
    void        getBlock(Block &aBlock);
    void        encode(Block &aBlock);
    void        decode(Block &aBlock);
    uint32_t    getBlockNumber() { return blockNumber; }
    void        setBlockNumber(uint32_t &theBlockNumber) {
        blockNumber = theBlockNumber;
    }
    uint32_t getStorageBlockNumber() { return storageBlockNum; }
    void     setStorageBlockNumber(uint32_t &theBlockNumber) {
        storageBlockNum = theBlockNumber;
    }
    // uint32_t            blockNumber;

   protected:
    KeyValues data;
    uint32_t  blockNumber;
    uint32_t  storageBlockNum;
};

//-------------------------------------------

using RowCollection = std::vector<std::unique_ptr<Row> >;
using RawRowCollection = std::vector<Row>;

}  // namespace ECE141
#endif /* Row_hpp */
