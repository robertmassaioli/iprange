#ifndef __IPRANGE_DS__
#define __IPRANGE_DS__

#include <vector>
#include <iostream>

typedef unsigned char      IPV4_t;
typedef unsigned short int IPV6_t;

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
class FullRange : public Unit<T> {
   public:
      T min(void) {return (T)0;}
      T max(void) {return ~((T)0);}
};

template <class T>
class IPRange {
   public:
      IPRange () {r_size = sizeof(T) * 4;}
      ~IPRange();
      // returns error code
      int add(const char* ipaddr);
      bool includes(const char* ipaddr);

      enum iprError {
         SUCCESSFUL_ADD = 0,
         INVALID_NAN,
         INVALID_RANGE,
         INVALID_SIZE_ADDRESS,
         LARGEST_ERROR
      };

   private:
      std::vector<char*>* ipToArray(char *str);
      std::vector<Unit<T>* >* generateRanges (std::vector<char*>* ipArray);

      std::vector<std::vector<Unit<T>* >* > fullRange;
      short int r_size;

      bool isDirty;
};

#include "iprange.cpp"

#endif
