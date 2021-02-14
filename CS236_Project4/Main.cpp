#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <exception>
#include <algorithm>
//#include <cctype>
using namespace std; //stop using this

//using std::cout;

//memory leak testing
#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

bool evaluatingQueries = false;

class Tuple {
public:
   vector<std::string> attributeValues;

   bool operator<(const Tuple& t) const
   {
      return (this->attributeValues < t.attributeValues);
   }
};

class Header {
public:
   vector<std::string> attributeNames;
};

class Variable {
public:
   std::string name = "";
   std::string value = "";
};

class Relation {
public:
   std::string name = "";
   Header header;
   set<Tuple>tuples; //why not just a singular tuple?

   Relation evalQueries(vector<std::string>& queryParams) {
      Relation finalRel;
      finalRel = this->select(queryParams);
      finalRel.project(queryParams, false);
      finalRel.rename(queryParams);
      return finalRel;
   }

   Relation select(vector<std::string>& queryParams) {
      Relation selected;
      selected.name = this->name;
      //bool matches = false;
      selected.tuples = tuples;
      //vector <Variable> variableValues;
      bool repeat = false;
      vector <int> variablePos;

      for (unsigned int i = 0; i < queryParams.size(); ++i) {
         if (queryParams.at(i).at(0) == '\'') {
            set<Tuple> tempSet;
            set <Tuple> ::iterator itr = selected.tuples.begin();
            set <Tuple> ::iterator itr2 = selected.tuples.end();
            for (; itr != itr2; ++itr) {
               if (queryParams.at(i) != (*itr).attributeValues.at(i)) {
                  //tempSet.erase(itr);
                  //itr = itr2;
                  //itr2 = selected.tuples.end();
               }
               else {
                  Tuple tempTup;
                  tempTup.attributeValues = (*itr).attributeValues;
                  tempSet.insert(tempTup);
               }

            }
            selected.tuples = tempSet;
         }
         else {
            repeat = false;
            for (unsigned int j = 0; j < i; ++j) {
               if (queryParams.at(i) == queryParams.at(j)) {
                  repeat = true;
               }
            }
            if (repeat == false) {
               for (unsigned int j = i; j < queryParams.size(); ++j) {
                  if (queryParams.at(i) == queryParams.at(j)) {
                     variablePos.push_back(j);
                  }
               }
               if (selected.tuples.size() > 0) {
                  set <Tuple> ::iterator itr = selected.tuples.begin();
                  set<Tuple> tempSet;
                  //set <Tuple> ::iterator itr2 = itr;
                  for (; itr != selected.tuples.end(); ++itr) {
                     for (unsigned int j = 0; j < variablePos.size(); ++j) {
                        if ((*itr).attributeValues.at(variablePos.at(j)) != (*itr).attributeValues.at(i)) {
                           //selected.tuples.erase(itr);
                           //itr = itr2;
                           //break;
                        }
                        else {
                           Tuple tempTup;
                           tempTup.attributeValues = (*itr).attributeValues;
                           tempSet.insert(tempTup);
                        }
                        //itr2 = itr;
                     }
                  }
                  selected.tuples = tempSet;
               }


               /*set <Tuple> ::iterator itr = selected.tuples.begin();
               for (unsigned int x = 0; x < i; ++x) {
                  ++itr;
               }
               set <Tuple>::iterator itr2 = itr;
               for (unsigned int j = 0; j < variablePos.size(); ++j) {
                  itr = itr2;
                  for (unsigned int f = 0; f < variablePos.at(j); ++f) {
                     ++itr;
                  }
                  if ((*itr).attributeValues.at(i) != (*itr2).attributeValues.at(i)) {

                  }
               }*/
            }
         }

      }

      /*set <Tuple> ::iterator itr;
      for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
         matches = false;
         for (unsigned int i = 0; i < (*itr).attributeValues.size(); ++i) {
            matches = true;
            if (queryParams.at(i).size() == 1) {
            //   Variable tempVar;
            //   tempVar.name() = "";
            //   variableValues.push_back(tempVar);
            //}
            if (queryParams.at(i) == (*itr).attributeValues.at(i)) {

            }
            else {
               matches = false;
               break;
            }
         }
         if (matches) {
            //selectedTups.insert((*itr));
         }
      }*/


      return selected;
   }

