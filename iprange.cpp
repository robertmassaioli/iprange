#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include "iprange.h"

using namespace std;

IPRange::IPRange(int rangeSize) {
   r_size = rangeSize;
   cout << "Created IPRange (size:" << r_size << ")" << endl;
}

int IPRange::add(const char* ipaddr) {
   // converting from a const char* to a char*
   short int strLen = strlen(ipaddr);
   char *ipadd = (char*)malloc(strLen + 1);
   strncpy(ipadd, ipaddr, strLen + 1);

   // breaking the string up by . separators
   cout << "Breaking up: " << ipaddr << endl;
   char *tok = NULL;
   char **gaps = (char**)malloc (sizeof(char*) * r_size);
   short int i = 0;
   tok = strtok(ipadd, ".");
   while(NULL != tok) {
      strLen = strlen(tok);
      gaps[i] = (char*)malloc(strLen + 1);
      strncpy(gaps[i], tok, strLen + 1);

      cout << gaps[i] << endl;
      tok = strtok(NULL, ".");
      i++;
   } 

   free(ipadd);

   Range* ranges[r_size];
   Range* tempRange = NULL;
   int minIn, maxIn;
   for (i = 0; i < r_size; i++) {
      if (isdigit(*gaps[i])) {
         minIn = atoi(gaps[i]);
         if (minIn == INT_MAX || minIn == INT_MIN) return INVALID_NAN;
         if (minIn < 0 || minIn > 0xFF) return INVALID_NOT_IN_RANGE;
         cout << "Single: " << minIn << endl;
         tempRange = new Range((char)minIn, (char)minIn);
      } else {
         cout << "Range: " << gaps[i] << endl;
         tok = strtok(gaps[i], "[]-");
         minIn = -1;
         while (NULL != tok) {
            if (minIn < 0) {
               minIn = atoi(tok);
            } else {
               maxIn = atoi(tok);
            }
            tok = strtok(NULL, "[]-");
         }
         if (maxIn == INT_MAX || maxIn == INT_MIN) return INVALID_NAN;
         if (minIn == INT_MAX || minIn == INT_MIN) return INVALID_NAN;
         if (maxIn < 0 || maxIn > 0xFF) return INVALID_NOT_IN_RANGE;
         if (minIn < 0 || minIn > 0xFF) return INVALID_NOT_IN_RANGE;
         tempRange = new Range((char)minIn, (char)maxIn);
      }
      ranges[i] = tempRange;
   }
   free(gaps);    // dont need the gaps anymore

   // add the ranges up into the tree and merge where required
   //
   return SUCCESSFUL_ADD;
}

bool IPRange::includes(const char* idaddr) {
   // break the address up into its parts (no ranges)
   // search through the tree for a match
   return false;
}
