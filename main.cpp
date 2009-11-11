#include <iostream>
#include <cstdlib>
#include "iprange.h"

using namespace std;

int main(int argc, char **argv) {
   IPRange<IPV4_t>* ipr = new IPRange<IPV4_t>();

   ipr->add("192.168.0.199");
   ipr->add("192.168.0.[1-20]");
   ipr->add("192.*.0.0");
   ipr->add("127.*.0.[200-*]");

   cout << "192.168.0.17 is " << ipr->includes("192.168.0.17") << endl;
   cout << "192.168.0.97 is " << ipr->includes("192.168.0.97") << endl;

   delete ipr;

   return EXIT_SUCCESS;
}
