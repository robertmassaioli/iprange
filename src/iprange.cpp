#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include "iprange.h"

using namespace std;

//
// Helper Function Definitions
//

static char* fromConstChar (const char* str);
template <class T>
static bool isInside(vector<Unit<T>* >* unit, vector<T>* test);

//
// Implementation of Class Functions
//

template <class T>
IPRange<T>::~IPRange() {
   typename vector< Unit<T>* >::iterator ipRangeIter;
   typename vector<vector<Unit<T>* >* >::iterator fullIter;

   for (fullIter = fullRange.begin(); fullIter != fullRange.end(); fullIter++) {
      for (ipRangeIter = (*fullIter)->begin(); ipRangeIter != (*fullIter)->end(); ipRangeIter++) {
         free (*ipRangeIter);
      }
      free (*fullIter);
   }
}

template <class T>
int IPRange<T>::add(const char* ipaddr) {
   cout << "\nAdding " << ipaddr << "..." << endl;
   // converting from a const char* to a char*
   char *ipadd = fromConstChar(ipaddr);

   // breaking the string up by . separators
   vector<char*>* gaps = ipToArray(ipadd);
   free(ipadd);

   vector<Unit<T>*>* ranges = generateRanges(gaps);
   typename vector<char*>::iterator citer;
   for (citer = gaps->begin(); citer != gaps->end(); citer++) {
      free(*citer);
   }
   delete gaps;    // dont need the gaps anymore

   typename vector<Unit<T>*>::iterator riter;
   cout << "r_size: " << r_size << endl;
   cout << "Size: " << ranges->size() << endl;
   for (riter = ranges->begin(); riter != ranges->end(); riter++) {
      cout << *(Unit<T>*)*riter << endl;
   }
   // add the new result to the list
   fullRange.push_back(ranges);
   isDirty = true;

   return SUCCESSFUL_ADD;
}

template <class T>
bool IPRange<T>::includes(const char* idaddr) {
   if (isDirty) {
      // sort the data
   }

   // break the address up into its parts (no ranges)
   char* idadd = fromConstChar(idaddr);
   char* tok = strtok(idadd, ".:");
   int temp;
   vector<T> units;
   units.clear();
   while(tok != NULL) {
      if (!isdigit(*tok)) return false;
      temp = atoi(tok);
      if (temp == INT_MAX || temp == INT_MIN) return false;
      units.push_back((T)temp);
      tok = strtok(NULL, ".:");
   }
   free (idadd);

   // search through the tree for a match
   typename vector<vector<Unit<T>* >* >::iterator fullIter;
   for(fullIter = fullRange.begin(); fullIter != fullRange.end(); fullIter++) {
      if (isInside(*fullIter, &units)) return true;
   }

   return false;
}

template <class T>
vector<char*>* IPRange<T>::ipToArray(char *str) {
   char *tok = NULL;
   vector<char*>* gaps = new vector<char*>(r_size);
   gaps->clear();
   short int i = 0;
   short int strLen;
   tok = strtok(str, ".");
   while(NULL != tok) {
      strLen = strlen(tok);
      gaps->push_back((char*)malloc(strLen + 1));
      strncpy((*gaps)[i], tok, strLen + 1);

      tok = strtok(NULL, ".");
      i++;
   } 

   return gaps;
}

template <class T>
vector<Unit<T>*>* IPRange<T>::generateRanges (vector<char*>* ipArray) {
   char* tok;
   vector<Unit<T>*>* ranges = new vector<Unit<T>*>(r_size);
   ranges->clear();

   Unit<T>* tempRange = NULL;
   int minIn, maxIn;
   minIn = maxIn = 0;
   vector<char*>::iterator citer;
   for (citer = ipArray->begin(); citer != ipArray->end(); citer++) {
      tempRange = NULL;
      if (isdigit(*(*citer))) {
         minIn = atoi(*citer);
         if (minIn == INT_MAX || minIn == INT_MIN) continue;// return INVALID_NAN;
         if (minIn < 0 || minIn > 0xFF) continue;// return INVALID_NOT_IN_RANGE;
         tempRange = new Single<T>(minIn);
      } else if (*(*citer) == '[') {
         tok = strtok(*citer, "[]-");
         minIn = -1;
         bool first = true;
         while (NULL != tok) {
            if (first) {
               if (*tok != '*' && *tok != 0) {
                  minIn = atoi(tok);
               } else {
                  minIn = 0;
               }
               first = false;
            } else {
               if (*tok != '*' && *tok != 0) {
                  maxIn = atoi(tok);
               } else {
                  maxIn = ~((T)0);
               }
            }
            tok = strtok(NULL, "[]-");
         }
         // all of these return NULL's should be thrown errors
         if (maxIn == INT_MAX || maxIn == INT_MIN) return NULL;// return INVALID_NAN;
         if (minIn == INT_MAX || minIn == INT_MIN) return NULL;// return INVALID_NAN;
         if ((T)maxIn < (T)minIn) return NULL;
         tempRange = new Range<T>(minIn, maxIn);
      } else if (*(*citer) == '*') {
         // this is the whole range
         tempRange = new FullRange<T>();
      } else {
         // throw an error here
         cout << "Error Place 2" << endl;
      }
      ranges->push_back(tempRange);
   }

   return ranges;
}

//
// Implmentation of the Helper Functions
//

static char* fromConstChar (const char* str) {
   short int strLen = strlen(str);
   char *newstr = (char*)malloc(strLen + 1);
   strncpy(newstr, str, strLen + 1);
   return newstr;
}

template <class T>
static bool isInside(vector<Unit<T>* >* unit, vector<T>* test) {
   typename vector<Unit<T>* >::const_iterator unit_iter = unit->begin();
   typename vector<T>::const_iterator test_iter = test->begin();

   while (unit_iter != unit->end() && test_iter != test->end()) {
      if (*test_iter < (*unit_iter)->min() || *test_iter > (*unit_iter)->max()) return false;
      test_iter++; 
      unit_iter++;
   }
   return true;
}
