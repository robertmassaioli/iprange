#ifndef __IPRANGE_DS__
#define __IPRANGE_DS__

#include <string>
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
      int add(std::string& ipaddr);
      bool includes(std::string& ipaddr);
      int size(void) { return fullRange.size(); }

      // the get and remove functions (for compatibility with fuppes)
      const std::vector<Unit<T>*>* get(int i) { return (const std::vector<Unit<T>*>*)fullRange.at(i); }
      void remove(int i) {
         std::vector<Unit<T>*>* vec = fullRange.at(i);
         while (!vec->empty()) {
            delete vec->back();
            vec->pop_back();
         }
         fullRange.erase(fullRange.begin() + i);
         delete vec;
      }
      void clear(void);

      enum iprError {
         SUCCESSFUL_ADD = 0,
         INVALID_NAN,
         INVALID_RANGE,
         INVALID_SIZE_ADDRESS,
         LARGEST_ERROR
      };

   private:
      std::vector<std::string*>* ipToArray(std::string& str);
      std::vector<Unit<T>* >* generateRanges (std::vector<std::string*>* ipArray);

      std::vector<std::vector<Unit<T>* >* > fullRange;
      short int r_size;
};

//
// The toString code
//

template <class T>
std::ostream& operator<<(std::ostream& output, Unit<T>& u) {
   if (u.min() == u.max()) {
      output << (int)u.min();
   } else {
      output << "[" << (int)u.min() << "," << (int)u.max() << "]";
   }
   return output;
}

template <class T>
std::ostream& operator<<(std::ostream& output, const std::vector<Unit<T>*>& ipar) {
   for(typename std::vector<Unit<T>*>::const_iterator iter = ipar.begin(); iter != ipar.end(); ++iter) {
      output << **iter;
      if (iter + 1 != ipar.end()) {
         output << ".";
      }
   }
   return output;
}

template <class T>
std::ostream& operator<<(std::ostream& output, IPRange<T>& ipr) {
   for (int i = 0; i < ipr.size(); ++i) {
      output << *ipr.get(i) << std::endl;
   }
   return output;
}

#include "iprange.tcc"

#endif
