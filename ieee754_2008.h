#include "common.h"

#ifndef CORE_IEEE754_2008_H_INCLUDED
#define CORE_IEEE754_2008_H_INCLUDED

namespace ieee754_util {

union IEEE754_half {
    enum {MANTISSA=10, E_MIN=-14, E_MAX=15, E_NAN=31}; typedef uint16_t Type;
    struct{
        uint16_t mantissa:10;
        uint16_t e:5;
        uint16_t sign:1;
    };
    uint16_t binary;
};

union IEEE754_float {
    enum {MANTISSA=23, E_MIN=-126, E_MAX=127, E_NAN=255}; typedef uint32_t Type;
    struct{
        uint32_t mantissa:23;
        uint32_t e:8;
        uint32_t sign:1;
    };
    uint32_t binary;
};

union IEEE754_double {
    enum {MANTISSA=52, E_MIN=-1022, E_MAX=1023, E_NAN=2047}; typedef uint64_t Type;
    struct{
        uint64_t mantissa:52;
        uint64_t e:11;
        uint64_t sign:1;
    };
    uint64_t binary;
};

template<class IEEE754> IEEE754 convertToIEEE754(float val){
    IEEE754 ret; ret.sign = (val < 0)?1:0; if(ret.sign) val = -val;
    if(val != val){ret.e = (typename IEEE754::Type)IEEE754::E_NAN; typename IEEE754::Type ones = (typename IEEE754::Type)0xFFFFFFFFFFFFFFFFULL; ret.mantissa = ones; return ret;}
    if(val == FLT_INF(float)){ret.e = IEEE754::E_NAN; ret.mantissa = 0; return ret;}
    if(val == 0){ret.e = 0; ret.mantissa = 0; return ret;}
    int e; val = frexp(val, &e); int exp = clamp<int>(e, IEEE754::E_MIN, IEEE754::E_MAX); val = ldexp(val, IEEE754::MANTISSA+((exp != IEEE754::E_MIN)?1:0)+e-exp);
    ret.e = exp-IEEE754::E_MIN; ret.mantissa = static_cast<typename IEEE754::Type>(val); return ret;
}

template<class IEEE754> IEEE754 convertToIEEE754(double val){
    IEEE754 ret; ret.sign = (val < 0)?1:0; if(ret.sign) val = -val;
    if(val != val){ret.e = (typename IEEE754::Type)IEEE754::E_NAN; typename IEEE754::Type ones = (typename IEEE754::Type)0xFFFFFFFFFFFFFFFFULL; ret.mantissa = ones; return ret;}
    if(val == FLT_INF(double)){ret.e = IEEE754::E_NAN; ret.mantissa = 0; return ret;}
    if(val == 0){ret.e = 0; ret.mantissa = 0; return ret;}
    int e; val = frexp(val, &e); int exp = clamp<int>(e, IEEE754::E_MIN, IEEE754::E_MAX); val = ldexp(val, IEEE754::MANTISSA+((exp != IEEE754::E_MIN)?1:0)+e-exp);
    ret.e = exp-IEEE754::E_MIN; ret.mantissa = static_cast<typename IEEE754::Type>(val); return ret;
}

template<class IEEE754> float convertToFloat(IEEE754 val){
    if(val.e == IEEE754::E_NAN) return ((val.mantissa)?FLT_NAN(float):((val.sign)?-FLT_INF(float):FLT_INF(float)));
    float ret;
    if(val.e == 0) ret = ldexp(float(val.mantissa), IEEE754::E_MIN-IEEE754::MANTISSA);
    else ret = ldexp(float(val.mantissa)+ldexp(1.f, IEEE754::MANTISSA), val.e+IEEE754::E_MIN-IEEE754::MANTISSA-1);
    return ((val.sign)?-ret:ret);
}

template<class IEEE754> double convertToDouble(IEEE754 val){
    if(val.e == IEEE754::E_NAN) return ((val.mantissa)?FLT_NAN(double):((val.sign)?-FLT_INF(double):FLT_INF(double)));
    double ret;
    if(val.e == 0) ret = ldexp(double(val.mantissa), IEEE754::E_MIN-IEEE754::MANTISSA);
    else ret = ldexp(double(val.mantissa)+ldexp(1.0, IEEE754::MANTISSA), val.e+IEEE754::E_MIN-IEEE754::MANTISSA-1);
    return ((val.sign)?-ret:ret);
}

}

#endif // CORE_IEEE754_2008_H_INCLUDED
