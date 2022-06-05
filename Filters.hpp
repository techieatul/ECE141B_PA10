//
//  Filters.hpp
//  RGAssignment5
//
//  Created by rick gessner on 4/4/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#ifndef Filters_h
#define Filters_h

#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>
#include "Errors.hpp"
#include "Tokenizer.hpp"
#include "BasicTypes.hpp"

namespace ECE141 {
  struct selectField {
    selectField(){}
    selectField(std::string &aTable, std::string aField) : table(aTable), field(aField) {}
    ~selectField(){};
    selectField(const selectField &aCopy):table(aCopy.table), field(aCopy.field){
      *this = aCopy;
    }
    selectField& operator=(const selectField &aCopy){
      table = aCopy.table;
      field = aCopy.field;
      return *this;
    }
    
    std::string table;
    std::string field;
  };
  
  class Row;
  class Entity;
  using ValueSet = std::unordered_set<Value>;
  struct Operand {
    Operand() {}
    Operand(std::string &aName, TokenType aType, Value &aValue, size_t anId=0)
      : ttype(aType), dtype(DataTypes::varchar_type), name(aName), entityName(""),
        value(aValue), entityId(anId) {}
    
    TokenType   ttype; //is it a field, or const (#, string)...
    DataTypes   dtype;
    std::string name;  //attr name
    Value    value;
    ValueSet   valSet;
    size_t      entityId;
    std::string entityName;
  };
  
  //---------------------------------------------------

  struct Expression {
    Operand     lhs;  //id
    Operand     rhs;  //usually a constant; maybe a field...
    Operators   op;   //=     //users.id=books.author_id
    Logical     logic; //and, or, not...
    
    Expression(Operand &aLHSOperand, Operators anOp,
               Operand &aRHSOperand)
      : lhs(aLHSOperand), rhs(aRHSOperand),
        op(anOp), logic(Logical::no_op) {}
    
    Expression(Operand &aLHSOperand, Operators anOp,
               Operand &aRHSOperand,Logical &alogical)
      : lhs(aLHSOperand), rhs(aRHSOperand),
        op(anOp), logic(alogical) {}


    bool operator()(KeyValues &aList);
  };
  
  using Expressions = std::vector<std::unique_ptr<Expression>>;

  //---------------------------------------------------

  class Filters {
  public:
    
    Filters();
    Filters(const Filters &aFilters);
   
    ~Filters();
    
    size_t        getCount() const {return expressions.size();}
    bool          matches(KeyValues &aList) const;
    Filters&      add(Expression *anExpression);
    Expressions&  getExpression(){return expressions;}
    StatusResult  parse(Tokenizer &aTokenizer, Entity &anEntity);
    
  protected:
    Expressions   expressions;
  };
 
}

#endif /* Filters_h */
