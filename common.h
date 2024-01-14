/** @file common.h
 * Contains common includes, defines, macros and inline functions.
 * Most files will directly or indirectly include common.h
 */

#ifndef CORE_COMMON_H_INCLUDED
#define CORE_COMMON_H_INCLUDED

#include <limits>

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

#ifdef _64BIT
typedef uint64_t size_t;
typedef uint64_t ptr_size_t;
typedef int64_t ptr_diff_t;
#else
typedef unsigned size_t;
typedef unsigned long ptr_size_t;
typedef long ptr_diff_t;
#endif

/**An unsigned char (value from 0 to uchar_max inclusive).*/
typedef unsigned char uchar;
/**An unsigned short (value from 0 to ushort_max inclusive).*/
typedef unsigned short ushort;
/**An unsigned integer (value from 0 to uint_max inclusive).*/
typedef unsigned int uint;
/**An unsigned long integer (value from 0 to ulong_max inclusive).*/
typedef unsigned long ulong;
/**A signed long long integer.*/
typedef long long longlong;
/**An unsigned long long integer (value from 0 to ulonglong_max inclusive).*/
typedef unsigned long long ulonglong;

/** The maximum value for a uchar type. For a 1 byte uchar, this value is 255.*/
const uchar uchar_max = uchar(-1);
/** The maximum value for a ushort type. For a 2 byte ushort, this value is 65,535.*/
const ushort ushort_max = ushort(-1);
/** The maximum value for a uint type. For a 4 byte uint, this value is 4,294,967,295.*/
const uint uint_max = uint(-1);
/** The maximum value for a ulong type.*/
const ulong ulong_max = ulong(-1);
/** The maximum value for a ulonglong type.*/
const ulonglong ulonglong_max = ulonglong(-1);

#ifndef NULL
#define NULL 0
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif
#ifndef OneOverPi
#define OneOverPi (1.0/M_PI)
#endif

#define FLT_INF(type) std::numeric_limits<type>::infinity()
#define FLT_NAN(type) std::numeric_limits<type>::quiet_NaN()
#define MAX_VALUE(type) std::numeric_limits<type>::max()
#define MIN_VALUE(type) std::numeric_limits<type>::lowest()

/** \def FOURCC(c0, c1, c2, c3)
 * Converts the four passed characters into a single 32-bit integer. */
#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))

/** Retrieves the absolute value of the argument.
 * @return If the argument is less than E(0) (using operator<), then -argument (unary -), otherwise argument
 */

template<class E> inline E abs(E a){return ((a<E(0))?-a:a);}

/** Retrieves the minimum of two values.
 * Both values must be of the same type, and must have a less than operator defined.
 * If the two values are of different types that can convert to one common type, use min<common type>().
 */
template<class E> inline E min(E a, E b){return ((a<b)?a:b);}
/** Retrieves the maximum of two values.
 * Both values must be of the same type, and must have a greater than operator defined.
 * If the two values are of different types that can convert to one common type, use max<common type>().
 */
template<class E> inline E max(E a, E b){return ((a>b)?a:b);}
/** Retrievs the minimum of three values.
 * @see min
 */
template<class E> inline E min3(E a, E b, E c){return min(a,min(b,c));}
/** Retrievs the maximum of three values.
 * @see max
 */
template<class E> inline E max3(E a, E b, E c){return max(a,max(b,c));}
/** Retrievs the minimum of four values.
 * @see min
 */
template<class E> inline E min4(E a, E b, E c, E d){return min(a,min(b,min(c,d)));}
/** Retrievs the maximum of four values.
 * @see min
 */
template<class E> inline E max4(E a, E b, E c, E d){return max(a,max(b,max(c,d)));}

/** Clamps a value between a lower bound and an upper bound.
 * If the value is less than the lower bound, it is clamped to the lower bound.
 * If the value is greater than the upper bound, it is clamped to the upper bound.
 * All values must be of the same type, and must have a greater than and less than operator defined.
 * If the two values are of different types that can convert to one type, use clamp<common type>().
 * @param e The value to clamp.
 * @param l The lower bound.
 * @param h The upper bound.
 */
