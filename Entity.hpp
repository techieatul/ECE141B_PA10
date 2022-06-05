//
//  Entity.hpp
//  Assignment3
//
//  Created by rick gessner on 3/18/22.
//  Copyright © 2022 rick gessner. All rights reserved.
//
#pragma once
#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <unordered_set>

#include "Attribute.hpp"
#include "Errors.hpp"
#include "BasicTypes.hpp"
#include "Storage.hpp"

namespace ECE141 {
  
  using AttributeOpt = std::optional<Attribute>;
  using AttributeList = std::vector<Attribute>;
  using DataRows      = std::unordered_set<uint32_t>;
  //------------------------------------------------

  class Entity {
  public:
                          Entity(const std::string aName);
                          Entity(const Entity &aCopy);
    
                          ~Entity();
    
    const std::string&    getName() const {return name;}
        
    const AttributeList&  getAttributes() const {return attributes;}
    Entity&               addAttribute(const Attribute &anAttribute);
    const Attribute*      getAttribute(const std::string &aName) const;
    const Attribute*      getPrimaryKey() const;
    StatusResult          encode(std::ostream &anOutput);
    StatusResult          decode(std::istream &anInput);
    Block                 getBlock();
    StatusResult          decodeBlock(Block& aBlock);
    uint32_t              getIndexBlockNum(){return theIndexBlockNum;}
    bool                  checkDuplicateAttr();
    void                  setAutoIncr(uint32_t &aNum){autoincr = aNum;}
    uint32_t              getAutoIncr(){return autoincr;}
    void                  setBlockId(uint32_t &aNum){theBlockId = aNum;}
    uint32_t              getBlockId(){return theBlockId;}
    DataRows&             getDataRows(){return theDataBlockNums;}
    void                  eraseDataRows(uint32_t& aNum){theDataBlockNums.erase(aNum);}
    void                  insertDataRow(uint32_t &aNum){theDataBlockNums.insert(aNum);}
    uint32_t              getEntityHashString(){return theEntityHashString;}
    void                  setEntityHashString(uint32_t &aHashString){theEntityHashString = aHashString;}
    uint32_t              getBlockNum(){return theBlockNum;}
    void                  setBlockNum(uint32_t &aNum){theBlockNum = aNum;}
    void                  setIndexBlockNum(uint32_t &aIndex){theIndexBlockNum = aIndex;}

  protected:
        
    AttributeList   attributes;
    std::string     name;
    uint32_t        autoincr;  //auto_increment
    uint32_t        theBlockId;
    uint32_t        theBlockNum;
    DataRows        theDataBlockNums;
    uint32_t        theIndexBlockNum;
    uint32_t        theEntityHashString;
    

  };
  
}
#endif /* Entity_hpp */
