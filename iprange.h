#ifndef __IPRANGE_DS__
#define __IPRANGE_DS__

#include <vector>

class range {
   public:
      unsigned char start, end;
};

class IPRange {
   public:
      void add(const char* ipaddr);
      bool includes(const char* ipaddr);
   private:
      std::vector<std::vector<range*> > fullRange;
};

#endif
