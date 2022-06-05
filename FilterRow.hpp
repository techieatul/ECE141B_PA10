#pragma once
#ifndef FilterRow_hpp
#define FilterRow_hpp
#include "Row.hpp"
#include <algorithm>
#include <vector>
#include "BasicTypes.hpp"
#include "DBQuery.hpp"
#include "Filters.hpp"
#include "keywords.hpp"
namespace ECE141 {

class FilterRow {
   public:
    FilterRow();
    ~FilterRow();
    FilterRow& filterOrderBy(DBQuery &aDB,RawRowCollection &theFilteredRow);
    FilterRow& filterLimit(DBQuery &aDB,RawRowCollection &theFilteredRow);
    FilterRow& filterWhere(DBQuery &aDB,RawRowCollection &theFilteredRow);
};

}  // namespace ECE141

#endif