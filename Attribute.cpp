//
//  Attribute.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include <iostream>
#include "Attribute.hpp"

namespace ECE141 {

  Attribute::Attribute(DataTypes aType)
    : type(aType),size(0),autoIncrement(false),primary(false),nullable(true) {}
 
  Attribute::Attribute(std::string aName, DataTypes aType, uint32_t aSize)  {
    name=aName;
    type=aType;
    size=aSize;
    autoIncrement=false;
    primary=false;
    nullable=true;
  }
 
  Attribute::Attribute(const Attribute &aCopy)  {
    name=aCopy.name;
    type=aCopy.type;
    size=aCopy.size;
    autoIncrement=aCopy.autoIncrement;
    primary=aCopy.primary;
    nullable=aCopy.nullable;
  }
 
  Attribute::~Attribute()  {
  }
 
  Attribute& Attribute::setName(std::string &aName)  {
    this->name = aName;
    return *this;
  }
 
  Attribute& Attribute::setDataType(DataTypes aType) {
    this->type=aType;
    return *this;
  }

  Attribute& Attribute::setSize(int aSize) {
    this->size=aSize; return *this;
  }

  Attribute& Attribute::setAutoIncrement(bool anAuto) {
    this->autoIncrement=anAuto; return *this;
  }

  Attribute& Attribute::setPrimaryKey(bool aPrimary) {
    this->primary=aPrimary; return *this;
  }

  Attribute& Attribute::setNullable(bool aNullable) {
    this->nullable=aNullable; return *this;
  }

  bool Attribute::isValid() {
    return true;
  }
  
  Value Attribute::toValue(const std::string &aValue) const {
    //might need to do type-casting here...    
    return Value{aValue};
  }

  void Attribute::reset(){
    std::string theName = "";
    this->setName(theName);
    this->setDataType(DataTypes::no_type);
    this->setSize(0);
    this->setAutoIncrement(false);
    this->setNullable(true);
    this->setPrimaryKey(false);
  

  }

  StatusResult Attribute::encode(std::ostream &anOutput){
    anOutput<<" Attribute_Info:"<<" "
            << this->name <<" "<< char(int(type)) <<" "
            << this->size <<" "<< this->autoIncrement <<" "
            << this->primary <<" "<< this->nullable <<" "<<"END";
            return StatusResult(Errors::noError);

  }
  StatusResult Attribute::decode(std::istream &anInput) {

    std::string theAttr;
    char type;
    anInput>>theAttr>>this->name>>type>>this->size>>this->autoIncrement>>this->primary>>this->nullable>>theAttr;
    switch(type){
      case 'I':{
        this->type = DataTypes::int_type;
        break;
      }
      case 'B':{
        this->type = DataTypes::bool_type;
        break;
      }
      case 'F':{
        this->type = DataTypes::float_type;
        break;
      }
      case 'V':{
        this->type = DataTypes::varchar_type;
        break;
      }
      case 'N':{
        this->type = DataTypes::no_type;
        break;
      }
      case 'D':{
        this->type = DataTypes::datetime_type;
        break;
      }
    }
    
    return StatusResult(Errors::noError);

  }

}
