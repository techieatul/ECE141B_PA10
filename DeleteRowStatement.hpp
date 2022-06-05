#pragma once
#include <iostream>
#include <string>

#include "Attribute.hpp"
#include "Entity.hpp"
#include "Row.hpp"
//#include "SQLProcessor.hpp"
#include "SQLStatement.hpp"
#include "Statement.hpp"

namespace ECE141 {
class SQLProcessor;
class DeleteRowStatement : public SQLStatement {
   public:
    DeleteRowStatement(SQLProcessor* aProc, Keywords aStatementType, Entity* anEntity)
        : SQLStatement::SQLStatement(aStatementType), entity(anEntity), theSQLProcessorPtr(aProc) {}
    ~DeleteRowStatement(){};

    StatusResult      parse(Tokenizer& aTokenizer) override;
    static bool       checkDeleteRow(Tokenizer aTokenizer);
    static Statement* deleteRowStatement(SQLProcessor* aProc, Tokenizer& aTokenizer);
    StatusResult      run(std::ostream& aStream) const override;
    SQLProcessor*     getSQLProcessor() const { return theSQLProcessorPtr; }

   protected:
    Entity*       entity;
    SQLProcessor* theSQLProcessorPtr;
};

}  // namespace ECE141