//
//  compare.hpp
//  Created by rick gessner on 4/30/21.
//

#ifndef compare_h
#define compare_h

#include <sstream>
  
  //Equal ------------------------------------------

  template <typename T>
  bool isEqual(const T &arg1, const T &arg2) {
    return arg1==arg2;
  }

  bool isEqual(const std::string &aStr1, const std::string &aStr2) {
    return aStr1==aStr2;
  }

  template <typename T1, typename T2>
  bool isEqual(const T1 &arg1, const T2 &arg2) {
    return static_cast<T1>(arg2)==arg1;
  }

  template <typename T1>
  bool isEqual(const T1 &arg1, const std::string &aStr) {
    std::stringstream temp;
    temp << arg1;
    std::string theStr=temp.str();
    bool theResult=theStr==aStr;
    return theResult;
  }

  bool isEqual(const std::string &aStr, const bool &arg2) {
    return isEqual(arg2,aStr);
  }

  bool isEqual(const std::string &aStr, const int &arg2) {
    return isEqual(arg2,aStr);
  }

  bool isEqual(const std::string &aStr, const double &arg2) {
    return isEqual(arg2,aStr);
  }

  // Less than/Greater than ------------------------------------------

  template <typename T1>
  bool isGreater(const T1 &arg1, const std::string &aStr) {
    std::stringstream temp;
    temp << arg1;
    std::string theStr=temp.str();
    bool theResult= (theStr.compare(aStr)>0);
    return theResult;
  }

  template <typename T1>
  bool isLesser(const T1 &arg1, const std::string &aStr) {
    std::stringstream temp;
    temp << arg1;
    std::string theStr=temp.str();
    bool theResult= (theStr.compare(aStr)<0);
    return theResult;
  }

// Lesser -------------------------------
  template <typename T>
  bool isLesser(const T &arg1, const T &arg2) {
    return arg1<arg2;
  }

  bool isLesser(const std::string &aStr1, const std::string &aStr2) {
    return aStr1.compare(aStr2)<0;
  }

  template <typename T1, typename T2>
  bool isLesser(const T1 &arg1, const T2 &arg2) {
    return static_cast<T1>(arg2)>arg1;
  }

  

  bool isLesser(const std::string &aStr, const bool &arg2) {
    return isGreater(arg2,aStr);
  }

  bool isLesser(const std::string &aStr, const int &arg2) {
    return isGreater(arg2,aStr);
  }

  bool isLesser(const std::string &aStr, const double &arg2) {
    return isGreater(arg2,aStr);
  }  



  // Greater ------------------------------------------

  template <typename T>
  bool isGreater(const T &arg1, const T &arg2) {
    return arg1>arg2;
  }

  bool isGreater(const std::string &aStr1, const std::string &aStr2) {
    return aStr1.compare(aStr2)>0;
  }

  template <typename T1, typename T2>
  bool isGreater(const T1 &arg1, const T2 &arg2) {
    return static_cast<T1>(arg2)<arg1;
  }

  

  bool isGreater(const std::string &aStr, const bool &arg2) {
    return isLesser(arg2,aStr);
  }

  bool isGreater(const std::string &aStr, const int &arg2) {
    return isLesser(arg2,aStr);
  }

  bool isGreater(const std::string &aStr, const double &arg2) {
    return isLesser(arg2,aStr);
  }



  // Not Equal -------------------------------
  template <typename T>
  bool IsnotEqual(const T &arg1, const T &arg2) {
    return arg1!=arg2;
  }

  bool IsnotEqual(const std::string &aStr1, const std::string &aStr2) {
    return aStr1!=aStr2;
  }

  template <typename T1, typename T2>
  bool IsnotEqual(const T1 &arg1, const T2 &arg2) {
    return static_cast<T1>(arg2)!=arg1;
  }

  template <typename T1>
  bool IsnotEqual(const T1 &arg1, const std::string &aStr) {
    std::stringstream temp;
    temp << arg1;
    std::string theStr=temp.str();
    bool theResult=theStr!=aStr;
    return theResult;
  }

  bool IsnotEqual(const std::string &aStr, const bool &arg2) {
    return IsnotEqual(arg2,aStr);
  }

  bool IsnotEqual(const std::string &aStr, const int &arg2) {
    return IsnotEqual(arg2,aStr);
  }

  bool IsnotEqual(const std::string &aStr, const double &arg2) {
    return IsnotEqual(arg2,aStr);
  }

  //--------- feel free to add more operators... ------


#endif /* compare_h */
