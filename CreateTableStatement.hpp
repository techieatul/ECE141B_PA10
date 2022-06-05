#pragma once
#include <iostream>
#include <string>

#include "SQLStatement.hpp"
#include "Statement.hpp"
#include "SQLProcessor.hpp"
namespace ECE141 {
class SQLProcessor;
class CreateTableStatement : public SQLStatement {
   public:
    CreateTableStatement(SQLProcessor* aSQLProc,Keywords aStatementType = Keywords::unknown_kw) : SQLStatement::SQLStatement(aStatementType),theSQLProcessorPtr(aSQLProc) {}
    ~CreateTableStatement(){};
    // Function to check if tokenized tokens represent CREATE TABLE <TABLE_NAME>;
    static bool checkCreateTable(Tokenizer aTokenizer);
    static Statement* createTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer);
    StatusResult run(std::ostream &aStream) const override;
    StatusResult parse(Tokenizer& aTokenizer) override;

    SQLProcessor* getSQLProcessor() const;

    protected:
    SQLProcessor* theSQLProcessorPtr;





    
};

}  // namespace ECE141