   void project(vector<std::string> queryParams, bool fromRules) {
      bool repeat = false;

      set<Tuple> tempSet;

      vector <bool> failed;
      set <Tuple> ::iterator itr3;
      for (itr3 = tuples.begin(); itr3 != tuples.end(); ++itr3) {
         failed.push_back(false);
      }

      for (unsigned int i = 0; i < queryParams.size(); ++i) {
         vector <int> variablePos;
         if (queryParams.at(i).at(0) == '\'') {
            /*set <Tuple> ::iterator itr;
            for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
               if (queryParams.at(i) != (*itr).attributeValues.at(i)) {
                  vector <std::string> tempString = (*itr).attributeValues;
                  vector <std::string>::iterator itr3 = tempString.begin();
                  tempString.erase(itr3 + i);
                  Tuple tempTup = (*itr);
                  tempTup.attributeValues = tempString;
                  tuples.erase(itr);
                  tuples.insert(itr, tempTup);

               }
            }*/
            int tupleNum = 0;
            set <Tuple> ::iterator itr;
            for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
               if ((queryParams.at(i) == (*itr).attributeValues.at(i)) && (failed.at(tupleNum) != true)) {
                  Tuple tempTup;
                  tempTup.attributeValues = (*itr).attributeValues;
                  tempSet.insert(tempTup);
                  ++tupleNum;
               }
            }
         }
         else {
            repeat = false;
            for (unsigned int j = 0; j < i; ++j) {
               if (queryParams.at(i) == queryParams.at(j)) {
                  repeat = true;
               }
            }
            if (repeat == false) {

               for (unsigned int j = i; j < queryParams.size(); ++j) {
                  if (queryParams.at(i) == queryParams.at(j)) {
                     variablePos.push_back(j);
                  }
               }

               //bool same = true;

               int tupleNum = 0;


               set <Tuple> ::iterator itr;
               for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
                  bool same = true;
                  for (unsigned int k = 0; k < variablePos.size(); ++k) {
                     if ((*itr).attributeValues.at(variablePos.at(k)) != (*itr).attributeValues.at(variablePos.at(0))) {
                        same = false;
                        failed.at(tupleNum) = true;
                     }
                  }
                  //if (queryParams.at(i) == (*itr).attributeValues.at(i)) {
                  if (same && !failed.at(tupleNum)) {
                     Tuple tempTup;

                     if (fromRules) {
                        vector <string> tempValues;
                        for (unsigned int o = 0; o < queryParams.size(); ++o) {
                           for (unsigned int p = 0; p < header.attributeNames.size(); ++p) {
                              if (header.attributeNames.at(p) == queryParams.at(o)) {
                                 tempValues.push_back((*itr).attributeValues.at(p));
                              }
                           }
                        }
                        tempTup.attributeValues = tempValues;
                     }
                     else {
                        tempTup.attributeValues = (*itr).attributeValues;
                     }

                     //tempTup.attributeValues = (*itr).attributeValues;          //Ok so the comment above this should be where I get rid of values in tuples that dont match the query values but its doesnt work so im putting it in rename because whatever
                     tempSet.insert(tempTup);
                  }
                  else {
                     set <Tuple> ::iterator itr2;
                     for (itr2 = tempSet.begin(); itr2 != tempSet.end(); ++itr2) {
                        Tuple tempTup;
                        tempTup.attributeValues = (*itr).attributeValues;
                        //tempSet.insert(tempTup);
                        if (tempTup.attributeValues == (*itr2).attributeValues) {
                           tempSet.erase(itr2);
                           break;
                        }

                     }
                  }
                  ++tupleNum;
                  //}
               }
            }
            else {
               /*set <Tuple> ::iterator itr;
               for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
                  if (queryParams.at(i) != (*itr).attributeValues.at(i)) {
                     vector <std::string> tempString = (*itr).attributeValues;
                     vector <std::string>::iterator itr3 = tempString.begin();
                     tempString.erase(itr3 + i);
                     Tuple tempTup = (*itr);
                     tempTup.attributeValues = tempString;
                     tuples.erase(itr);
                     tuples.insert(tempTup);

                  }
               }*/
            }

         }

      }
      tuples = tempSet;
      return;

   }

   void rename(vector<std::string>& queryParams) {
      vector <string> varNames;
      bool repeat = false;
      
      /*if (header.attributeNames.size() != 0) {
         set<Tuple> tempTuples;

         set <Tuple> ::iterator itr;
         
         for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
            for (unsigned int i = 0; i < queryParams.size(); ++i) {
               string sl = header.attributeNames.at(i);
               transform(sl.begin(), sl.end(), sl.begin(), ::tolower);
               if (queryParams.at(i) == sl) {
                  tempTuples.insert((*itr));
               }
            }

         }

         tuples = tempTuples;

      }*/

      for (unsigned int i = 0; i < queryParams.size(); ++i) {
         if (evaluatingQueries) {
            if (queryParams.at(i).at(0) == '\'') {
            }
            else {
            repeat = false;
            for (unsigned int j = 0; j < i; ++j) {
               if (queryParams.at(i) == queryParams.at(j)) {
                  repeat = true;
               }
            }
            if (repeat == false) {
               varNames.push_back(queryParams.at(i));
            }
            }
         }
         else {
            
            repeat = false;
            for (unsigned int j = 0; j < i; ++j) {
               if (queryParams.at(i) == queryParams.at(j)) {
                  repeat = true;
               }
            }
            if (repeat == false) {
               varNames.push_back(queryParams.at(i));
            }

         }
      }
      header.attributeNames = varNames;

      return;
   }

   Relation join(Relation& toBeJoined) {
      Relation joinedTo = *this;
      Header outRelHeader;
      vector<int> overlap1;
      vector<int> overlap2;

      for (unsigned int i = 0; i < joinedTo.header.attributeNames.size(); ++i) {
         outRelHeader.attributeNames.push_back(joinedTo.header.attributeNames.at(i));
      }

      for (unsigned int i = 0; i < toBeJoined.header.attributeNames.size(); ++i) {

         std::vector<string>::iterator itr;
         itr = std::find (outRelHeader.attributeNames.begin(), outRelHeader.attributeNames.end(), toBeJoined.header.attributeNames.at(i));

         if (itr != outRelHeader.attributeNames.end()) {

            overlap1.push_back(itr-outRelHeader.attributeNames.begin());
            overlap2.push_back(i);

         }
         else {

            outRelHeader.attributeNames.push_back(toBeJoined.header.attributeNames.at(i));

         }
      }
      //outRelHeader.attributeNames = joinedTo.header.attributeNames + toBeJoined.header.attributeNames;

      Relation outputRelation;
      outputRelation.header = outRelHeader;

      /*for (unsigned int i = 0; i < outputRelation.header.attributeNames.size(); ++i) {
         cout << outputRelation.header.attributeNames.at(i) << ", ";
      }
      cout << "\n";*/


      std::set<Tuple>::iterator t1_itr;
      for (t1_itr = joinedTo.tuples.begin(); t1_itr != joinedTo.tuples.end(); ++t1_itr) {

         std::set<Tuple>::iterator t2_itr;
         for (t2_itr = toBeJoined.tuples.begin(); t2_itr != toBeJoined.tuples.end(); ++t2_itr) {

            //overlap1 and 2 should be same size
            bool match = true;
            if (overlap1.size() != 0) {
               for (unsigned int k = 0; k < overlap1.size(); ++k) {
                  if ((*t1_itr).attributeValues.at(overlap1.at(k)) != (*t2_itr).attributeValues.at(overlap2.at(k))) {
                     match = false;
                     break;
                  }
               }
            }

            if (match) {
               Tuple tempTup;

               // this is assuming that if any matching headers values dont match then we dont create a new tuple
               for (unsigned int k = 0; k < (*t1_itr).attributeValues.size(); ++k) {
                  tempTup.attributeValues.push_back((*t1_itr).attributeValues.at(k));
               }
               for (unsigned int k = 0; k < (*t2_itr).attributeValues.size(); ++k) {
                  bool isOverlap = false;
                  for (unsigned int j = 0; j < overlap2.size(); ++j) {
                     if (k == (unsigned int) overlap2.at(j)) {
                        isOverlap = true;
                        break;
                     }
                  }
                  
                  if (!isOverlap) {
                     tempTup.attributeValues.push_back((*t2_itr).attributeValues.at(k));
                  }
                  
               }

               outputRelation.tuples.insert(tempTup);
            }

         }
      }

      //std::set<Tuple>::iterator itr;
      /*for (itr = outputRelation.tuples.begin(); itr != outputRelation.tuples.end(); ++itr) {
         for (unsigned int i = 0; i < (*itr).attributeValues.size(); ++i) {
            cout << (*itr).attributeValues.at(i) << ", ";
         }
         cout << "\n";
      }*/

      return outputRelation;
   }

   //return relation?
   void unionize(vector<std::string>& queryParams, set<Tuple>& newTuples) {
      std::set<Tuple>::iterator itr;
      //int tuplePos = 0;
      for (itr = newTuples.begin(); itr != newTuples.end(); ++itr) {
         if (tuples.insert(*(itr)).second) {
            cout << "  ";
            for (unsigned int o = 0; o < (*itr).attributeValues.size(); ++o) {
               cout << this->header.attributeNames.at(o) << "=";
               if (o != (*itr).attributeValues.size() - 1) {
                  cout << (*itr).attributeValues.at(o) << ", ";
               }
               else {
                  cout << (*itr).attributeValues.at(o);
               }
            }
            cout << "\n";
         }
         else {
            
         }
         //++tuplePos;
      } // Doesnt determine if attribute values match

      /*for (unsigned int i = 0; i < queryParams.size(); ++i) {
         for (unsigned int k = 0; k < header.attributeNames.size(); ++k) {
            if (header.attributeNames.at(k) == queryParams.at(i)) {
               itr = next(newTuples.begin(),k);
               if (tuples.insert(*(itr)).second) {

               }
               else {
                  for (unsigned int o = 0; o < (*itr).attributeValues.size(); ++o) {
                     cout << (*itr).attributeValues.at(o) << ".";
                  }
               }
            }
         }
      }*/
      
   }
};