template<class E> inline E clamp(E e, E l, E h){return ((e > l)?((e > h)?h:e):l);}
/** Linearly interpolates bewteen two values.
 * If the factor is 0, this function returns the first value, if the factor is 1, this function returns the second value.
 * If the factor is between 0 and 1, this function linearly interpolates between the two.
 * If the factor is below 0 or above 1, the result goes beyond the two values along the line between the two values.
 * @param a The first value to interpolate from.
 * @param b The second value to interpolate to.
 * @param f The factor to interpolate by.
 * @return The interpolated value @f$ a*(1-f)+b*f @f$
 */
template<class E, class Scalar> inline E interp(E a, E b, Scalar f){return a*(Scalar(1)-f)+b*f;}
/** Smoothly interpolates between 0 and 1 between edge0 and edge1 respectively. Beyond edge0 is 0, and beyond edge1 is 1, where
 * beyond is the direction away from the opposite edge. */
template<class E> inline E smoothstep(E edge0, E edge1, E x){E f = clamp(E((x-edge0)/(edge1-edge0)), E(0), E(1)); return f*f*(E(3)-E(2)*f);}
/** Returns the square of a single value @f$ i*i @f$ .*/
template<class E> inline E square(E i){return i*i;}

/** Converts the value from degrees to radians. */
inline double toRadians(double d){return d*M_PI/180.0;}
/** Converts the value from radians to degrees. */
inline double toDegrees(double r){return r*180.0/M_PI;}

/** An enumeration to specify either the X axis, the Y axis or the Z axis.*/
enum AXIS {X_AXIS=0,Y_AXIS=1,Z_AXIS=2};

/** Returns the number of one bits in the 32 bit unsigned integer.*/
inline uint32_t numberOfOnes(uint32_t i){i = i - ((i >> 1) & 0x55555555); i = (i & 0x33333333) + ((i >> 2) & 0x33333333); return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;}
extern uint32_t MultiplyDeBruijnBitPosition[];
/** Returns the number of zero bits after the last one bit in a 32 bit unsigned integer.*/
inline uint32_t numberOfTrailingZeros(int32_t i){return MultiplyDeBruijnBitPosition[((uint32_t)((i & -i) * 0x077CB531U)) >> 27];}
/** Returns the number of zero bits before the first one bit in a 32 bit unsigned integer.*/
inline uint32_t numberOfLeadingZeros(uint32_t i){i |= (i >> 1); i |= (i >> 2); i |= (i >> 4); i |= (i >> 8); i |= (i >> 16); return 32-numberOfOnes(i);}

/** Increments a void pointer by a specified offset of bytes.
 * @param buffer The buffer to offset.
 * @param offset The number of bytes to offset.
 */
inline void* bufferOffset(void* buffer, ptr_diff_t offset){return ((char*)buffer)+offset;}
/** The const form of bufferOffset(void*,ptr_diff_t). @see bufferOffset(void*,ptr_diff_t) */
inline const void* bufferOffset(const void* buffer, ptr_diff_t offset){return ((const char*)buffer)+offset;}

/** Swaps the value of v1 and v2. This will call v1.operator=(v2) and v2.operator=(old value of v1) */
template<class E> inline void swap(E& v1, E& v2){E tmp = v1; v1 = v2; v2 = tmp;}

/** Returns @f$ 2^i @f$ optimized for when i and the result are unsigned integers.
 * This performs a bit shift, and an overflow occurs if i is greater than the number of bits in an unsigned integer.
*/
inline uint pow2(uint i){return 1u<<i;}
/** Returns @f$ base^{exp} @f$ optimized for when the base and exponent are positive integers.*/
uint powi(uint base, uint exp);
/** Returns @f$ \lfloor log_2(i) \rfloor @f$ optimized for when i and the result are 32 bit unsigned integers.*/
inline uint32_t log2floor(uint32_t i){i |= (i >> 1); i |= (i >> 2); i |= (i >> 4); i |= (i >> 8); i |= (i >> 16); return numberOfOnes(i)-1;}
/** Returns @f$ \lceil log_2(i) \rceil @f$ optimized for when i and the result are 32 bit unsigned integers.*/
inline uint32_t log2ceil(uint32_t i){int v = i&(i-1); v |= -v; v >>= 31; return log2floor(i)-v;}

