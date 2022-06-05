#pragma once
#include <iostream>

#include "SQLStatement.hpp"
#include "Statement.hpp"
//#include "SQLProcessor.hpp"
#include <string>

#include "Attribute.hpp"
namespace ECE141 {
class SQLProcessor;
class DropTableStatement : public SQLStatement {
   public:
    DropTableStatement(SQLProcessor* aProc, Keywords aStatementType = Keywords::unknown_kw) : SQLStatement::SQLStatement(aStatementType), theSQLProcessorPtr(aProc) {}
    ~DropTableStatement(){};

    // Function to check if tokenized tokens represent DROP TABLE <TABLE_NAME>;
    static bool       checkDropTable(Tokenizer aTokenizer);

    static Statement* dropTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer);

    SQLProcessor*     getSQLProcessor() const { return theSQLProcessorPtr; }
    StatusResult      parse(Tokenizer& aTokenizer) override;

    StatusResult      run(std::ostream& aStream) const override;

   protected:
    SQLProcessor* theSQLProcessorPtr;
};

}  // namespace ECE141