class Database {
public:
   vector<Relation> relations;
};





class token {
public:
   std::string type = "";
   std::string label = "";
   int line = 0;

   std::string toString() {
      return "(" + type + ",\"" + label + "\"," + to_string(line) + ")" + "\n";
   }
};

vector<token> lexer(vector<std::string> lines_in_order) {
   int lineNumber = 1;
   int tokenCount = 0;

   std::string line = "";
   bool stringOpen = false;
   int stringOpenAt = 0;
   int stringOpenPos = 0;
   bool commentOpen = false;
   int commentOpenAt = 0;
   int commentOpenPos = 0;

   bool fID = false;
   bool sID = false;
   bool rID = false;
   bool qID = false;

   vector<token> tokenList;

   for (int a = 0; static_cast<unsigned int>(a) < lines_in_order.size(); ++a) {
      //cout << a;
      line = lines_in_order.at(a);
      for (int i = 0; static_cast<unsigned int>(i) < line.length(); ++i) {
         char c = line.at(i);

         if (commentOpen) {
            if (c == '|' && static_cast<unsigned int>(i) + 1 < line.length() && line.at(i + 1) == '#') {
               ++i; //??????
               if (a == commentOpenAt - 1) {

                  commentOpen = false;
               }
               else {
                  commentOpen = false;
                  ++tokenCount;
               }
            }
            else {
               if (static_cast<unsigned int>(a) == lines_in_order.size() - 1 && static_cast<unsigned int>(i) == line.length() - 1) {
                  std::string tempString = "";
                  tempString += lines_in_order.at(commentOpenAt - 1).substr(commentOpenPos) + "\n";
                  for (int l = commentOpenAt; l <= a; ++l) {
                     if (l == a) {
                        tempString += lines_in_order.at(l).substr(0, i + 1) + "\n";
                     }
                     else {
                        tempString += lines_in_order.at(l).substr(0) + "\n"; //you dont even need substring here
                     }

                  }

                  token temp;
                  temp.type = "UNDEFINED";
                  temp.label = tempString;
                  temp.line = commentOpenAt;
                  tokenList.push_back(temp);

                  ++tokenCount;
                  commentOpen = false;
               }
            }
         }
         else if (stringOpen) {
            if (c == '\'') {
               if (static_cast<unsigned int>(i) + 1 < line.length() && line.at(i + 1) == '\'') {
                  ++i;
               }
               else if (a == stringOpenAt - 1) {
                  token temp;
                  temp.type = "STRING";
                  temp.label = lines_in_order.at(stringOpenAt - 1).substr(stringOpenPos, i + 1 - stringOpenPos);
                  temp.line = stringOpenAt;
                  tokenList.push_back(temp);

                  stringOpen = false;
                  ++tokenCount;
               }
               else {
                  std::string tempString = "";
                  tempString += lines_in_order.at(stringOpenAt - 1).substr(stringOpenPos);
                  for (int l = stringOpenAt; l <= a; ++l) {
                     if (l == a) {
                        tempString += "\n" + lines_in_order.at(l).substr(0, i + 1);
                     }
                     else {
                        tempString += "\n" + lines_in_order.at(l).substr(0);
                     }

                  }

                  token temp;
                  temp.type = "STRING";
                  temp.label = tempString;
                  temp.line = stringOpenAt;
                  tokenList.push_back(temp);

                  stringOpen = false;
                  ++tokenCount;
               }
            }
            else {
               if (static_cast<unsigned int>(a) == lines_in_order.size() - 1 && static_cast<unsigned int>(i) == line.length() - 1) {
                  std::string tempString = "";
                  tempString += lines_in_order.at(stringOpenAt - 1).substr(stringOpenPos) + "\n";
                  for (int l = stringOpenAt; l <= a; ++l) {
                     if (l == a) {
                        tempString += lines_in_order.at(l).substr(0, i + 1) + "\n";
                     }
                     else {
                        tempString += lines_in_order.at(l).substr(0) + "\n";
                     }

                  }

                  token temp;
                  temp.type = "UNDEFINED";
                  temp.label = tempString;
                  temp.line = stringOpenAt;
                  tokenList.push_back(temp);

                  ++tokenCount;
                  stringOpen = false;
               }
            }
         }
         else if (c == ',') {
            token temp;
            temp.type = "COMMA";
            temp.label = ",";
            temp.line = lineNumber;
            tokenList.push_back(temp);

            ++tokenCount;
         }
         else if (c == '.') {
            token temp;
            temp.type = "PERIOD";
            temp.label = ".";
            temp.line = lineNumber;
            tokenList.push_back(temp);

            ++tokenCount;
         }
         else if (c == '?') {
            token temp;
            temp.type = "Q_MARK";
            temp.label = "?";
            temp.line = lineNumber;
            tokenList.push_back(temp);

            ++tokenCount;
         }
         else if (c == '(') {
            token temp;
            temp.type = "LEFT_PAREN";
            temp.label = "(";
            temp.line = lineNumber;
            tokenList.push_back(temp);

            ++tokenCount;
         }
         else if (c == ')') {
            token temp;
            temp.type = "RIGHT_PAREN";
            temp.label = ")";
            temp.line = lineNumber;
            tokenList.push_back(temp);

            ++tokenCount;
         }
         else if (c == ':') {
            if (static_cast<unsigned int>(i) < line.length() - 1 && line.at(i + 1) == '-') {
               token temp;
               temp.type = "COLON_DASH";
               temp.label = ":-";
               temp.line = lineNumber;
               tokenList.push_back(temp);

               ++tokenCount;
               ++i;
            }
            else {
               token temp;
               temp.type = "COLON";
               temp.label = ":";
               temp.line = lineNumber;
               tokenList.push_back(temp);

               ++tokenCount;
            }
         }
         else if (c == '*') {
            token temp;
            temp.type = "MULTIPLY";
            temp.label = "*";
            temp.line = lineNumber;
            tokenList.push_back(temp);
            ++tokenCount;
         }
         else if (c == '+') {
            token temp;
            temp.type = "ADD";
            temp.label = "+";
            temp.line = lineNumber;
            tokenList.push_back(temp);
            ++tokenCount;
         }
         else if (c == 'S' && (sID != true)) {   //how this works with ID
            if (static_cast<unsigned int>(i) < line.length() - 6 && line.substr(i, 7) == "Schemes") {
               if (static_cast<unsigned int>(i) < line.length() - 7 && (isalpha(line.at(i + 7)) || isdigit(line.at(i + 7)))) {
                  sID = true;
                  --i;
               }
               else {
                  token temp;
                  temp.type = "SCHEMES";
                  temp.label = "Schemes";
                  temp.line = lineNumber;
                  tokenList.push_back(temp);
                  ++tokenCount;
                  i += 6;
               }
            }
            else {
               sID = true;
               --i;
            }
         }
         else if (c == 'F' && (fID != true)) {   //how this works with ID
            if (static_cast<unsigned int>(i) < line.length() - 4 && line.substr(i, 5) == "Facts") {
               if (static_cast<unsigned int>(i) < line.length() - 5 && (isalpha(line.at(i + 5)) || isdigit(line.at(i + 5)))) {
                  fID = true;
                  --i;
               }
               else {
                  token temp;
                  temp.type = "FACTS";
                  temp.label = "Facts";
                  temp.line = lineNumber;
                  tokenList.push_back(temp);
                  ++tokenCount;
                  i += 4;
               }
            }
            else {
               fID = true;
               --i;
            }
         }
         else if (c == 'R' && (rID != true)) {   //how this works with ID
            if (static_cast<unsigned int>(i) < line.length() - 4 && line.substr(i, 5) == "Rules") {
               if (static_cast<unsigned int>(i) < line.length() - 5 && (isalpha(line.at(i + 5)) || isdigit(line.at(i + 5)))) {
                  rID = true;
                  --i;
               }
               else {
                  token temp;
                  temp.type = "RULES";
                  temp.label = "Rules";
                  temp.line = lineNumber;
                  tokenList.push_back(temp);
                  ++tokenCount;
                  i += 4;
               }
            }
            else {
               rID = true;
               --i;
            }
         }
         else if (c == 'Q' && (qID != true)) {   //how this works with ID
            if (static_cast<unsigned int>(i) < line.length() - 6 && line.substr(i, 7) == "Queries") {
               if (static_cast<unsigned int>(i) < line.length() - 7 && (isalpha(line.at(i + 7)) || isdigit(line.at(i + 7)))) {
                  qID = true;
                  --i;
               }
               else {
                  token temp;
                  temp.type = "QUERIES";
                  temp.label = "Queries";
                  temp.line = lineNumber;
                  tokenList.push_back(temp);
                  ++tokenCount;
                  i += 6;
               }
            }
            else {
               qID = true;
               --i;
            }
         }
         else if (c == '\'') {
            stringOpenPos = i;
            stringOpen = true;
            stringOpenAt = lineNumber;

         }
         else if (c == '#') {
            if (static_cast<unsigned int>(i) < line.length() - 1 && line.at(i + 1) == '|') {
               commentOpenPos = i;
               commentOpen = true;
               commentOpenAt = lineNumber;
            }
            else {
               i += line.length() - i;
            }
         }
         else if (isalpha(c)) {
            std::string ID = "";
            bool isS = false;
            bool isF = false;
            bool isR = false;
            bool isQ = false;

            for (int j = i; static_cast<unsigned int>(j) < line.length(); ++j) {
               if (static_cast<unsigned int>(j) < line.length() - 6 && line.substr(j, 7) == "Schemes" && sID == false) {
                  isS = true;
                  i += 6;
                  break;
               }
               else if (static_cast<unsigned int>(j) < line.length() - 4 && line.substr(i, 5) == "Facts" && fID == false) {
                  isF = true;
                  i += 4;
                  break;
               }
               else if (static_cast<unsigned int>(j) < line.length() - 4 && line.substr(i, 5) == "Rules" && rID == false) {
                  isR = true;
                  i += 4;
                  break;
               }
               else if (static_cast<unsigned int>(j) < line.length() - 6 && line.substr(i, 7) == "Queries" && qID == false) {
                  isQ = true;
                  i += 6;
                  break;
               }
               else if (isalpha(line.at(j)) || isdigit(line.at(j))) {
                  ID += line.at(j);
               }
               else {
                  break;
               }
            }

            fID = false;
            sID = false;
            rID = false;
            qID = false; //this is all redundant. Move ID before identifiers or delete identifiers?

            if (isS) {
               token temp;
               temp.type = "SCHEMES";
               temp.label = "Schemes";
               temp.line = lineNumber;
               tokenList.push_back(temp);
               ++tokenCount;
            }
            else if (isF) {
               token temp;
               temp.type = "FACTS";
               temp.label = "Facts";
               temp.line = lineNumber;
               tokenList.push_back(temp);
               ++tokenCount;
            }
            else if (isR) {
               token temp;
               temp.type = "RULES";
               temp.label = "Rules";
               temp.line = lineNumber;
               tokenList.push_back(temp);
               ++tokenCount;
            }
            else if (isQ) {
               token temp;
               temp.type = "QUERIES";
               temp.label = "Queries";
               temp.line = lineNumber;
               tokenList.push_back(temp);
               ++tokenCount;
            }
            else {
               token temp;
               temp.type = "ID";
               temp.label = ID;
               temp.line = lineNumber;
               tokenList.push_back(temp);
               ++tokenCount;
            }
            if (ID.length() > 0) {
               i += ID.length() - 1;
            }

         }
         else if (isspace(c)) {}
         else {
            token temp;
            temp.type = "UNDEFINED";
            temp.label = c;
            temp.line = lineNumber;
            tokenList.push_back(temp);

            ++tokenCount;
         }

      }

      ++lineNumber;
   }

   if (stringOpen) {
      std::string tempString = "";
      tempString += lines_in_order.at(stringOpenAt - 1).substr(stringOpenPos) + "\n";
      for (int l = stringOpenAt; static_cast<unsigned int>(l) < lines_in_order.size(); ++l) {
         if (l == stringOpenAt) {
            tempString += lines_in_order.at(l).substr(0) + "\n";
         }
         else {
            tempString += lines_in_order.at(l).substr(0) + "\n"; //you dont even need substring here
         }

      }

      token temp;
      temp.type = "UNDEFINED";
      temp.label = tempString;
      temp.line = stringOpenAt;
      tokenList.push_back(temp);

      ++tokenCount;
      stringOpen = false;
   }
   if (commentOpen) {
      std::string tempString = "";
      tempString += lines_in_order.at(commentOpenAt - 1).substr(commentOpenPos) + "\n";
      for (int l = commentOpenAt; static_cast<unsigned int>(l) < lines_in_order.size(); ++l) {
         //cout << l;
         if (l == commentOpenAt) {
            tempString += lines_in_order.at(l).substr(0) + "\n";
         }
         else {
            tempString += lines_in_order.at(l).substr(0) + "\n"; //you dont even need substring here
         }

      }

      token temp;
      temp.type = "UNDEFINED";
      temp.label = tempString;
      temp.line = commentOpenAt;
      tokenList.push_back(temp);

      ++tokenCount;
      commentOpen = false;
   }

   token temp;
   temp.type = "EOF";
   temp.label = "";
   temp.line = lineNumber;
   tokenList.push_back(temp);
   ++tokenCount;


   return tokenList;
   /*string output = "";
   for (unsigned int i = 0; i < tokenList.size(); ++i) {
      //cout << tokenList.at(i).toString();
      output += tokenList.at(i).toString();
   }

   //cout << "Total Tokens = " << tokenList.size() << "\n";
   output += "Total Tokens = " + to_string(tokenList.size()) + "\n";*/
   //return output;
}

