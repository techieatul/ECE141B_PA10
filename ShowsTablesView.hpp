#pragma once

#include <iostream>
#include <vector>

#include "DBQuery.hpp"
#include "Entity.hpp"
#include "LRUCache.hpp"
#include "Row.hpp"
#include "View.hpp"

namespace ECE141 {
using ViewRow = std::vector<std::vector<std::string>>;
class ShowsTablesView : public View {
   protected:
    std::vector<std::string>              theRow;
    std::vector<std::vector<std::string>> theRows;
    std::vector<size_t>                   theColWidth;
    size_t                                theCurrCol;
    LRUCache<uint32_t, ViewRow>           theViewCache;

   public:
    ShowsTablesView(bool aPrintRow);
    ~ShowsTablesView();
    bool printrow;
    bool addToRow(std::string aMember);
    bool addRow(std::vector<std::string> &aRow);
    bool endOfRow();
    bool show(std::ostream &aStream);
    bool showQuery(RawRowCollection &theRows, DBQuery &aDBQuery, std::ostream &anOutput);
    bool printLine(std::ostream &aStream);
    bool printRow(std::ostream &aStream, std::vector<std::string> aRow);
};

}  // namespace ECE141
