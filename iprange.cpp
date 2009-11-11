#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include "iprange.h"

using namespace std;

static char* fromConstChar (const char* str);

template <class T>
int IPRange<T>::add(const char* ipaddr) {
   cout << "\nAdding..." << endl;
   // converting from a const char* to a char*
   char *ipadd = fromConstChar(ipaddr);

   // breaking the string up by . separators
   char** gaps = ipToArray(ipadd);
   free(ipadd);

   Unit<T>** ranges = generateRanges(gaps);
   for (int i = 0; i < r_size; i++) {
      free(gaps[i]);
   }
   free(gaps);    // dont need the gaps anymore

   for (int i = 0; i < r_size; i++) {
      cout << *ranges[i] << endl;
   }
   // add the ranges up into the tree and merge where required
   //
   delete ranges;
   return SUCCESSFUL_ADD;
}

template <class T>
bool IPRange<T>::includes(const char* idaddr) {
   // break the address up into its parts (no ranges)
   // search through the tree for a match
   return false;
}

static char* fromConstChar (const char* str) {
   short int strLen = strlen(str);
   char *newstr = (char*)malloc(strLen + 1);
   strncpy(newstr, str, strLen + 1);
   return newstr;
}

template <class T>
char** IPRange<T>::ipToArray(char *str) {
   char *tok = NULL;
   char **gaps = (char**)malloc (sizeof(char*) * r_size);
   short int i = 0;
   short int strLen;
   tok = strtok(str, ".");
   while(NULL != tok) {
      strLen = strlen(tok);
      gaps[i] = (char*)malloc(strLen + 1);
      strncpy(gaps[i], tok, strLen + 1);

      tok = strtok(NULL, ".");
      i++;
   } 

   return gaps;
}

template <class T>
Unit<T>** IPRange<T>::generateRanges (char** ipArray) {
   int i;
   char* tok;
   Unit<T>** ranges = new Unit<T>*[r_size];
   Unit<T>* tempRange = NULL;
   int minIn, maxIn;
   minIn = maxIn = 0;
   for (i = 0; i < r_size; i++) {
      tempRange = NULL;
      if (isdigit(*ipArray[i])) {
         minIn = atoi(ipArray[i]);
         if (minIn == INT_MAX || minIn == INT_MIN) continue;// return INVALID_NAN;
         if (minIn < 0 || minIn > 0xFF) continue;// return INVALID_NOT_IN_RANGE;
         tempRange = new Single<T>(minIn);
      } else if (*ipArray[i] == '[') {
         tok = strtok(ipArray[i], "[]-");
         minIn = -1;
         while (NULL != tok) {
            if (minIn < 0) {
               minIn = atoi(tok);
            } else {
               maxIn = atoi(tok);
            }
            tok = strtok(NULL, "[]-");
         }
         if (maxIn == INT_MAX || maxIn == INT_MIN) return NULL;// return INVALID_NAN;
         if (minIn == INT_MAX || minIn == INT_MIN) return NULL;// return INVALID_NAN;
         if (maxIn < 0 || maxIn > 0xFF) return NULL;// return INVALID_NOT_IN_RANGE;
         if (minIn < 0 || minIn > 0xFF) return NULL;// return INVALID_NOT_IN_RANGE;
         tempRange = new Range<T>(minIn, maxIn);
      } else if (*ipArray[i] == '*') {
         // this is the whole range
      } else {
         // throw an error here
      }
      ranges[i] = tempRange;
   }

   return ranges;
}