int currentToken = 0;
std::string fileOutput = "";

class Predicate {
public:
   std::string name = "";
   vector<std::string> contentList;

   std::string toString() const {
      std::string tempOut = "";
      tempOut += name + "(";
      for (unsigned int i = 0; i < contentList.size(); ++i) {
         tempOut += contentList.at(i);
         if (i != contentList.size() - 1) {
            tempOut += ",";
         }
      }
      tempOut += ")";
      return tempOut;
   }
};

class Rule {
public:
   std::string headPredicateName = "";
   vector<std::string> headPredicateContent;
   vector<Predicate> predicateList;

   std::string toString() const {
      std::string tempOut = "";
      tempOut += headPredicateName + "(";
      for (unsigned int i = 0; i < headPredicateContent.size(); ++i) {
         tempOut += headPredicateContent.at(i);
         if (i != headPredicateContent.size() - 1) {
            tempOut += ",";
         }
      }
      tempOut += ") :- ";
      for (unsigned int j = 0; j < predicateList.size(); ++j) {
         tempOut += predicateList.at(j).toString();
         if (j != predicateList.size() - 1) {
            tempOut += ",";
         }
      }
      return tempOut;
   }
};

class Parameter {
public:
   std::string toString() const {
      return value;
   }
   //string type = "";
   std::string value = "";
};

