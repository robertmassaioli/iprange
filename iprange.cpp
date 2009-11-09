#include <iostream>
#include <cstdlib>
#include <cstring>
#include "iprange.h"

using namespace std;

void IPRange::add(const char* ipaddr) {
   // break the address up into ranges
   short int strLen = strlen(ipaddr);
   char *ipadd = (char*)malloc(strLen + 1);
   strncpy(ipadd, ipaddr, strLen + 1);

   cout << "Breaking up: " << ipaddr << endl;
   char *tok = NULL;
   tok = strtok(ipadd, ".");
   while(NULL != tok) {
      cout << tok << endl;

      tok = strtok(NULL, ".");
   } 

   // add the ranges up into the tree and merge where required
}

bool IPRange::includes(const char* idaddr) {
   // break the address up into its parts (no ranges)
   // search through the tree for a match
   return false;
}
