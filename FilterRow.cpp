#include "FilterRow.hpp"



namespace ECE141{
    FilterRow::FilterRow(){}
    FilterRow::~FilterRow(){}
    // To filterRows based on ORDER BY
    FilterRow& FilterRow::filterOrderBy(DBQuery &aDB,RawRowCollection &theFilteredRow){
        if(aDB.getOrderBy() != ""){
        // Order the rows ascending
        std::string theOrderBy = aDB.getOrderBy();
        if(aDB.getIsAscending()){
            std::sort(theFilteredRow.begin(),theFilteredRow.end(),[&](Row& lhs, Row& rhs){
                KeyValues v1 = lhs.getValueData();
                KeyValues v2 = rhs.getValueData();
                return v1[theOrderBy]<v2[theOrderBy];
            });
        }else{
           // Order the rows descending
            std::sort(theFilteredRow.begin(),theFilteredRow.end(),[&](Row& lhs, Row& rhs){
                KeyValues v1 = lhs.getValueData();
                KeyValues v2 = rhs.getValueData();
                return v1[theOrderBy]>v2[theOrderBy];
            });

        }


    }

    return *this;

    }
    // To filterRows based on WHERE 
    FilterRow& FilterRow::filterWhere(DBQuery &aDB,RawRowCollection &theFilteredRow){
       if(aDB.getFilter().getCount()==0){
           return *this;
       }
       RawRowCollection::iterator it = theFilteredRow.begin();
       while(it!=theFilteredRow.end()){
           if(!(aDB.getFilter().matches((*it).getData()))){
               it = theFilteredRow.erase(it);
           }else{
               it++;
           }
       }
       
        return *this;
    }
    // To filterRows based on LIMIT
    FilterRow& FilterRow::filterLimit(DBQuery &aDB,RawRowCollection &theFilteredRow){
        if(aDB.getLimit()!=-1){
            theFilteredRow.erase(theFilteredRow.begin()+aDB.getLimit(),theFilteredRow.end());
        }
        return *this;
    }

} 