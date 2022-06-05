#pragma once
#include <iostream>

#include "SQLStatement.hpp"
#include "Statement.hpp"
//#include "SQLProcessor.hpp"
#include <string>

#include "Attribute.hpp"

namespace ECE141 {
class SQLProcessor;
class DescribeTableStatement : public SQLStatement {
   public:
    DescribeTableStatement(SQLProcessor* aProc, Keywords aStatementType = Keywords::unknown_kw) : SQLStatement::SQLStatement(aStatementType), theSQLProcessorPtr(aProc) {}
    ~DescribeTableStatement(){};
    // Function to check if tokenized tokens represent DESCRIBE <TABLE_NAME>;
    static bool       checkDescribeTable(Tokenizer aTokenizer);

    static Statement* describeTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer);
    SQLProcessor*     getSQLProcessor() const { return theSQLProcessorPtr; }

    StatusResult      run(std::ostream& aStream) const override;

   protected:
    SQLProcessor* theSQLProcessorPtr;
};

}  // namespace ECE141