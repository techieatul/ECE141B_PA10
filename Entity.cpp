//
//  Entity.cpp
//  PA3
//
//  Created by rick gessner on 3/2/22.
//

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <cstring>
#include <string>
#include "Entity.hpp"
#include "BlockIO.hpp"
#include <sstream>
#include <unordered_set>
#include "Helpers.hpp"


namespace ECE141 {

 //STUDENT: Implement this class...

  Entity::Entity(const std::string aName) :
    name(aName),  autoincr(1) {}

  Entity::Entity(const Entity &aCopy) {
    name=aCopy.name;
    autoincr=aCopy.autoincr;
  }

  Entity::~Entity() {
    
  }
 
  Entity& Entity::addAttribute(const Attribute &anAttribute) {
    if(!getAttribute(anAttribute.getName())) {
      attributes.push_back(anAttribute);
    }
    return *this;
  }

  const Attribute* Entity::getAttribute(const std::string &aName) const {
    for(int i = 0; i<this->attributes.size();i++){
      if(attributes.at(i).getName() == aName){
        return &attributes.at(i);
      }

    } 
    return nullptr;
  }
  
  // USE: ask the entity for name of primary key (may not have one...)
  const Attribute* Entity::getPrimaryKey() const {  
    for(int i = 0; i<this->attributes.size();i++){
      if(attributes.at(i).isPrimaryKey()){
        return &attributes.at(i);
      }

    } 
    return nullptr;
  }

  StatusResult encode(std::ostream &anOutput){
    return StatusResult(Errors::noError);
  }


  StatusResult decode(std::istream &anInput){
  
    return StatusResult(Errors::noError);
  }

  Block Entity::getBlock(){
    // Create a block and encode the entity info
    Block theBlock(BlockType::entity_block);
    theBlock.header.type='E';
    strcpy(theBlock.header.theTitle,this->getName().c_str());
    theBlock.header.theTableNameHash = Helpers::hashString(this->getName().c_str());
    theBlock.header.theBlockId = this->getBlockId();
    theBlock.header.theEntityId = this->getAutoIncr();
    theBlock.header.theBlockNum = this->getBlockNum();

    // Encoding payload char array
    std::stringstream theStream;
    theStream << "Attr_num "<<this->attributes.size()<<" \n ";
    
    std::vector<Attribute> theAttr = this->getAttributes();
    for(size_t i = 0; i<theAttr.size();i++){
      theAttr[i].encode(theStream);
      theStream<<"\n";
    }
    // To add code for encoding Row numbers belonging to this entity

    DataRows theRows = this->getDataRows();
    theStream<<"Data:" <<" ";
    for(auto itr: theRows){
       theStream<<itr<<" ";
    }
    theStream<<"END_Data"<<" ";
    // CHANGED FOR INDEX
    // theStream<<"Index"<<" ";
    // theStream<<getIndexBlockNum()<<" "<<"END_Index"<<" ";
    theStream.read(theBlock.payload,ECE141::kPayloadSize);
    return theBlock;
    
  }
  // To decode the Entity Block
  StatusResult Entity::decodeBlock(Block& aBlock){
    int theAttrNum = 0;
    std::string theAttr;
    std::stringstream theStream;
    theStream.write(aBlock.payload,kPayloadSize);
    theStream>>theAttr>>theAttrNum;
    
    Attribute theAttribute;
    for(size_t i =0; i<theAttrNum;i++){
      theAttribute.decode(theStream);
      this->addAttribute(theAttribute);
    }
    // Decode Data Rows and populate theDataBlockNums

    theStream>>theAttr; // theAttr will have Data:
    while(theStream>>theAttr){
      // CHANGED FOR INDEX
      // if(theAttr=="END_Data" || theAttr=="\0" || theAttr=="Index"){
      //   break;
      // }
      if(theAttr=="END_Data" || theAttr=="\0"){
        break;
      }
      uint32_t theNum = Helpers::convertStrToUnint32_t(theAttr);
      this->insertDataRow(theNum);
    }

    // CHANGED FOR INDEX

    // Decode Index Block Number
    
    // theStream>>theAttr; // theAttr will have Index:
    // while(theStream>>theAttr){
    //   if(theAttr=="END_Index" || theAttr=="\0"){
    //     break;
    //   }
    //   uint32_t theNum = Helpers::convertStrToUnint32_t(theAttr);
    //   this->setIndexBlockNum(theNum);
    // }

    // Store blockId and EntityId
    uint32_t theEntityId = aBlock.header.theBlockId;
    uint32_t theAutoIncrId = aBlock.header.theEntityId;
    uint32_t theEntityHashName = aBlock.header.theTableNameHash;
    uint32_t theBlockNum = aBlock.header.theBlockNum;

    this->setBlockId(theEntityId);
    this->setAutoIncr(theAutoIncrId);
    this->setEntityHashString(theEntityHashName);
    this->setBlockNum(theBlockNum);

    return StatusResult(Errors::noError);
  }

  bool Entity::checkDuplicateAttr(){

    std::unordered_set<std::string> theAttrFieldSet;
    for(auto it: this->getAttributes()){
      theAttrFieldSet.insert(it.getName());
    }
    if(theAttrFieldSet.size()!=this->getAttributes().size()){
      return true;
    }

    return false;

  }


}
