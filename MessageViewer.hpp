// Author: Atul
// A generic class to handle all types of printing stamenets

#pragma once
#include "FolderReader.hpp"
#include "View.hpp"
#include "Config.hpp"
#include "Attribute.hpp"
#include "ShowsTablesView.hpp"

namespace ECE141 {

// USE: provide view class that lists db files in storage path...
class MessageViewer : public View {
   public:
    MessageViewer() {}
    void setMessage(std::string &aMessage) {
        theMessage = aMessage;
    }

    std::string getMessage() {
        return theMessage;
    }

    bool show(std::ostream &anOutput) {
        anOutput << getMessage();
        anOutput << " (" << Config::getTimer().elapsed() << " secs)" << std::endl;
        return true;
    }

    void showTableView(std::ostream &anOutput, std::vector<std::string> &anInputVec, size_t &aLargestDbName) {
        aLargestDbName=aLargestDbName+4; // To handle |<space>....<space>|
        printHeader(anOutput, anInputVec.at(0), aLargestDbName);
        for (size_t i = 1; i < anInputVec.size(); i++) {
            anOutput << "| " << anInputVec.at(i) << std::string(aLargestDbName - anInputVec.at(i).length() - 3, ' ') << "|"
                     << "\n";
        }

        anOutput << "+" << std::string(aLargestDbName - 2, '-') << "+"
                 << "\n";
    }
    void showIndexView(std::ostream &anOutput, std::vector<std::string> &anInputVec, size_t &aLargestDbName) {
        aLargestDbName=aLargestDbName+4; // To handle |<space>....<space>|
        printHeader(anOutput, anInputVec.at(0), aLargestDbName);
        for (size_t i = 1; i < anInputVec.size(); i++) {
            anOutput << "| " << anInputVec.at(i) << std::string(aLargestDbName - anInputVec.at(i).length() - 3, ' ') << "|"
                     << "\n";
        }

        anOutput << "+" << std::string(aLargestDbName - 2, '-') << "+"
                 << "\n";
    }
    void printHeader(std::ostream &anOutput, std::string &aHeader, size_t &aLargestDbName) {
        anOutput << "+" << std::string(aLargestDbName - 2, '-') << "+"
                 << "\n";
        anOutput << "| " << aHeader << std::string(aLargestDbName - aHeader.length() - 3, ' ') << "|"
                 << "\n";
        anOutput << "+" << std::string(aLargestDbName - 2, '-') << "+"
                 << "\n";
    }


    void printAttrTable(std::ostream &anOutput,std::vector<Attribute> anAttr){

        ShowsTablesView theTb(false);
        theTb.addToRow("Field");
        theTb.addToRow("Type");
        theTb.addToRow("Null");
        theTb.addToRow("Key");
        theTb.addToRow("Default");
        theTb.addToRow("Extra");
        theTb.endOfRow();
        for(size_t i = 0; i<anAttr.size();i++){
            theTb.addToRow(anAttr.at(i).getName());
            DataTypes theDt = anAttr.at(i).getType();
            switch(theDt){
                case DataTypes::int_type:{
                    theTb.addToRow("integer");
                    break;

                }
                case DataTypes::float_type:{
                    theTb.addToRow("float");
                    break;
                    
                }
                case DataTypes::bool_type:{
                    theTb.addToRow("boolean");
                    break;
                    
                }
                case DataTypes::varchar_type:{
                    std::stringstream ss;
                    ss<<"varchar("<<anAttr.at(i).getSize()<<")";
                    theTb.addToRow(ss.str());
                    break;
                    
                }
                case DataTypes::no_type:{
                    theTb.addToRow("no_type");
                    break;
                }
                case DataTypes::datetime_type:{
                    theTb.addToRow("date");
                    break;
                }
            }

            std::string theNull = anAttr.at(i).isNullable()?"YES":"NO";
            theTb.addToRow(theNull);
            std::string thePrimary = anAttr.at(i).isPrimaryKey()?"YES":" ";
            theTb.addToRow(thePrimary);
            theTb.addToRow("NULL");
            std::stringstream ss;
            ss<<" ";
            if(anAttr.at(i).isAutoIncrement()){
                ss<<"auto_increment ";
            }
            if(anAttr.at(i).isPrimaryKey()){
                ss<<"primary key ";
            }

            theTb.addToRow(ss.str());
            theTb.endOfRow();


        }

        theTb.show(anOutput);

    }

   protected:
    std::string theMessage;
};

}  // namespace ECE141