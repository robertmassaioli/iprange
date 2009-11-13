#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include <string>
#include "iprange.h"

using namespace std;

//
// Helper Function Definitions
//

//static char* fromConstChar (const char* str);
template <class T>
static bool isInside(vector<Unit<T>* >* unit, vector<T>* test);

//
// Implementation of Class Functions
//

template <class T>
IPRange<T>::~IPRange() {
   clear();
}

template <class T>
void IPRange<T>::clear(void) {
   vector<Unit<T>* >* temp;

   while(!fullRange.empty()) {
      temp = fullRange.back();
      while (!temp->empty()) {
         delete temp->back();
         temp->pop_back();
      }
      delete temp;
      fullRange.pop_back();
   }
}

template <class T>
int IPRange<T>::add(string& ipadd) {
   // breaking the string up by . separators
   vector<string*>* gaps = ipToArray(ipadd);

   if (gaps == NULL || (short int)gaps->size() != r_size) throw INVALID_SIZE_ADDRESS;

   vector<Unit<T>*>* ranges = NULL;
   try {
      ranges = generateRanges(gaps);
   } catch (IPRange<T>::iprError error) {
      if (gaps != NULL) delete gaps;
      throw; // catch this error and throw it again
   }
   typename vector<string*>::iterator citer;
   for (citer = gaps->begin(); citer != gaps->end(); ++citer) {
      delete *citer;
   }
   delete gaps;    // dont need the gaps anymore
   gaps = NULL;

   // add the new result to the list
   fullRange.push_back(ranges);

   return SUCCESSFUL_ADD;
}

template <class T>
bool IPRange<T>::includes(string& idadd) {
   // break the address up into its parts (no ranges)
   char* cstr_idadd = new char[idadd.size() + 1];
   strcpy(cstr_idadd, idadd.c_str());
   char* tok = strtok(cstr_idadd, ".:");
   int temp;
   vector<T> units;
   while(tok != NULL) {
      if (!isdigit(*tok)) throw INVALID_NAN;
      temp = atoi(tok);
      if (temp == INT_MAX || temp == INT_MIN) throw INVALID_NAN;
      units.push_back((T)temp);
      tok = strtok(NULL, ".:");
   }
   delete[] cstr_idadd;

   // search through the tree for a match
   typename vector<vector<Unit<T>* >* >::iterator fi;
   for(fi = fullRange.begin(); fi != fullRange.end(); ++fi) {
      if (isInside(*fi, &units)) return true;
   }

   return false;
}

template <class T>
vector<string*>* IPRange<T>::ipToArray(string& str) {
   char *tok = NULL;
   vector<string*>* gaps = new vector<string*>();

   short int i = 0;
   short int strLen;
   char* cstr_str = new char[str.size() + 1];
   strcpy(cstr_str, str.c_str());
   tok = strtok(cstr_str, ".");
   while(NULL != tok) {
      strLen = strlen(tok);
      gaps->push_back(new string(tok));

      tok = strtok(NULL, ".");
      i++;
   } 
   delete[] cstr_str;

   return gaps;
}

template <class T>
vector<Unit<T>*>* IPRange<T>::generateRanges (vector<string*>* ipArray) {
   vector<Unit<T>*>* ranges = new vector<Unit<T>*>();

   Unit<T>* tempRange = NULL;
   int minIn, maxIn;
   minIn = maxIn = 0;
   vector<string*>::iterator citer;
   char firstChar = 0;
   for (citer = ipArray->begin(); citer != ipArray->end(); ++citer) {
      firstChar = (*(*citer))[0];
      if (isdigit(firstChar)) {
         minIn = atoi((*citer)->data());
         if (minIn == INT_MAX || minIn == INT_MIN) throw INVALID_NAN;
         if (minIn < 0 || minIn > 0xFF) throw INVALID_RANGE;
         tempRange = new Single<T>(minIn);
      } else if (firstChar == '[') {
         char* tok;
         char* cstr_citer = new char[(*citer)->size() + 1];
         strcpy(cstr_citer, (*citer)->c_str());

         tok = strtok(cstr_citer, "[]-");
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
         delete[] cstr_citer;

         // all of these return NULL's should be thrown errors
         if (maxIn == INT_MAX || maxIn == INT_MIN) throw INVALID_NAN;
         if (minIn == INT_MAX || minIn == INT_MIN) throw INVALID_NAN;
         if ((T)maxIn < (T)minIn) throw INVALID_RANGE;
         tempRange = new Range<T>(minIn, maxIn);
      } else if (firstChar == '*') {
         // this is the whole range
         tempRange = new FullRange<T>();
      } else {
         throw INVALID_NAN;
      }
      ranges->push_back(tempRange);
   }

   return ranges;
}

//
// Implmentation of the Helper Functions
//

/*static char* fromConstChar (const char* str) {
   short int strLen = strlen(str);
   char *newstr = (char*)malloc(strLen + 1);
   strncpy(newstr, str, strLen + 1);
   return newstr;
}*/

template <class T>
static bool isInside(vector<Unit<T>* >* unit, vector<T>* test) {
   typename vector<Unit<T>* >::const_iterator unit_iter = unit->begin();
   typename vector<T>::const_iterator test_iter = test->begin();

   if (unit->size() != test->size()) return false;

   while (unit_iter != unit->end() && test_iter != test->end()) {
      if (*test_iter < (*unit_iter)->min() || *test_iter > (*unit_iter)->max()) return false;
      ++test_iter; 
      ++unit_iter;
   }
   return true;
}
