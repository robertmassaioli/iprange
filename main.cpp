#include <iostream>
#include <cstdlib>
#include "iprange.h"

using namespace std;

int main(int argc, char **argv) {
   IPRange* ipr = new IPRange();

   ipr->add("192.168.0.199");
   ipr->add("192.168.0.[1-20]");

   cout << "192.168.0.17 is " << ipr->includes("192.168.0.17") << endl;
   cout << "192.168.0.97 is " << ipr->includes("192.168.0.97") << endl;

   delete ipr;

   return EXIT_SUCCESS;
}
