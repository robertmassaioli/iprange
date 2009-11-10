#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include "iprange.h"

using namespace std;

static char* fromConstChar (const char* str);

IPRange::IPRange(int rangeSize) {
   r_size = rangeSize;
}

int IPRange::add(const char* ipaddr) {
   cout << "\nAdding..." << endl;
   // converting from a const char* to a char*
   char *ipadd = fromConstChar(ipaddr);

   // breaking the string up by . separators
   char** gaps = ipToArray(ipadd);
   free(ipadd);

   Range** ranges = generateRanges(gaps);
   free(gaps);    // dont need the gaps anymore

   for (int i = 0; i < r_size; i++) {
      cout << "(" << (int)ranges[i]->start << ", " << (int)ranges[i]->end << ")" << endl;
   }
   // add the ranges up into the tree and merge where required
   //
   delete ranges;
   return SUCCESSFUL_ADD;
}

bool IPRange::includes(const char* idaddr) {
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

char** IPRange::ipToArray(char *str) {
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

Range** IPRange::generateRanges (char** ipArray) {
   int i;
   char* tok;
   Range** ranges = new Range*[r_size];
   Range* tempRange = NULL;
   int minIn, maxIn;
   minIn = maxIn = 0;
   for (i = 0; i < r_size; i++) {
      if (isdigit(*ipArray[i])) {
         minIn = atoi(ipArray[i]);
         if (minIn == INT_MAX || minIn == INT_MIN) return NULL;// return INVALID_NAN;
         if (minIn < 0 || minIn > 0xFF)return NULL;// return INVALID_NOT_IN_RANGE;
         tempRange = new Range((char)minIn, (char)minIn);
      } else {
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
         tempRange = new Range((char)minIn, (char)maxIn);
      }
      ranges[i] = tempRange;
   }

   return ranges;
}
