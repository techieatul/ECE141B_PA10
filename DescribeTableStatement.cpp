#include "DescribeTableStatement.hpp"

#include "Database.hpp"
namespace ECE141 {

bool DescribeTableStatement::checkDescribeTable(Tokenizer aTokenizer) {
    Token              theDescribeToken{TokenType::keyword, Keywords::describe_kw,
                           Operators::unknown_op, "describe"};
    Token              theTableNameToken{TokenType::identifier, Keywords::unknown_kw,
                            Operators::unknown_op, "table_name"};
    Token              theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw,
                            Operators::unknown_op, ";"};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theDescribeToken);
    SQLVector.push_back(theTableNameToken);
    SQLVector.push_back(theSemiColonToken);

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) ||
            (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}
Statement* DescribeTableStatement::describeTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer) {
    DescribeTableStatement* theDescribeTable = new DescribeTableStatement(aProc, Keywords::describe_kw);
    aTokenizer.skipTo(TokenType::identifier);
    theDescribeTable->setTableName(aTokenizer.current().data);
    return theDescribeTable;
}

StatusResult DescribeTableStatement::run(std::ostream& aStream) const {
    SQLProcessor* theSQLProcessorPtr = getSQLProcessor();
    Database*     theDatabase = theSQLProcessorPtr->getDatabaseInUse();
    DescribeTableStatement* theDescribeTableStmt = const_cast<DescribeTableStatement*>(this);
    return theDatabase->describeTable(theDescribeTableStmt, aStream);
}

}  // namespace ECE141