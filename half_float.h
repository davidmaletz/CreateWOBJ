/** @file half_float.h
 * Adds support and all functions needed for the primitive type half_float, a 16-bit floating point value.
 */

#ifndef CORE_HALF_FLOAT_H_INCLUDED
#define CORE_HALF_FLOAT_H_INCLUDED

#include "NormalizeValue.h"
#include "TypeToken.h"
#include "ieee754_2008.h"

/** The new "primitive" type, half_float. */
struct half_float {
    ieee754_util::IEEE754_half value;
    inline half_float(){}
    /** Creates a half_float from a float. */
    inline half_float(float f) : value(ieee754_util::convertToIEEE754<ieee754_util::IEEE754_half>(f)){}
    inline half_float(double f) : value(ieee754_util::convertToIEEE754<ieee754_util::IEEE754_half>(f)){}

	inline half_float& operator+=(half_float f){*this = float(*this)+float(f); return *this;}
	inline half_float& operator-=(half_float f){*this = float(*this)-float(f); return *this;}
	inline half_float& operator*=(half_float f){*this = float(*this)*float(f); return *this;}
	inline half_float& operator/=(half_float f){*this = float(*this)/float(f); return *this;}
	inline half_float operator+(half_float f) const{return float(*this)+float(f);}
	inline half_float operator-(half_float f) const{return float(*this)-float(f);}
	inline half_float operator*(half_float f) const{return float(*this)*float(f);}
	inline half_float operator/(half_float f) const{return float(*this)/float(f);}
	inline half_float operator-() const{half_float h = *this; h.value.sign = !value.sign; return h;}
	inline bool operator==(half_float f) const{return f.value.binary == value.binary;}
	inline bool operator!=(half_float f) const{return f.value.binary != value.binary;}

    /** Converts this half float to a float. */
    inline operator float() const {return ieee754_util::convertToFloat(value);}
    inline operator double() const {return ieee754_util::convertToDouble(value);}
};

template<typename TypeFrom, typename TypeTo> struct Normalizer ;

template<> struct Normalizer<half_float, float> {
    inline static float normalizeValue(const half_float& t){return (float)t;}
};
template<> struct Normalizer<float, half_float> {
    inline static half_float normalizeValue(const float& t){return half_float(t);}
};

inline half_float average(half_float c1, half_float c2){return half_float(average(float(c1), float(c2)));}

template<> inline TypeToken typeToken<half_float>(){return TYPE_HALF_FLOAT;}
template<> struct TYPE_Get<TYPE_HALF_FLOAT> {typedef half_float Type;};

#endif // CORE_HALF_FLOAT_H_INCLUDED