class DatalogProgram {
public:
   vector<Predicate> schemeList1;
   vector<Predicate> factList1;
   vector<Rule> ruleList1;
   vector<Predicate> queryList1;
   vector<Parameter> parameterList1;
   set<std::string> domainSet;
   Database dataB;

   void interpret() {


      for (unsigned int i = 0; i < schemeList1.size(); ++i) {
         Relation r;
         r.name = schemeList1.at(i).name;
         r.header.attributeNames = schemeList1.at(i).contentList;
         dataB.relations.push_back(r);

      }

      for (unsigned int i = 0; i < factList1.size(); ++i) {
         for (unsigned int j = 0; j < dataB.relations.size(); ++j) {
            if (factList1.at(i).name == dataB.relations.at(j).name) {
               Tuple tuple1;
               for (unsigned int f = 0; f < factList1.at(i).contentList.size(); ++f) {
                  //Tuple tuple1;
                  tuple1.attributeValues.push_back(factList1.at(i).contentList.at(f));
                  //tempTup.insert(tuple1);
               }
               dataB.relations.at(j).tuples.insert(tuple1);
            }

         }
         //int* p = find(dataB.relations.at, dataB.relations.end(), "test");
         //r.header.attributeNames = schemeList1.at(i).contentList;
      }

      //test join
      //dataB.relations.at(0).join(dataB.relations.at(4));  

      
      //rules
      cout << "Rule Evaluation" << "\n";

      int rulePasses = 0;
      bool inserting = true;

      while (inserting) {
         int preTupleCount = 0;
         for (unsigned int k = 0; k < dataB.relations.size(); ++k) {
            for (unsigned int o = 0; o < dataB.relations.at(k).tuples.size(); ++o) {
               ++preTupleCount;
            }
         }

         for (unsigned int i = 0; i < ruleList1.size(); ++i) {
            cout << ruleList1.at(i).headPredicateName << "(";

            for (unsigned int k = 0; k < ruleList1.at(i).headPredicateContent.size(); ++k) {
               if (k != ruleList1.at(i).headPredicateContent.size() - 1) {
                  cout << ruleList1.at(i).headPredicateContent.at(k) << ",";
               }
               else {
                  cout << ruleList1.at(i).headPredicateContent.at(k);
               }
            }

            cout << ") :- ";
            for (unsigned int o = 0; o < ruleList1.at(i).predicateList.size(); ++o) {
               cout << ruleList1.at(i).predicateList.at(o).name << "(";
               for (unsigned int j = 0; j < ruleList1.at(i).predicateList.at(o).contentList.size(); ++j) {
                  if (j == ruleList1.at(i).predicateList.at(o).contentList.size() - 1) {
                     cout << ruleList1.at(i).predicateList.at(o).contentList.at(j);
                  }
                  else {
                     cout << ruleList1.at(i).predicateList.at(o).contentList.at(j) << ",";
                  }
               }
               if (o != ruleList1.at(i).predicateList.size() - 1) {
                  cout << "),";
               }
               else {
                  cout << ")";
               }
            }
            cout << ".\n";


            Relation currRelation;
            vector <Relation> interRelations;

            for (unsigned int k = 0; k < ruleList1.at(i).predicateList.size(); ++k) {

               for (unsigned int j = 0; j < dataB.relations.size(); ++j) {
                  if (ruleList1.at(i).predicateList.at(k).name == dataB.relations.at(j).name) {

                     currRelation = dataB.relations.at(j).evalQueries(ruleList1.at(i).predicateList.at(k).contentList);

                  }
               }

               interRelations.push_back(currRelation);

               /*if (currRelation.tuples.size() == 0) {
                  //cout << "No\n";
               }
               else {
                  //cout << "Yes(" << currRelation.tuples.size() << ")\n";
                  set <Tuple> ::iterator itr;
                  for (itr = currRelation.tuples.begin(); itr != currRelation.tuples.end(); ++itr) {
                     for (unsigned int j = 0; j < currRelation.header.attributeNames.size(); ++j) {
                        if (j != currRelation.header.attributeNames.size() - 1) {
                           if (j == 0) {
                              cout << " ";
                           }
                           cout << " ";
                           cout << currRelation.header.attributeNames.at(j) << "=";

                           for (unsigned int o = 0; o < ruleList1.at(i).predicateList.at(k).contentList.size(); ++o) {
                              if (currRelation.header.attributeNames.at(j) == ruleList1.at(i).predicateList.at(k).contentList.at(o)) {
                                 cout << (*itr).attributeValues.at(o) << ",";
                                 break;
                              }
                           }
                        }
                        else {
                           if (j == 0) {
                              cout << " ";
                           }
                           cout << " ";
                           cout << currRelation.header.attributeNames.at(j) << "=";

                           for (unsigned int o = 0; o < ruleList1.at(i).predicateList.at(k).contentList.size(); ++o) {
                              if (currRelation.header.attributeNames.at(j) == ruleList1.at(i).predicateList.at(k).contentList.at(o)) {
                                 cout << (*itr).attributeValues.at(o);
                                 break;
                              }
                           }


                           cout << "\n";
                        }
                     }
                  }
               }*/

            }

            Relation finalRelation = interRelations.at(0);

            for (unsigned int k = 1; k < interRelations.size(); ++k) {
               finalRelation = finalRelation.join(interRelations.at(k));
            }

            finalRelation.project(ruleList1.at(i).headPredicateContent, true);
            finalRelation.name = ruleList1.at(i).headPredicateName;
            //finalRelation.rename(ruleList1.at(i).headPredicateContent);

            for (unsigned int k = 0; k < dataB.relations.size(); ++k) {
               if (dataB.relations.at(k).name == finalRelation.name) {
                  finalRelation.rename(dataB.relations.at(k).header.attributeNames);
                  dataB.relations.at(k).unionize(finalRelation.header.attributeNames, finalRelation.tuples);
                  /*set <Tuple> ::iterator itr;
                  for (itr = dataB.relations.at(k).tuples.begin(); itr != dataB.relations.at(k).tuples.end(); ++itr) {
                     for (unsigned int l = 0; l < dataB.relations.at(k).header.attributeNames.size(); ++l) {
                        cout << dataB.relations.at(k).header.attributeNames.at(l) << "=";
                        cout << (*itr).attributeValues.at(l) << ", ";
                     }
                  }
                  */
                  //cout << "\n";
               }
            }

            

            /*for (unsigned int k = 0; k < finalRelation.header.attributeNames.size(); ++k) {
               cout << finalRelation.header.attributeNames.at(k) << "=";
            }
            cout << "\n";*/

         }

         int postTupleCount = 0;
         for (unsigned int k = 0; k < dataB.relations.size(); ++k) {
            for (unsigned int o = 0; o < dataB.relations.at(k).tuples.size(); ++o) {
               ++postTupleCount;
            }
         }

         ++rulePasses;
         if (preTupleCount == postTupleCount) {
            inserting = false;
         }

      }
      //end rule evaluation

      cout << "\n";
      cout << "Schemes populated after " << rulePasses << " passes through the Rules." << "\n";
      cout << "\n";

      //Query Output
      cout << "Query Evaluation" << "\n";
      evaluatingQueries = true;
      Relation currRelation;

      for (unsigned int i = 0; i < queryList1.size(); ++i) {
         for (unsigned int j = 0; j < dataB.relations.size(); ++j) {
            if (queryList1.at(i).name == dataB.relations.at(j).name) {

               currRelation = dataB.relations.at(j).evalQueries(queryList1.at(i).contentList);

            }
         }

         cout << queryList1.at(i).name << "(";
         for (unsigned int j = 0; j < queryList1.at(i).contentList.size(); ++j) {
            if (j == queryList1.at(i).contentList.size() - 1) {
               cout << queryList1.at(i).contentList.at(j);
            }
            else {
               cout << queryList1.at(i).contentList.at(j) << ",";
            }
         }
         cout << ")? ";
         if (currRelation.tuples.size() == 0) {
            cout << "No\n";
         }
         else {
            cout << "Yes(" << currRelation.tuples.size() << ")\n";
            set <Tuple> ::iterator itr;
            for (itr = currRelation.tuples.begin(); itr != currRelation.tuples.end(); ++itr) {
               for (unsigned int k = 0; k < currRelation.header.attributeNames.size(); ++k) {
                  if (k != currRelation.header.attributeNames.size() - 1) {
                     if (k == 0) {
                        cout << " ";
                     }
                     cout << " ";
                     cout << currRelation.header.attributeNames.at(k) << "=";

                     for (unsigned int o = 0; o < queryList1.at(i).contentList.size(); ++o) {
                        if (currRelation.header.attributeNames.at(k) == queryList1.at(i).contentList.at(o)) {
                           cout << (*itr).attributeValues.at(o) << ",";
                           break;
                        }
                     }
                     //cout << (*itr).attributeValues.at(k) << ",";
                  }
                  else {
                     if (k == 0) {
                        cout << " ";
                     }
                     cout << " ";
                     cout << currRelation.header.attributeNames.at(k) << "=";

                     for (unsigned int o = 0; o < queryList1.at(i).contentList.size(); ++o) {
                        if (currRelation.header.attributeNames.at(k) == queryList1.at(i).contentList.at(o)) {
                           cout << (*itr).attributeValues.at(o) << "\n";
                           break;
                        }
                     }
                     //cout << (*itr).attributeValues.at(k) << "\n";
                  }
               }
            }

         }

      }

      //End Query Output

      /*for (unsigned int i = 0; i < dataB.relations.size(); ++i) {
         cout << dataB.relations.at(i).name;
         for (unsigned int j = 0; j < dataB.relations.at(i).header.attributeNames.size(); ++j) {
            cout << dataB.relations.at(i).header.attributeNames.at(j);
         }

         set <Tuple> ::iterator itr;
         for (itr = dataB.relations.at(i).tuples.begin(); itr != dataB.relations.at(i).tuples.end(); ++itr) {
            for (unsigned int f = 0; f < (*itr).attributeValues.size(); ++f) {
               cout << (*itr).attributeValues.at(f);
            }
         }


      }*/
   }


