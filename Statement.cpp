//
//  Statement.cpp
//  Database
//
//  Created by rick gessner on 3/20/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Statement.hpp"
#include "Tokenizer.hpp"

namespace ECE141 {
  
  Statement::Statement(Keywords aStatementType) : stmtType(aStatementType)  {
  }
  
  Statement::Statement(const Statement &aCopy) : stmtType(aCopy.stmtType) {
  }
  
  Statement::~Statement() {
  }
  
  //USE: -------------------
  StatusResult Statement::parse(Tokenizer &aTokenizer) {
    return StatusResult{};
  }
  
  StatusResult Statement::run(std::ostream &aStream) const {
    //return commandable.runStatement(*this);
    return StatusResult();
  }

  // USE: Set DBName
  void Statement::setDBName(std::string aDBName = ""){ Statement::DBName = aDBName;}

  // USE: Get DB Name

  const std::string Statement::getDBName() const {return Statement::DBName;}
  
}
