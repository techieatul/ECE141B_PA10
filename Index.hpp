//
//  Index.hpp
//  RGAssignment3
//
//  Created by rick gessner on 4/2/21.
//

#ifndef Index_hpp
#define Index_hpp

#include <stdio.h>
#include <map>
#include <map>
#include <sstream>
#include <functional>
#include "Storage.hpp"
#include "BasicTypes.hpp"
#include "Errors.hpp"
#include "Helpers.hpp"


namespace ECE141 {

  enum class IndexType {intKey=0, strKey};
  
  using IndexVisitor =
    std::function<bool(const IndexKey&, uint32_t)>;
    
  struct Index : public Storable, BlockIterator {
    Index(Storage &aStorage):storage(aStorage){}
    Index(Storage &aStorage,std::string& anEntityName, std::string& aFieldName,IndexType aType=IndexType::intKey,uint32_t aBlockNum=0) : storage(aStorage), type(aType), blockNum(aBlockNum),entityName(anEntityName),fieldName(aFieldName) {
          changed=false;
          entityId=0;
    }

    Index(const Index& aCopy) : storage(aCopy.storage), data(aCopy.data), type(aCopy.type),
          entityName(aCopy.entityName), fieldName(aCopy.fieldName), blockNum(aCopy.blockNum),
        changed(false) {*this = aCopy;}

    Index& operator=(const Index& aCopy) {
      //storage = aCopy.storage;
      fieldName = aCopy.fieldName;
      blockNum = aCopy.blockNum;
      data = aCopy.data;
      type = aCopy.type;
      entityName = aCopy.entityName;
      changed = false;
      return *this;
    }
    
    class ValueProxy {
    public:
      Index     &index;
      IndexKey  key;
      IndexType type;
      
      ValueProxy(Index &anIndex, uint32_t aKey)
        : index(anIndex), key(aKey), type(IndexType::intKey) {}
      
      ValueProxy(Index &anIndex, const std::string &aKey)
        : index(anIndex), key(aKey), type(IndexType::strKey) {}
      
      ValueProxy& operator= (uint32_t aValue) {
        index.setKeyValue(key,aValue);
        return *this;
      }
      
      operator IntOpt() {return index.valueAt(key);}
    }; //value proxy
    
    ValueProxy operator[](const std::string &aKey) {
      return ValueProxy(*this,aKey);
    }

    ValueProxy operator[](uint32_t aKey) {
      return ValueProxy(*this,aKey);
    }
      
    uint32_t getBlockNum() const {return blockNum;}
    Index&   setBlockNum(uint32_t aBlockNum) {
               blockNum=aBlockNum;
               return *this;
             }
    
    bool    isChanged() {return changed;}
    Index&  setChanged(bool aChanged) {
      changed=aChanged; return *this;      
    }
        
    StorageInfo getStorageInfo(size_t aSize) {
      //student complete...
    }
            
    IntOpt valueAt(IndexKey &aKey) {
      return exists(aKey) ? data[aKey] : (IntOpt)(std::nullopt);
    }

    bool setKeyValue(IndexKey &aKey, uint32_t aValue) {
      data[aKey]=aValue;
      return changed=true; //side-effect indended!
    }
        
    StatusResult erase(const std::string &aKey) {
      //student implement
      return StatusResult{Errors::noError};
    }

    StatusResult erase(uint32_t aKey) {
      //student implement
      return StatusResult{Errors::noError};
    }

    size_t getSize() {return data.size();}
    
    bool exists(IndexKey &aKey) {
      return data.count(aKey);
    }
      
    StatusResult encode(std::ostream &anOutput) override {
      anOutput << entityName << " " << int(type) << " " << fieldName << " " << blockNum << " " << data.size() << " ";

      for (auto& theData : data) {
        switch (theData.first.index()) {
        case 0:
          anOutput << "I " << std::get<uint32_t>(theData.first);
          break;
        case 1:
          anOutput << "S " << std::get<std::string>(theData.first);
          break;
        }
        anOutput << ' ' << theData.second << ' ';
      }
      return StatusResult{Errors::noError};
    }

    void getBlock(Block& aBlock){
      aBlock.header.type = 'I';
      aBlock.header.theBlockNum = blockNum;
      aBlock.header.theTableNameHash = Helpers::hashString(entityName.c_str());
      strcpy(aBlock.header.theTitle, entityName.c_str());
      aBlock.header.theBlockId = -1;
      aBlock.header.theEntityId = -1;

      std::stringstream ss;
      StatusResult theStatus = this->encode(ss);
      if(theStatus){ss.read(aBlock.payload, ECE141::kPayloadSize);}
      return;
    }
    
    StatusResult decode(std::istream &anInput) override {

      anInput >> entityName; // retrieve entityName
      std::string theDecodeStr;

      anInput >> theDecodeStr;
      type = IndexType{ std::stoi(theDecodeStr) }; // retrieve IndexType

      anInput >> fieldName; // retrieve fieldName

      anInput >> theDecodeStr;
      blockNum = Helpers::convertStrToUnint32_t(theDecodeStr); // retrieve blockNumber
      
      size_t      theDataSize;
      uint32_t    theIntKey;
      std::string theStrKey;
      char        theType;
      uint32_t    theValue;
      

      anInput >> theDataSize;
      for (size_t i = 0; i < theDataSize; i++) {
        anInput >> theType;
        IndexKey theKey;
        switch (theType) {
        case 'I':
          anInput >> theIntKey >> theValue;
          theKey = theIntKey;
          break;
        case 'S':
          anInput >> theStrKey >> theValue;
          theKey = theStrKey;
          break;
        }
        data[theKey] = theValue;
      }
      return StatusResult{Errors::noError};
    }
    
    //visit blocks associated with index
    bool each(const BlockVisitor& aVisitor) override {
      Block theBlock;
      for(auto thePair : data) {
        if(storage.readBlock(thePair.second, theBlock)) {
          if(!aVisitor(theBlock,thePair.second)) {return false;}
        }
      }
      return true;
    }

    //visit index values (key, value)...
    bool eachKV(IndexVisitor aCall) {
      for(auto thePair : data) {
        if(!aCall(thePair.first,thePair.second)) {
          return false;
        }
      }
      return true;
    }
    
  protected:
    
    Storage                       &storage;
    std::map<IndexKey, uint32_t>  data;
    IndexType                     type;
    std::string                   name;
    bool                          changed;
    uint32_t                      blockNum; //where index storage begins
    uint32_t                      entityId;
    std::string                   entityName;
    std::string                   fieldName;
  }; //index

  using IndexMap = std::map<std::string, std::unique_ptr<Index> >;

}


#endif /* Index_hpp */
