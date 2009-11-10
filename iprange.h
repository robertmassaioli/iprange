#ifndef __IPRANGE_DS__
#define __IPRANGE_DS__

#include <vector>

#define IPRANGE_SIZE_IPV4  4
#define IPRANGE_SIZE_IPV6  8

#define SUCCESSFUL_ADD        0
#define INVALID_NAN           1
#define INVALID_NOT_IN_RANGE  2

class Range {
   public:
      Range(unsigned char s = 0, unsigned char e = 0) {start = s; end = e;}
      unsigned char start, end;
};

class IPRange {
   public:
      IPRange (int rangeSize = IPRANGE_SIZE_IPV4);
      // returns error code
      int add(const char* ipaddr);
      bool includes(const char* ipaddr);

   private:
      char** ipToArray(char *str);
      Range** generateRanges (char** ipArray);

      std::vector<std::vector<Range*> > fullRange;
      short int r_size;
};

#endif
