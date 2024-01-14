/** @file NormalizeValue.h
 * Contains the normalizeValue template function for converting from normalized types to normalized types.
 */

#ifndef CORE_NORMALIZEVALUE_H_INCLUDED
#define CORE_NORMALIZEVALUE_H_INCLUDED

#include "common.h"

template<typename TypeFrom, typename TypeTo, bool b> struct NormalizerIf;
template<typename TypeFrom, typename TypeTo> struct NormalizerIf<TypeFrom,TypeTo,true>{
    inline static TypeTo normalizeValue(const TypeFrom& t){
        TypeTo ret = t;
        ret <<= ((sizeof(TypeTo)-sizeof(TypeFrom))*8);
        return ret;
    }
};
template<typename TypeFrom, typename TypeTo> struct NormalizerIf<TypeFrom,TypeTo,false>{
    inline static TypeTo normalizeValue(const TypeFrom& t){
        TypeFrom ret = t;
        ret >>= ((sizeof(TypeFrom)-sizeof(TypeTo))*8);
        return ret;
    }
};

template<typename TypeFrom, typename TypeTo> struct Normalizer {
    inline static TypeTo normalizeValue(const TypeFrom& t);
};
template<typename Type> struct Normalizer<Type,Type> {
    inline static Type normalizeValue(const Type& t){return t;}
};
template<typename TypeFrom> struct Normalizer<TypeFrom,float> {
    inline static float normalizeValue(const TypeFrom& t){return t/(pow2(sizeof(TypeFrom)*(is_signed<TypeFrom>::value?7:8))-1.f);}
};
template<typename TypeFrom> struct Normalizer<TypeFrom,double> {
    inline static double normalizeValue(const TypeFrom& t){return t/(pow2(sizeof(TypeFrom)*(is_signed<TypeFrom>::value?7:8))-1.0);}
};
template<typename TypeTo> struct Normalizer<float, TypeTo> {
    inline static TypeTo normalizeValue(const float& t){return (TypeTo)(clamp(t,is_signed<TypeTo>::value?-1.f:0.f, 1.f)*(pow2(sizeof(TypeTo)*(is_signed<TypeTo>::value?7:8))-1.f));}
};
template<typename TypeTo> struct Normalizer<double, TypeTo> {
    inline static TypeTo normalizeValue(const double& t){return (TypeTo)(clamp(t,is_signed<TypeTo>::value?-1.0:0.0, 1.0)*(pow2(sizeof(TypeTo)*(is_signed<TypeTo>::value?7:8))-1.0));}
};
template<> struct Normalizer<float, float> {
    inline static float normalizeValue(const float& t){return t;}
};
template<> struct Normalizer<double, double> {
    inline static double normalizeValue(const double& t){return t;}
};
template<> struct Normalizer<float, double> {
    inline static double normalizeValue(const float& t){return (double)t;}
};
template<> struct Normalizer<double, float> {
    inline static float normalizeValue(const double& t){return (float)t;}
};

/** Template function which converts a normalized value from one primitive type to another.
 * For integer values, this will interpret min value to max value as -1 to 1. For unsigned integer values, this
 * will interpret 0 to max value as 0 to 1. Float values remain unchanged.<br><br>
 * <b>Examples:</b>
 * @code
 * normalizeValue<ushort,uint>(ushort_max) = uint_max
 * normalizeValue<ushort,float>(ushort_max) = 1
 * normalizeValue<float,uint>(1) = uint_max
 * @endcode
 */
template <typename TypeFrom, typename TypeTo> inline TypeTo normalizeValue(const TypeFrom& t){
        return Normalizer<TypeFrom,TypeTo>::normalizeValue(t);
    }

template<typename TypeFrom, typename TypeTo> inline TypeTo Normalizer<TypeFrom,TypeTo>::normalizeValue(const TypeFrom& t){
    if(is_signed<TypeFrom>::value == is_signed<TypeTo>::value)
        return NormalizerIf<TypeFrom, TypeTo, (sizeof(TypeTo) > sizeof(TypeFrom))>::normalizeValue(t);
    else if(is_signed<TypeFrom>::value) return normalizeValue<float, TypeTo>(normalizeValue<TypeFrom, float>(t)*0.5f+0.5f);
    else return normalizeValue<float, TypeTo>(normalizeValue<TypeFrom, float>(t)*2.f-1.f);
}

#endif // CORE_NORMALIZEVALUE_H_INCLUDED
