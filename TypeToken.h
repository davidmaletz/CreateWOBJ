/** @file TypeToken.h
 * Contains type tokens, a way to name primitive types.
 */

#ifndef CORE_TYPETOKEN_H_INCLUDED
#define CORE_TYPETOKEN_H_INCLUDED

#include "common.h"

/** A token value representing a primitive type. */
enum TypeToken {
    /** Character type (char). */ TYPE_CHAR = 0,
    /** Unsigned byte/character type (uchar). */ TYPE_UCHAR = 1,
    /** Short integer type (short). */ TYPE_SHORT = 2,
    /** Unsigned short integer type (ushort). */ TYPE_USHORT = 3,
    /** Integer type (int). */ TYPE_INT = 4,
    /** Unsigned int type (uint). */ TYPE_UINT = 5,
    /** 16-bit floating point type (half_float). @see half_float */ TYPE_HALF_FLOAT = 6,
    /** Floating point type (float). */ TYPE_FLOAT = 7,
};

/** Template function that returns the TypeToken for a specific primitive type.
 * For example, typeToken<uint>() returns TYPE_UINT.
 * If there is no type token for the template type, it will return NO_TYPE.
 * @see TypeToken @see TYPE_Get
 */
template<typename T> inline TypeToken typeToken(){return -1;}
template<> inline TypeToken typeToken<char>(){return TYPE_CHAR;}
template<> inline TypeToken typeToken<uchar>(){return TYPE_UCHAR;}
template<> inline TypeToken typeToken<short>(){return TYPE_SHORT;}
template<> inline TypeToken typeToken<ushort>(){return TYPE_USHORT;}
template<> inline TypeToken typeToken<int>(){return TYPE_INT;}
template<> inline TypeToken typeToken<uint>(){return TYPE_UINT;}
template<> inline TypeToken typeToken<float>(){return TYPE_FLOAT;}

/** Gets the type of a staticly known TypeToken.
 * For example, TYPE_Get<TYPE_INT>::Type is a typedef of int.
 * If the type token is invalid, TYPE_Get will return a compiler error.
 * @see TypeToken @see typeToken
 */
template<TypeToken Type> struct TYPE_Get {};
template<> struct TYPE_Get<TYPE_CHAR> {typedef char Type;};
template<> struct TYPE_Get<TYPE_UCHAR> {typedef uchar Type;};
template<> struct TYPE_Get<TYPE_SHORT> {typedef short Type;};
template<> struct TYPE_Get<TYPE_USHORT> {typedef ushort Type;};
template<> struct TYPE_Get<TYPE_INT> {typedef int Type;};
template<> struct TYPE_Get<TYPE_UINT> {typedef uint Type;};
template<> struct TYPE_Get<TYPE_FLOAT> {typedef float Type;};

#endif // CORE_TYPETOKEN_H_INCLUDED
