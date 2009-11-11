#ifndef __IPRANGE_DS__
#define __IPRANGE_DS__

#include <vector>
#include <iostream>

#define IPRANGE_SIZE_IPV4  4
#define IPRANGE_SIZE_IPV6  8

#define SUCCESSFUL_ADD        0
#define INVALID_NAN           1
#define INVALID_NOT_IN_RANGE  2

typedef unsigned char IPV4_t;
typedef short int IPV6_t;

template <class T>
class Unit {
   public:
      virtual T min(void) = 0;
      virtual T max(void) = 0;
};

template <class T>
std::ostream& operator<<(std::ostream& output, Unit<T>& u) {
   output << "[" << (int)u.min() << "," << (int)u.max() << "]";
   return output;
}

template <class T>
class Single : public Unit<T> {
   public:
      Single(T s) {single = s;}
      T min(void) {return single;}
      T max(void) {return single;}
   private:
      T single;
};

template <class T>
class Range : public Unit<T> {
   public:
      Range(T s = 0, T e = 0) {start = s; end = e;}
      virtual T min(void) {return start;}
      virtual T max(void) {return end;}
   private:
      T start, end;
};

template <class T>
class IPRange {
   public:
      IPRange () {r_size = sizeof(T) * 4;}
      // returns error code
      int add(const char* ipaddr);
      bool includes(const char* ipaddr);

   private:
      char** ipToArray(char *str);
      Unit<T>** generateRanges (char** ipArray);

      std::vector<std::vector<Unit<T>*> > fullRange;
      short int r_size;
};

#include "iprange.cpp"

#endif
