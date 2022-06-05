#pragma once
#include <iostream>
#include <string>

#include "Attribute.hpp"
#include "DBQuery.hpp"
#include "SQLStatement.hpp"
#include "Statement.hpp"
//#include "SQLProcessor.hpp"
#include "BlockIO.hpp"
#include "Entity.hpp"
namespace ECE141 {
class SQLProcessor;

class SelectStatement : public SQLStatement {
   public:
    SelectStatement(SQLProcessor *aProc, Keywords aStatementType, Entity *anEntity);
    ~SelectStatement(){};
    StatusResult      parseStatement(Tokenizer &aTokenizer);
    StatusResult      parseSelect(Tokenizer &aTokenizer);
    StatusResult      parseOperand(Tokenizer &aTokenizer, Operand &anOperand);
    static bool       checkSelectTable(Tokenizer aTokenizer);
    static Statement *selectStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
    StatusResult      parse(Tokenizer &aTokenizer) override;
    StatusResult      parseJoin(Tokenizer &aTokenizer);
    Entity*           getEntity(std::string &aName);

    SQLProcessor     *getSQLProcessor() const { return theSQLProcessorPtr; }
    StatusResult      run(std::ostream &aStream) const override;

   protected:
    Entity       *entity;

    SQLProcessor *theSQLProcessorPtr;
};

}  // namespace ECE141