   std::string toString() const {
      std::string tempString = "";

      for (unsigned int i = 0; i < dataB.relations.size(); ++i) {
         tempString += dataB.relations.at(i).name + "\n";
         for (unsigned int j = 0; j < dataB.relations.at(i).header.attributeNames.size(); ++j) {
            if (j == dataB.relations.at(i).header.attributeNames.size() - 1) {
               tempString += dataB.relations.at(i).header.attributeNames.at(j);
            }
            else {
               tempString += dataB.relations.at(i).header.attributeNames.at(j) + ", ";
            }
         }
         tempString += "\n ";

         set <Tuple> ::iterator itr;
         for (itr = dataB.relations.at(i).tuples.begin(); itr != dataB.relations.at(i).tuples.end(); ++itr) {
            for (unsigned int f = 0; f < (*itr).attributeValues.size(); ++f) {
               tempString += (*itr).attributeValues.at(f);
            }
            tempString += "\n ";
         }


      }

      return tempString;
   }
};


/* So if I put the functions specific into their respective classes, then make a entry in their respective list everytime im going to use one
* then I can just call list.at(example).function()
*/

class Parser {
public:
   std::string name = "";
   DatalogProgram outputData;
   enum class listType { SchemeType, FactType, RuleType, QueryType };
   listType currentListType = listType::SchemeType;
   int currentListPos = 0;
   int currentListPos2 = 0;
   int queryCount = 0;
   int factListCount = 0;
   bool emptyFacts = false;