#ifdef DEBUG
inline void d_assert_print(const char* test, const char* file, int line);
void d_assert_message(bool print_stack_trace, const char* message, ...);
/** \def d_assert(test,...)
 * If compiled in debug mode and the test fails, this will print the message and then a stack trace before exiting.
 * @param test The test to perform.
 * @param ... A printf formatted string followed by any printf args.
 */
#define d_assert(test, ...) if(test) (void)0; else {d_assert_print(#test, __FILE__, __LINE__);d_assert_message(true, __VA_ARGS__);}
/** \def d_assert_nst(test,...)
 * Same as d_assert, but will not print a stack trace if the test fails. @see d_assert */
#define d_assert_nst(test, ...) if(test) (void)0; else {d_assert_print(#test, __FILE__, __LINE__);d_assert_message(false, __VA_ARGS__);}
#else
#define d_assert(test, ...) ((void)0)
#define d_assert_nst(test, ...) ((void)0)
#endif

#ifdef _DOXYGEN_
/** Returns the average of two values. */
inline E average(E e1, E e2){}
#endif

inline bool average(bool b1, bool b2){return b1 | b2;}
inline char average(char c1, char c2){return (c1&c2)+((c1^c2)>>1);}
inline char average(wchar_t c1, wchar_t c2){return (c1&c2)+((c1^c2)>>1);}
inline uchar average(uchar c1, uchar c2){return (c1&c2)+((c1^c2)>>1);}
inline short average(short c1, short c2){return (c1&c2)+((c1^c2)>>1);}
inline ushort average(ushort c1, ushort c2){return (c1&c2)+((c1^c2)>>1);}
inline int average(int c1, int c2){return (c1&c2)+((c1^c2)>>1);}
inline uint average(uint c1, uint c2){return (c1&c2)+((c1^c2)>>1);}
inline long average(long c1, long c2){return (c1&c2)+((c1^c2)>>1);}
inline ulong average(ulong c1, ulong c2){return (c1&c2)+((c1^c2)>>1);}
inline longlong average(longlong c1, longlong c2){return (c1&c2)+((c1^c2)>>1);}
inline ulonglong average(ulonglong c1, ulonglong c2){return (c1&c2)+((c1^c2)>>1);}
inline float average(float c1, float c2){return (c1+c2)*0.5f;}
inline double average(double c1, double c2){return (c1+c2)*0.5;}

/** Returns the luminance of an RGB color.
 * The Red, Green and Blue channels are floating point numbers (0 to 1 instead of 0 to 255).
 * Values above 1 are allowed for HDR imaging.
 */
inline float luminance(float r, float g, float b){return r*0.299f+g*0.587f+b*0.114f;}

/** \def foreach(iterable, as)
 * Foreach iterates over all elements in an iterable object.
 * <b>Examples:</b>
 * @code
 * Iterable* iterable = ...;
 * foreach(*iterable, i){
 *     i->doSomething(); //NOTE: doSomething must be const if *iterable is const.
 * }
 * @endcode
 * @param iterable The iterable object. It must have an iterator() function defined.
 * Do not pass a pointer to an iterable object, pass the actual object via *iterable if it is a pointer to the iterable object.
 * @param as A name for each element of the array as it iterates that element. AS will have the type of
 * the iterator, which should act like a pointer to each element if the iterator follows the generic iterator.
 * @note This is a macro, not a real function.
 * @see iterator
 */
#define foreach(iterable, as) for(typeof((iterable).iterator()) as = (iterable).iterator(); as; as++)

/** Returns the fractional portion of a real number.
 * The result is always between 0 and 1, not including 1.
 * Example: @f$ frac(34.7158) = 0.7158 @f$ .
 */
template<class E> inline E frac(E f){return f-floor(f);}

#endif // CORE_COMMON_H_INCLUDED
