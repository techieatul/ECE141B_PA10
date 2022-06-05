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
class UpdateTableStatement : public SQLStatement {
   public:
    UpdateTableStatement(SQLProcessor *aProc, Keywords aStatementType, Entity *anEntity)
        : SQLStatement::SQLStatement(aStatementType), entity(anEntity), theSQLProcessorPtr(aProc) {}
    ~UpdateTableStatement(){};

    StatusResult      parse(Tokenizer &aTokenizer) override;
    StatusResult      parseSet(Tokenizer &aTokenizer);
    static bool       checkUpdateTable(Tokenizer aTokenizer);
    static Statement *updateTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
    StatusResult      run(std::ostream &aStream) const override;
    SQLProcessor     *getSQLProcessor() const { return theSQLProcessorPtr; }

   protected:
    Entity       *entity;
    SQLProcessor *theSQLProcessorPtr;
};

}  // namespace ECE141