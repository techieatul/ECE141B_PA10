//
//  Filters.cpp
//  Datatabase5
//
//  Created by rick gessner on 3/5/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#include "Filters.hpp"
#include <string>
#include <limits>
#include <vector>
#include <stack>
#include "keywords.hpp"
#include "Helpers.hpp"
#include "Entity.hpp"
#include "Attribute.hpp"
#include "Compare.hpp"

namespace ECE141 {
  
  using Comparitor = bool (*)(Value &aLHS, Value &aRHS);
  using StackOpr   = void (*)(std::stack<bool> &aStack);
  // To check if LHS==RHS
  bool equals(Value &aLHS, Value &aRHS) {
    bool theResult=false;
    
    std::visit([&](auto const &aLeft) {
      std::visit([&](auto const &aRight) {
        theResult=isEqual(aLeft,aRight);
      },aRHS);
    },aLHS);
    return theResult;
  }
   // To check if LHS>RHS
  bool greater(Value &aLHS, Value &aRHS) {
    bool theResult=false;
    
    std::visit([&](auto const &aLeft) {
      std::visit([&](auto const &aRight) {
        theResult=isGreater(aLeft,aRight);
      },aRHS);
    },aLHS);
    return theResult;
  }

   // To check if LHS>=RHS
  bool greaterEqual(Value &aLHS, Value &aRHS) {
    bool theResult=false;
    
    std::visit([&](auto const &aLeft) {
      std::visit([&](auto const &aRight) {
        theResult=(isGreater(aLeft,aRight) || isEqual(aLeft,aRight));
      },aRHS);
    },aLHS);
    return theResult;
  }
  //  // To check if LHS<RHS
  bool lesser(Value &aLHS, Value &aRHS) {
    bool theResult=false;
    
    std::visit([&](auto const &aLeft) {
      std::visit([&](auto const &aRight) {
        theResult=isLesser(aLeft,aRight);
      },aRHS);
    },aLHS);
    return theResult;
  }
   // To check if LHS<=RHS
  bool lesserEqual(Value &aLHS, Value &aRHS) {
    bool theResult=false;
    
    std::visit([&](auto const &aLeft) {
      std::visit([&](auto const &aRight) {
        theResult=(isLesser(aLeft,aRight) || isEqual(aLeft,aRight));
      },aRHS);
    },aLHS);
    return theResult;
  }
   // To check if LHS!=RHS
  bool notEqual(Value &aLHS, Value &aRHS) {
    bool theResult=false;
    
    std::visit([&](auto const &aLeft) {
      std::visit([&](auto const &aRight) {
        theResult=IsnotEqual(aLeft,aRight);
      },aRHS);
    },aLHS);
    return theResult;
  }
  



   // To do logic for Expression AND Expression
  void logicalAnd(std::stack<bool> &theValStack){
        bool theFirst = theValStack.top();
        theValStack.pop();
        bool theSecond = theValStack.top();
        theValStack.pop();
        bool theRes = (theFirst) && (theSecond);
        theValStack.push(theRes);
    
  }
  // To do logic for Expression OR Expression
  void logicalOr(std::stack<bool> &theValStack){
        bool theFirst = theValStack.top();
        theValStack.pop();
        bool theSecond = theValStack.top();
        theValStack.pop();
        bool theRes = (theFirst) || (theSecond);
        theValStack.push(theRes);
    
  }
  // To do logic for NOT Expression
  void logicalNot(std::stack<bool> &theValStack){
        bool theFirst = theValStack.top();
        theValStack.pop();
        bool theRes = !(theFirst);
        theValStack.push(theRes);
  }
  // To do logic for Expression without any logical operation
  void logicalNoOp(std::stack<bool> &theValStack){
        return;
  }

  
  static std::map<Operators, Comparitor> comparitors {
    {Operators::equal_op, equals},
    {Operators::gt_op, greater},
    {Operators::gte_op, greaterEqual},
    {Operators::lt_op, lesser},
    {Operators::lte_op, lesserEqual},
    {Operators::notequal_op, notEqual},
  };

  static std::map<Logical,StackOpr> stackOpr {
    {Logical::and_op, logicalAnd},
    {Logical::or_op, logicalOr},
    {Logical::not_op, logicalNot},
    {Logical::no_op, logicalNoOp}
  };
  bool Expression::operator()(KeyValues &aList) {
    Value theLHS{lhs.value};
    Value theRHS{rhs.value};

    if(TokenType::identifier==lhs.ttype) {
      theLHS=aList[lhs.name]; //get row value
    }

    if(TokenType::identifier==rhs.ttype) {
      theRHS=aList[rhs.name]; //get row value
    }
    if(TokenType::keyword == rhs.ttype && rhs.value == Value{std::string("in_kw")}){
       ValueSet theValSet = rhs.valSet;
       if(theValSet.find(theLHS)!=theValSet.end()){
         return true;
       }
    }
    return comparitors.count(op)
      ? comparitors[op](theLHS, theRHS) : false;
  }
  
  //--------------------------------------------------------------
  
  Filters::Filters()  {}
  
  Filters::Filters(const Filters &aCopy)  {
  }
  
  
  Filters::~Filters() {
    //no need to delete expressions, they're unique_ptrs!
  }
   

  Filters& Filters::add(Expression *anExpression) {
    expressions.push_back(std::unique_ptr<Expression>(anExpression));
    return *this;
  }
    
