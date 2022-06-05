#pragma once
#include <iostream>
#include <string>

#include "Attribute.hpp"
#include "BlockIO.hpp"
//#include "Database.hpp"
#include "Entity.hpp"
#include "Row.hpp"

//#include "SQLProcessor.hpp"
#include "SQLStatement.hpp"
#include "Statement.hpp"

namespace ECE141 {
class SQLProcessor;
using RowVectors = std::vector<Row>;
class InsertTableStatement : public SQLStatement {
   public:
    InsertTableStatement(SQLProcessor *anSQLProcessor, Keywords aStatementType, RowVectors *aRowVector, Entity *anEntity)
        : SQLStatement::SQLStatement(aStatementType), rows(aRowVector), entity(anEntity), theSQLProcessorPtr(anSQLProcessor) {}
    ~InsertTableStatement(){};

    StatusResult      parse(Tokenizer &aTokenizer) override;
    bool              createRow(InsertTableStatement               &aStatement,
                                std::map<std::string, std::string> &aKVList);
    bool              makeRowsFromValueLists(Tokenizer  &aTokenizer,
                                             StringList &aFields);
    static bool       checkInsertTable(Tokenizer aTokenizer);
    static Statement *insertTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer);
    StatusResult      run(std::ostream &aStream) const override;
    SQLProcessor     *getSQLProcessor() const { return theSQLProcessorPtr; }

   protected:
    RowVectors   *rows;
    Entity       *entity;
    SQLProcessor *theSQLProcessorPtr;
};

}  // namespace ECE141