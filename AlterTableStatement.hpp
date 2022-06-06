#pragma once
#include <iostream>
#include <string>
#include <vector> 

#include "SQLStatement.hpp"
#include "Statement.hpp"
#include "SQLProcessor.hpp"
namespace ECE141 {
class SQLProcessor;
class AlterTableStatement : public SQLStatement {
   public:
    AlterTableStatement(SQLProcessor* aSQLProc,Keywords aStatementType = Keywords::unknown_kw) : SQLStatement::SQLStatement(aStatementType),theSQLProcessorPtr(aSQLProc) {}
    ~AlterTableStatement(){};
    // Function to check if tokenized tokens represent CREATE TABLE <TABLE_NAME>;
    static bool checkAlterTable(Tokenizer aTokenizer);
    static Statement* alterTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer);
    StatusResult run(std::ostream &aStream) const override;
    StatusResult parse(Tokenizer& aTokenizer) override;

    SQLProcessor* getSQLProcessor() const;
    void setAlterFieldType(const std::string& aFieldName){
        theAlterFieldName = aFieldName;
    }
    std::string getAlterFieldName() const{
        return theAlterFieldName;
    }
    protected:
    SQLProcessor* theSQLProcessorPtr;
    std::string    theAlterFieldName;





    
};

}  // namespace ECE141