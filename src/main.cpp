#include <iostream>
#include <cstdlib>
#include "iprange.h"

using namespace std;

int main(int argc, char **argv) {
   IPRange<IPV4_t>* ipr = new IPRange<IPV4_t>();

   vector<const char*> addrs;
   // test cases that should work
   addrs.push_back("192.168.0.199");
   addrs.push_back("192.168.0.[1-20]");
   addrs.push_back("192.*.0.0");
   addrs.push_back("127.*.0.[200-*]");
   addrs.push_back("127.*.0.[*-100]");

   // testing the error handler
   addrs.push_back("127.*.0.[200-100]");
   addrs.push_back("127.b.0.[200-100]");
   addrs.push_back("192.*.100.0.100");

   vector<const char*> errorMessages;
   errorMessages.push_back("Successfully Added the IP Address");
   errorMessages.push_back("the address contained something that was not a number.");
   errorMessages.push_back("the address contained an range such that [x,y] where x > y.");
   errorMessages.push_back("the address contained too many parts.");
   errorMessages.push_back("An error ocurred but its message was not defined; please make sure the syntax of your allowed ip adresses is correct.");

   vector<const char*>::const_iterator addr_iter;
   for (addr_iter = addrs.begin(); addr_iter != addrs.end(); addr_iter++) {
      try {
         ipr->add(*addr_iter);
      } catch (IPRange<IPV4_t>::iprError error) {
         cout << "Error: " << *addr_iter << " ";
         cout << errorMessages.at((int)error) << endl;
      }
   }

   cout << "192.168.0.17 is " << ipr->includes("192.168.0.17") << endl;
   cout << "127.255.0.97 is " << ipr->includes("127.255.0.97") << endl;
   cout << "128.255.0.97 is " << ipr->includes("128.255.0.97") << endl;

   delete ipr;

   return EXIT_SUCCESS;
}
