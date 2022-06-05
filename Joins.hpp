//
//  Join.hpp
//  RGAssignment8
//
//  Created by rick gessner on 5/21/21.
//

#ifndef Join_h
#define Join_h

#include <string>
#include "BasicTypes.hpp"
#include "Errors.hpp"
#include "keywords.hpp"
#include "Row.hpp"

namespace ECE141 {

  class Join  {
  public:
    Join(const std::string &aTable, Keywords aType)
      : table(aTable), joinType(aType), expr() {}
            
    std::string table;
    Keywords    joinType;  //left, right, etc...
    Expression  expr;
  };

  using JoinList = std::vector<Join>;

}

#endif /* Join_h */