   void idlist(vector<token>& input) {
      if (input.at(currentToken).type == "COMMA") {
         ++currentToken;
      }
      else {
         return;
      }

      if (input.at(currentToken).type == "ID") {
         if (currentListType == listType::SchemeType) {
            outputData.schemeList1.at(currentListPos).contentList.push_back(input.at(currentToken).label);
         }
         else if (currentListType == listType::FactType) {
            outputData.factList1.at(currentListPos).contentList.push_back(input.at(currentToken).label);
         }
         else if (currentListType == listType::RuleType) {
            outputData.ruleList1.at(currentListPos).headPredicateContent.push_back(input.at(currentToken).label);
         }
         else if (currentListType == listType::QueryType) {
            outputData.queryList1.at(currentListPos).contentList.push_back(input.at(currentToken).label);
         }

         ++currentToken;
      }
      else throw exception();

      this->idlist(input); //might need to catch exception

      return;
   }

   void scheme(vector<token>& input) {
      Predicate tempPred;
      outputData.schemeList1.push_back(tempPred);
      currentListType = listType::SchemeType;
      currentListPos = outputData.schemeList1.size() - 1;
      std::string name = "";

      if (input.at(currentToken).type == "ID") {
         name = input.at(currentToken).label;
         outputData.schemeList1.at(currentListPos).name = name;
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "LEFT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "ID") {
         outputData.schemeList1.at(currentListPos).contentList.push_back(input.at(currentToken).label);
         ++currentToken;
      }
      else throw exception();

      this->idlist(input);

      if (input.at(currentToken).type == "RIGHT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      return;
   }

   void stringList(vector<token>& input) {
      Parameter tempParam;
      if (input.at(currentToken).type == "COMMA") {
         ++currentToken;
      }
      else {
         return;
      }

      if (input.at(currentToken).type == "STRING") {
         tempParam.value = input.at(currentToken).label;
         outputData.parameterList1.push_back(tempParam);
         outputData.factList1.at(currentListPos).contentList.push_back(input.at(currentToken).label);
         ++currentToken;
      }
      else throw exception();

      this->stringList(input);  //might need to catch exception

      return;
   }

   void schemeList(vector<token>& input) {
      //int tempCurTok = currentToken;
      vector<Predicate> tempSchemeList = outputData.schemeList1;
      listType tempCurrListType = currentListType;

      try {
         this->scheme(input);
      }
      catch (exception& e) {        //handling lambda?????
         //currentToken = tempCurTok;       //probably will only throw in first if else but might as well
         outputData.schemeList1 = tempSchemeList;
         currentListType = tempCurrListType;
         currentListPos = outputData.schemeList1.size() - 1;

         return;
      }

      this->schemeList(input);

      return;
   }

   void fact(vector<token>& input) {
      Predicate tempPred;
      outputData.factList1.push_back(tempPred);
      currentListType = listType::FactType;
      currentListPos = outputData.factList1.size() - 1;
      Parameter tempParam;
      emptyFacts = false;

      if (input.at(currentToken).type == "ID") {
         outputData.factList1.at(currentListPos).name = input.at(currentToken).label;
         ++currentToken;
      }
      else {
         emptyFacts = true;
         throw exception();
      }


      if (input.at(currentToken).type == "LEFT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "STRING") {
         tempParam.value = input.at(currentToken).label;
         outputData.parameterList1.push_back(tempParam);
         outputData.factList1.at(currentListPos).contentList.push_back(input.at(currentToken).label);
         ++currentToken;
      }
      else throw exception();

      this->stringList(input);

      if (input.at(currentToken).type == "RIGHT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "PERIOD") {
         ++currentToken;
      }
      else throw exception();

      return;
   }

   void factList(vector<token>& input) {
      //int tempCurTok = currentToken;
      vector<Predicate> tempFactList = outputData.factList1;
      listType tempCurrListType = currentListType;

      if (factListCount == 0) {
         try {
            this->fact(input);
         }
         catch (exception& e) {        //There is a way to combine the if/else this is contained in i think, im just to tired to work it out so redundancy
            if (emptyFacts) {
               outputData.factList1 = tempFactList;
               currentListType = tempCurrListType;
               currentListPos = outputData.factList1.size() - 1;
               return;
            }
            else {
               throw exception();
            }
         }

      }
      else {
         try {
            this->fact(input);
         }
         catch (exception& e) {        //lambda
            //currentToken = tempCurTok; 
            outputData.factList1 = tempFactList;
            currentListType = tempCurrListType;
            currentListPos = outputData.factList1.size() - 1;

            return;
         }
      }

      ++factListCount;
      this->factList(input);

      --factListCount;
      return;
   }

   void headPredicate(vector<token>& input) {

      if (input.at(currentToken).type == "ID") {
         outputData.ruleList1.at(currentListPos).headPredicateName = input.at(currentToken).label;
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "LEFT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "ID") {
         outputData.ruleList1.at(currentListPos).headPredicateContent.push_back(input.at(currentToken).label);
         ++currentToken;
      }
      else throw exception();

      this->idlist(input);

      if (input.at(currentToken).type == "RIGHT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      return;
   }

   void operatorFunc(vector<token>& input) {
      if (input.at(currentToken).type == "ADD") {
         outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.at(outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.size() - 1) += input.at(currentToken).label;
         ++currentToken;
      }
      else if (input.at(currentToken).type == "MULTIPLY") {
         outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.at(outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.size() - 1) += input.at(currentToken).label;
         ++currentToken;
      }
      else throw exception();
      return;
   }

   void expression(vector<token>& input, bool exp) {
      if (input.at(currentToken).type == "LEFT_PAREN") {
         if (exp) {
            outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.at(outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.size() - 1) += input.at(currentToken).label;
         }
         else {
            outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.push_back(input.at(currentToken).label);
         }
         ++currentToken;
      }
      else throw exception();

      this->parameter(input, true);
      this->operatorFunc(input);
      this->parameter(input, true);

      if (input.at(currentToken).type == "RIGHT_PAREN") {
         outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.at(outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.size() - 1) += input.at(currentToken).label;
         ++currentToken;
      }
      else throw exception();

      return;
   }

   void parameter(vector<token>& input, bool exp) {
      if (input.at(currentToken).type == "STRING") {
         if (exp) {
            outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.at(outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.size() - 1) += input.at(currentToken).label;
            ++currentToken;
         }
         else {
            outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.push_back(input.at(currentToken).label);
            ++currentToken;
         }
      }
      else if (input.at(currentToken).type == "ID") {
         if (exp) {
            outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.at(outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.size() - 1) += input.at(currentToken).label;
            ++currentToken;
         }
         else {
            outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).contentList.push_back(input.at(currentToken).label);
            ++currentToken;
         }
      }
      else {
         if (exp) {
            this->expression(input, true);
         }
         else {
            this->expression(input, false);
         }
      }
      return;
   }

   void parameterList(vector<token>& input) {
      //not sure if all 3 conditions must be good to not use lambda. I've set it up so if first is good, then it verifies the left (not lambda)
      if (input.at(currentToken).type == "COMMA") {
         ++currentToken;

         this->parameter(input, false);

         this->parameterList(input);
      }
      else return;
   }

   void predicate(vector<token>& input) {
      Predicate tempPred;
      Rule tempRule;
      if (outputData.ruleList1.size() == 0) {
         outputData.ruleList1.push_back(tempRule);
      }
      outputData.ruleList1.at(currentListPos).predicateList.push_back(tempPred);
      if (outputData.ruleList1.at(currentListPos).predicateList.size() == 0) {
         currentListPos2 = 0;
      }
      else {
         currentListPos2 = outputData.ruleList1.at(currentListPos).predicateList.size() - 1;
      }


      if (input.at(currentToken).type == "ID") {
         outputData.ruleList1.at(currentListPos).predicateList.at(currentListPos2).name = input.at(currentToken).label;
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "LEFT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      this->parameter(input, false);

      this->parameterList(input);

      if (input.at(currentToken).type == "RIGHT_PAREN") {
         ++currentToken;
      }
      else throw exception();

      return;
   }

   void predicateList(vector<token>& input) {
      if (input.at(currentToken).type == "COMMA") {
         ++currentToken;
         vector<Predicate> tempPredList = outputData.ruleList1.at(currentListPos).predicateList;

         try {
            this->predicate(input);
         }
         catch (exception& e) {
            outputData.ruleList1.at(currentListPos).predicateList = tempPredList;
            currentListPos = outputData.ruleList1.at(currentListPos).predicateList.size() - 1;
            return;
         }

         this->predicateList(input);
      }
      else return;
   }

   void rule(vector<token>& input) {
      Rule tempRule;
      outputData.ruleList1.push_back(tempRule);
      currentListType = listType::RuleType;
      currentListPos = outputData.ruleList1.size() - 1;

      this->headPredicate(input);

      if (input.at(currentToken).type == "COLON_DASH") {
         ++currentToken;
      }
      else throw exception();

      this->predicate(input);
      this->predicateList(input);

      if (input.at(currentToken).type == "PERIOD") {
         ++currentToken;
      }
      else throw exception();

      return;
   }

   void ruleList(vector<token>& input) {
      //int tempCurTok = currentToken;
      vector<Rule> tempRuleList = outputData.ruleList1;
      listType tempCurrListType = currentListType;
      try {
         this->rule(input);
      }
      catch (exception& e) {        //lambda
         //currentToken = tempCurTok;
         outputData.ruleList1 = tempRuleList;
         currentListType = tempCurrListType;
         if (outputData.ruleList1.size() != 0) {
            currentListPos = outputData.ruleList1.size() - 1;
         }

         return;
      }

      this->ruleList(input);

      return;
   }

   void query(vector<token>& input) {
      this->predicate(input);

      if (input.at(currentToken).type == "Q_MARK") {
         ++queryCount;
         ++currentToken;
      }
      else throw exception();

      return;
   }

   void queryList(vector<token>& input) {
      vector<Predicate> tempPredList = outputData.ruleList1.at(currentListPos).predicateList;
      try {
         this->query(input);
      }
      catch (exception& e) {
         outputData.ruleList1.at(currentListPos).predicateList = tempPredList;
         currentListPos = outputData.ruleList1.at(currentListPos).predicateList.size() - 1;

         return;
      }
      this->queryList(input);

      return;
   }

   DatalogProgram parseFile(vector<token>& input) {
      if (input.at(currentToken).type == "SCHEMES") {
         ++currentToken;
      }
      else {
         throw exception();
         //throw input.at(currentToken);
      }

      if (input.at(currentToken).type == "COLON") {
         ++currentToken;
      }
      else throw exception();

      this->scheme(input);
      this->schemeList(input);

      if (input.at(currentToken).type == "FACTS") {
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "COLON") {
         ++currentToken;
      }
      else throw exception();

      this->factList(input);

      if (input.at(currentToken).type == "RULES") {
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "COLON") {
         ++currentToken;
      }
      else throw exception();

      this->ruleList(input);

      if (input.at(currentToken).type == "QUERIES") {
         ++currentToken;
      }
      else throw exception();

      if (input.at(currentToken).type == "COLON") {
         ++currentToken;
      }
      else throw exception();

      this->query(input);
      this->queryList(input);

      for (unsigned int i = outputData.ruleList1.at(outputData.ruleList1.size() - 1).predicateList.size() - (queryCount); i < outputData.ruleList1.at(outputData.ruleList1.size() - 1).predicateList.size(); ++i) {
         outputData.queryList1.push_back(outputData.ruleList1.at(outputData.ruleList1.size() - 1).predicateList.at(i));
      }
      for (int j = 0; j < queryCount; ++j) {
         outputData.ruleList1.at(outputData.ruleList1.size() - 1).predicateList.pop_back();
      }
      if (outputData.ruleList1.at(outputData.ruleList1.size() - 1).predicateList.size() == 0) {
         outputData.ruleList1.pop_back();
      }

      //if an extra letter is placed at end of file, it isnt tokenized
      if (input.at(currentToken).type == "EOF") {
         ++currentToken;
      }
      else throw exception();

      for (unsigned int i = 0; i < outputData.parameterList1.size(); ++i) {
         outputData.domainSet.insert(outputData.parameterList1.at(i).value);
      }

      return outputData;
   }

   std::string toString() const {
      return name;
   }
};




int main(int argc, char* argv[]) {
   VS_MEM_CHECK               // enable memory leak check

   

   //Testing input/output
      if (argc < 2) {
         cerr << "Please provide name of input file";
         return 1;
      }

   ifstream in(argv[1]);
   if (!in) {
      cerr << "Unable to open " << argv[1] << " for input";
      return 2;
   }


   vector<string> lines_in_order;
   std::string line2 = "";
   while (getline(in, line2)) {
      lines_in_order.push_back(line2);
   }

   vector<token> tokenList = lexer(lines_in_order);

   
   Parser data;



   try {
      DatalogProgram outputData = data.parseFile(tokenList);
      outputData.interpret();
      

   }
   catch (exception& e) {
      cout << "Failure!" << "\n";
      cout << "  ";
      cout << tokenList.at(currentToken).toString();
      return 0;
   }






   in.close();
   //out.close();
   return 0;
}