  //compare expressions to row; return true if matches
  bool Filters::matches(KeyValues &aList) const {
    
   
    
    std::vector<bool> theValMatch;
    std::vector<Logical> theLogicalOpr; 
    for(auto &theExpr : expressions) {
      bool theResult = (*theExpr)(aList);
      theValMatch.push_back(theResult);
      theLogicalOpr.push_back((*theExpr).logic);
    }
    if(theValMatch.size()==1 && theLogicalOpr[0]==Logical::no_op){
      return theValMatch[0];
    }

    // if there are more vector elements then that mean we also must have logical operators
    // push all elements to stack
    std::stack<bool> theValStack;
    for (auto it = theValMatch.rbegin(); it != theValMatch.rend(); ++it) {
      theValStack.push(*it);
    }

    // Do the logic here
    for(const auto &it: theLogicalOpr){
      if(stackOpr.count(it)) {
        stackOpr[it](theValStack);
      }else{
        return false;
      }

      
    }
    bool theFinalRes = theValStack.top();
    theValStack.pop();
    return theFinalRes;
  }
 

  //where operand is field, number, string...
  StatusResult parseOperand(Tokenizer &aTokenizer,
                            Entity &anEntity, Operand &anOperand) {
    StatusResult theResult{noError};
    std::string theEntityName = anEntity.getName();
    Token &theToken = aTokenizer.current();
    if(TokenType::identifier==theToken.type) {
      if(auto *theAttr=anEntity.getAttribute(theToken.data)) {
        anOperand.ttype=theToken.type;
        anOperand.name=theToken.data; //hang on to name...
        anOperand.entityId=Helpers::hashString(theEntityName.c_str());
        anOperand.dtype=theAttr->getType();
        anOperand.entityName = theEntityName;
      }
      else {
        anOperand.ttype=TokenType::string;
        anOperand.dtype=DataTypes::varchar_type;
        anOperand.value=theToken.data;
      }
    }
    else if(TokenType::number==theToken.type) {
      anOperand.ttype=TokenType::number;
      anOperand.dtype=DataTypes::int_type;
      anOperand.entityId=Helpers::hashString(theEntityName.c_str());
      anOperand.entityName = theEntityName;
      if (theToken.data.find('.')!=std::string::npos) {
        anOperand.dtype=DataTypes::float_type;
        anOperand.value=std::stof(theToken.data);
      }
      else anOperand.value=std::stoi(theToken.data);
    }else if(TokenType::punctuation==theToken.type && theToken.data=="("){
      Token theField = aTokenizer.peek(-2);
      const Attribute* theAttr = anEntity.getAttribute(theField.data);
      const DataTypes theFieldType = theAttr->getType();
      if(theAttr){
        aTokenizer.next(); // In value
        while(aTokenizer.current().data!=")"){
          Value theOprVal = Helpers::convertStrToVal(aTokenizer.current().data,theFieldType);
          anOperand.valSet.insert(theOprVal);
          aTokenizer.next();
          if(aTokenizer.current().data == ","){
            aTokenizer.next();
          }
         }
         anOperand.dtype = theFieldType;
         anOperand.ttype = TokenType::keyword;
         anOperand.entityName = anEntity.getName();
         anOperand.entityId = anEntity.getEntityHashString();
         anOperand.value = std::string("in_kw");

      }else{
        theResult.error=syntaxError;
      }
      
    }
    else theResult.error=syntaxError;
    if(theResult) aTokenizer.next();
    return theResult;
  }
    
  //STUDENT: Add validation here...
  bool validateOperands(Operand &aLHS, Operand &aRHS, Entity &anEntity) {
    if(TokenType::identifier==aLHS.ttype) { //most common case...
      //STUDENT: Add code for validation as necessary
      return true;
    }
    else if(TokenType::identifier==aRHS.ttype) {
      //STUDENT: Add code for validation as necessary
      return true;
    }
    return false;
  }
  StatusResult isLogicalKeyword(Keywords &aKeyWord){
    if(aKeyWord == Keywords::and_kw || aKeyWord == Keywords::and_kw || aKeyWord == Keywords::not_kw){
      return StatusResult(Errors::noError);
    }
    return StatusResult(Errors::unknownError);
  }
  //STUDENT: This starting point code may need adaptation...
  StatusResult Filters::parse(Tokenizer &aTokenizer,Entity &anEntity) {
    StatusResult  theResult{noError};

    while(theResult && (2<aTokenizer.remaining())) {
      Operand theLHS,theRHS;
      Token &theToken=aTokenizer.current();
      if(theToken.type!=TokenType::identifier) return theResult;
      if((theResult=parseOperand(aTokenizer,anEntity,theLHS))) {
        Token &theToken=aTokenizer.current();
        if(theToken.type==TokenType::operators || (theToken.keyword == Keywords::in_kw)) {
          Operators theOp=Helpers::toOperator(theToken.data);
          aTokenizer.next();
          if((theResult=parseOperand(aTokenizer,anEntity,theRHS))) {
            if(validateOperands(theLHS, theRHS, anEntity)) {
              // Added code to check if we have any logical operator
             if(isLogicalKeyword(aTokenizer.current().keyword)){
               Logical theLogicalOpr = Helpers::convertKeyWordToLogical(aTokenizer.current().keyword);
               aTokenizer.next();
               add(new Expression(theLHS, theOp, theRHS,theLogicalOpr));
             }else{
               add(new Expression(theLHS, theOp, theRHS));
             }
              
              if(aTokenizer.skipIf(semicolon)) {
                break;
              }
            }
            else theResult.error=syntaxError;
          }
        }
      }
      else theResult.error=syntaxError;
    }
    return theResult;
  }

}