#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "iprange.h"

using namespace std;

int main(int argc, char **argv) {
   IPRange<IPV4_t> ipr;

   vector<string> addrs;
   // test cases that should work
   addrs.push_back(string("192.168.0.199"));
   addrs.push_back(string("192.168.0.[1-20]"));
   addrs.push_back(string("192.*.0.0"));
   addrs.push_back(string("127.*.0.[200-*]"));
   addrs.push_back(string("127.*.0.[*-100]"));

   // testing the error handler
   addrs.push_back(string("127.*.0.[200-100]"));
   addrs.push_back(string("127.b.0.[200-100]"));
   addrs.push_back(string("192.*.100.0.100"));

   vector<string> errorMessages;
   errorMessages.push_back("Successfully Added the IP Address");
   errorMessages.push_back("the address contained something that was not a number.");
   errorMessages.push_back("the address contained a range such that [x,y] where x > y.");
   errorMessages.push_back("the address contained too many parts.");
   errorMessages.push_back("An error ocurred but its message was not defined; please make sure the syntax of your allowed ip adresses is correct.");

   for (vector<string>::iterator addr_iter = addrs.begin(); addr_iter != addrs.end(); ++addr_iter) {
      try {
         ipr.add(*addr_iter);
      } catch (IPRange<IPV4_t>::iprError error) {
         cout  
            << "Error: " << *addr_iter << " "
            << errorMessages.at((int)error) << endl;
      }
   }

   cout << "IPRange object: (" << ipr.size() << ")" <<  endl;
   cout << ipr << endl;

   vector<string> testStrings;
   testStrings.push_back(string("192.168.0.17"));
   testStrings.push_back(string("127.255.0.97"));
   testStrings.push_back(string("128.255.0.97"));

   cout << "Results:" << endl;
   for (vector<string>::iterator sv = testStrings.begin(); sv != testStrings.end(); ++sv) {
      cout << *sv << " is " << ipr.includes(*sv) << endl; 
   }
   cout << endl;

   return EXIT_SUCCESS;
}
