/** @file vec.h
 * Contains 2D, 3D and 4D vector classes (vec2, vec3 and vec4) with tools and utilities to manipulate them.
 * The vector classes can be used as vectors, points or even general purpose containers, however, the utilities
 * are designed for floating point components, and many of the utilities will not work correctly for integer or
 * object components.
 */

#ifndef CORE_VEC_H_INCLUDED
#define CORE_VEC_H_INCLUDED

#include "common.h"

/** A 2D vector class. Contains two components of type E, which can be accessed using x and y, s and t, r and g
 * or _ar[0] and _ar[1]. Contains many utilities for manipulating 2D vectors, and linear algebra functions on
 * 2D vectors. @note The linear algebra functions may not work on non-floating point vectors.
 */
template<class E> class vec2 {
public:
    /** The dimension of the vector. A constant value of 2. */
    static const int dim=2;
    /** The type of each component of the vector. */
    typedef E _type;
	union{
		struct{E x, y;};
		struct{E s, t;};
		struct{E r, g;};
		E _ar[dim];
	};
	/** Retrieves the i<sup>th</sup> component of this vector. No bounds checking is done, as this is synonymous to
	 * _ar[i]. A vec2 ALWAYS has a length of 2, so only indices 0 and 1 are valid. */
	inline E& operator[](int i) {return _ar[i];}
	/** The const form of operator[](int). @see operator[](int) */
	inline const E& operator[](int i) const {return _ar[i];}
	/** Adds a scalar value to all components of this vector. Calls operator+= on each component to add the scalar. */
	inline vec2<E>& operator+=(E v){x += v; y += v; return *this;}
	/** Subtracts a scalar value from all components of this vector. Calls operator-= on each component to subtract the scalar. */
	inline vec2<E>& operator-=(E v){x -= v; y -= v; return *this;}
	/** Multiplies a scalar value to all components of this vector. Calls operator*= on each component to multiply the scalar. */
	inline vec2<E>& operator*=(E v){x *= v; y *= v; return *this;}
	/** Divides a scalar value from all components of this vector. Calls operator/= on each component to divide the scalar. */
	inline vec2<E>& operator/=(E v){x /= v; y /= v; return *this;}
	/** The same as operator+=, except it returns a new vec2 instead of modifying this vec2. @see operator+=(E) */
	inline vec2<E> operator+(E v) const{return vec2<E>::make(x+v,y+v);}
	/** The same as operator-=, except it returns a new vec2 instead of modifying this vec2. @see operator-=(E) */
	inline vec2<E> operator-(E v) const{return vec2<E>::make(x-v,y-v);}
	/** The same as operator*=, except it returns a new vec2 instead of modifying this vec2. @see operator*=(E) */
	inline vec2<E> operator*(E v) const{return vec2<E>::make(x*v,y*v);}
	/** The same as operator/=, except it returns a new vec2 instead of modifying this vec2. @see operator/=(E) */
	inline vec2<E> operator/(E v) const{return vec2<E>::make(x/v,y/v);}
	/** The same as operator+, except the order of addition is reversed. @see operator+(E) const */
	inline friend vec2<E> operator+(E v, const vec2<E>& rhs){return vec2<E>::make(v+rhs.x,v+rhs.y);}
	/** The same as operator-, except the order of subtraction is reversed. @see operator-(E) const */
	inline friend vec2<E> operator-(E v, const vec2<E>& rhs){return vec2<E>::make(v-rhs.x,v-rhs.y);}
	/** The same as operator*, except the order of multiplication is reversed. @see operator*(E) const */
	inline friend vec2<E> operator*(E v, const vec2<E>& rhs){return vec2<E>::make(v*rhs.x,v*rhs.y);}
	/** The same as operator/, except the order of division is reversed. @see operator/(E) const */
	inline friend vec2<E> operator/(E v, const vec2<E>& rhs){return vec2<E>::make(v/rhs.x,v/rhs.y);}
	/** For every component of the vector, adds the corresponding component of the passed vector to it. */
	inline vec2<E>& operator+=(const vec2<E>& v){x += v.x; y += v.y; return *this;}
	/** For every component of the vector, subtracts the corresponding component of the passed vector from it. */
	inline vec2<E>& operator-=(const vec2<E>& v){x -= v.x; y -= v.y; return *this;}
	/** For every component of the vector, multiplies the corresponding component of the passed vector to it. */
	inline vec2<E>& operator*=(const vec2<E>& v){x *= v.x; y *= v.y; return *this;}
	/** For every component of the vector, divides the corresponding component of the passed vector from it. */
	inline vec2<E>& operator/=(const vec2<E>& v){x /= v.x; y /= v.y; return *this;}
	/** The same as operator+=, except it returns a new vec2 instead of modifying this vec2. @see operator+=(const vec2<E>&) */
	inline vec2<E> operator+(const vec2<E>& v) const{return vec2<E>::make(x+v.x,y+v.y);}
	/** The same as operator-=, except it returns a new vec2 instead of modifying this vec2. @see operator-=(const vec2<E>&) */
	inline vec2<E> operator-(const vec2<E>& v) const{return vec2<E>::make(x-v.x,y-v.y);}
	/** The same as operator*=, except it returns a new vec2 instead of modifying this vec2. @see operator*=(const vec2<E>&) */
	inline vec2<E> operator*(const vec2<E>& v) const{return vec2<E>::make(x*v.x,y*v.y);}
	/** The same as operator/=, except it returns a new vec2 instead of modifying this vec2. @see operator/=(const vec2<E>&) */
	inline vec2<E> operator/(const vec2<E>& v) const{return vec2<E>::make(x/v.x,y/v.y);}
	/** Negation operator, returns a new vector with all components of this vector negated. */
	inline vec2<E> operator-() const{return vec2<E>::make(-x,-y);}
	/** Returns true if all components of this vector are equal to the corresponding components of the passed vector.
	 * This uses the operator== of each component, and so if two vectors differ by only a small epsilon (due
     * to floating point errors or whatnot), they are still not equal. */
	inline bool operator==(const vec2<E> &v) const{return (x==v.x)&(y==v.y);}
	/** A synonym for !operator==(v). @see operator==(const vec2<E>&) const */
	inline bool operator!=(const vec2<E> &v) const{return (x!=v.x)|(y!=v.y);}
	/** Returns the dot product of this vector and another vector. The dot product is the sum
	 * of the product of the corresponding components in each vector. */
	inline friend E dot(const vec2<E> &v1, const vec2<E> &v2){return v1.x*v2.x+v1.y*v2.y;}
	/** Sets each component of this vector to the minimum of that component and the scalar e. @see min */
	inline friend vec2<E>& min(vec2<E> &v, E e){v.x = min(v.x, e); v.y = min(v.y, e); return v;}
	/** Sets each component of this vector to the maximum of that component and the scalar e. @see max */
	inline friend vec2<E>& max(vec2<E> &v, E e){v.x = max(v.x, e); v.y = max(v.y, e); return v;}
	/** Sets each component of this vector to the minimum of that component and the scalar e. @see min */
	inline friend vec2<E>& min(E e, vec2<E> &v){v.x = min(v.x, e); v.y = min(v.y, e); return v;}
	/** Sets each component of this vector to the maximum of that component and the scalar e. @see max */
	inline friend vec2<E>& max(E e, vec2<E> &v){v.x = max(v.x, e); v.y = max(v.y, e); return v;}
	/** Returns a new vector where each component is the minimum of the corresponding components in the passed vectors. @see min */
	inline friend vec2<E> min(const vec2<E> &v1, const vec2<E> &v2){return vec2<E>::make(min(v1.x, v2.x), min(v1.y, v2.y));}
	/** Returns a new vector where each component is the maximum of the corresponding components in the passed vectors. @see max */
	inline friend vec2<E> max(const vec2<E> &v1, const vec2<E> &v2){return vec2<E>::make(max(v1.x, v2.x), max(v1.y, v2.y));}
	/** Returns the minimum value of the passed vector. @see min */
	inline friend E min(const vec2<E> &v){return min(v.x, v.y);}
	/** Returns the maximum value of the passed vector. @see max */
	inline friend E max(const vec2<E> &v){return max(v.x, v.y);}
    /** Returns a new vector where each component is the absolute value of the corresponding component in the passed vector. */
	inline friend vec2<E> abs(const vec2<E> &v){return vec2<E>::make(E(fabs(v.x)), E(fabs(v.y)));}
	/** Returns a new vector where each component is the fractional value of the corresponding component in the passed vector. */
	inline friend vec2<E> frac(const vec2<E>& f){return vec2<E>::make(core::frac(f.x), core::frac(f.y));}
	/** Clamps each component in the vector between the scalars l and h. @see clamp */
	inline friend vec2<E>& clamp(vec2<E> &v, E l, E h){v.x = clamp(v.x, l, h); v.y = clamp(v.y, l, h); return v;}
	/** Creates a new vector where each component is the average of the corresponding components of the passed vectors. */
	inline friend vec2<E> average(const vec2<E> &v1, const vec2<E> &v2){return vec2<E>::make(average(v1.x, v2.x),average(v1.y, v2.y));}
	/** Returns a new vector with the base-e exponent of each component of the passed vector. */
	inline friend vec2<E> exp(const vec2<E> &v){return vec2<E>::make(core::exp(v.x), core::exp(v.y));}
	/** Returns a new vector with the base-e logarithm of each component of the passed vector. */
	inline friend vec2<E> log(const vec2<E> &v){return vec2<E>::make(core::log(v.x), core::log(v.y));}
	/** Returns a new vector where each component is the corresponding component of the first vector raised to the corresponding component of the second vector. */
	inline friend vec2<E> pow(const vec2<E> &v1, const vec2<E> &v2){return vec2<E>::make(core::pow(v1.x, v2.x), core::pow(v1.y, v2.y));}
    /** Converts the value from degrees to radians. */
    inline friend vec2<E> toRadians(const vec2<E> &d){return d*(M_PI/180.0);}
    /** Converts the value from radians to degrees. */
    inline friend vec2<E> toDegrees(const vec2<E> &r){return r*(180.0/M_PI);}
	/** Creates a vec2 with all the same components. */
	inline static vec2<E> make(E _v=E()){vec2<E> ret = {{{_v,_v}}}; return ret;}
	/** Creates a vec2 from the corresponding components. */
	inline static vec2<E> make(E _x, E _y){vec2<E> ret = {{{_x,_y}}}; return ret;}
	/** Creates a vec2 from the first two elements of a c array, performing conversions from the type of the elements of the c array to the type of the new vec2's components if necessary. */
	template<class T> inline static vec2<E> make(const T* v){vec2<E> ret = {{{E(v[0]),E(v[1])}}}; return ret;}
	/** Creates a vec2 from another vec2, performing conversions from the type of the passed vec2's components to the type of the new vec2's components if necessary. */
	template<class T> inline static vec2<E> make(const vec2<T> &v){vec2<E> ret = {{{E(v.x),E(v.y)}}}; return ret;}
};

/** A 3D vector class. Contains three components of type E, which can be accessed using x, y, z; s, t, p; r, g, b
 * or _ar[0], _ar[1], _ar[2]. Additionally, a vec2 of any two contiguous variables can be accessed using two
 * characters from above. Examples of this are xy to retrieve the first two components, and yz to retrieve the last two. You cannot
 * access xz, as these values are not contiguous (x is not right before y). However, some graphic cards support efficient
 * swizzle operations that do just that.<br><br>
 * Contains many utilities for manipulating 3D vectors, and linear algebra functions on
 * 3D vectors. @note The linear algebra functions may not work on non-floating point vectors.
 */
template<class E> class vec3 {
public:
    /** The dimension of the vector. A constant value of 3. */
    static const int dim=3;
    /** The type of each component of the vector. */
    typedef E _type;
#define vec3access(x,y,z,n) struct{E x, y, z;};\
struct{vec2<E> x##y; E pad1##n;}; \
struct{E pad2##n; vec2<E> y##z;};
	union{
	    vec3access(x,y,z,0);
	    vec3access(s,t,p,1);
	    vec3access(r,g,b,2);
		E _ar[dim];
	};
#undef vec3access
    /** Retrieves the i<sup>th</sup> component of this vector. No bounds checking is done, as this is synonymous to
	 * _ar[i]. A vec3 ALWAYS has a length of 3, so only indices 0 to 2 are valid. */
	inline E& operator[](int i) {return _ar[i];}
	/** The const form of operator[](int). @see operator[](int) */
	inline const E& operator[](int i) const {return _ar[i];}
	/** Adds a scalar value to all components of this vector. Calls operator+= on each component to add the scalar. */
	inline vec3<E>& operator+=(E v){x += v; y += v; z += v; return *this;}
	/** Subtracts a scalar value from all components of this vector. Calls operator-= on each component to subtract the scalar. */
	inline vec3<E>& operator-=(E v){x -= v; y -= v; z -= v; return *this;}
	/** Multiplies a scalar value to all components of this vector. Calls operator*= on each component to multiply the scalar. */
	inline vec3<E>& operator*=(E v){x *= v; y *= v; z *= v; return *this;}
	/** Divides a scalar value from all components of this vector. Calls operator/= on each component to divide the scalar. */
	inline vec3<E>& operator/=(E v){x /= v; y /= v; z /= v; return *this;}
	/** The same as operator+=, except it returns a new vec3 instead of modifying this vec3. @see operator+=(E) */
	inline vec3<E> operator+(E v) const{return vec3<E>::make(x+v,y+v,z+v);}
	/** The same as operator-=, except it returns a new vec3 instead of modifying this vec3. @see operator-=(E) */
	inline vec3<E> operator-(E v) const{return vec3<E>::make(x-v,y-v,z-v);}
	/** The same as operator*=, except it returns a new vec3 instead of modifying this vec3. @see operator*=(E) */
	inline vec3<E> operator*(E v) const{return vec3<E>::make(x*v,y*v,z*v);}
	/** The same as operator/=, except it returns a new vec3 instead of modifying this vec3. @see operator/=(E) */
	inline vec3<E> operator/(E v) const{return vec3<E>::make(x/v,y/v,z/v);}
	/** The same as operator+, except the order of addition is reversed. @see operator+(E) const */
	inline friend vec3<E> operator+(E v, const vec3<E>& rhs){return vec3<E>::make(v+rhs.x,v+rhs.y,v+rhs.z);}
	/** The same as operator-, except the order of subtraction is reversed. @see operator-(E) const */
	inline friend vec3<E> operator-(E v, const vec3<E>& rhs){return vec3<E>::make(v-rhs.x,v-rhs.y,v-rhs.z);}
	/** The same as operator*, except the order of multiplication is reversed. @see operator*(E) const */
	inline friend vec3<E> operator*(E v, const vec3<E>& rhs){return vec3<E>::make(v*rhs.x,v*rhs.y,v*rhs.z);}
	/** The same as operator/, except the order of division is reversed. @see operator/(E) const */
	inline friend vec3<E> operator/(E v, const vec3<E>& rhs){return vec3<E>::make(v/rhs.x,v/rhs.y,v/rhs.z);}
	/** For every component of the vector, adds the corresponding component of the passed vector to it. */
	inline vec3<E>& operator+=(const vec3<E>& v){x += v.x; y += v.y; z += v.z; return *this;}
	/** For every component of the vector, subtracts the corresponding component of the passed vector from it. */
	inline vec3<E>& operator-=(const vec3<E>& v){x -= v.x; y -= v.y; z -= v.z; return *this;}
	/** For every component of the vector, multiplies the corresponding component of the passed vector to it. */
	inline vec3<E>& operator*=(const vec3<E>& v){x *= v.x; y *= v.y; z *= v.z; return *this;}
	/** For every component of the vector, divides the corresponding component of the passed vector from it. */
	inline vec3<E>& operator/=(const vec3<E>& v){x /= v.x; y /= v.y; z /= v.z; return *this;}
	/** The same as operator+=, except it returns a new vec3 instead of modifying this vec3. @see operator+=(const vec3<E>&) */
	inline vec3<E> operator+(const vec3<E>& v) const{return vec3<E>::make(x+v.x,y+v.y,z+v.z);}
	/** The same as operator-=, except it returns a new vec3 instead of modifying this vec3. @see operator-=(const vec3<E>&) */
	inline vec3<E> operator-(const vec3<E>& v) const{return vec3<E>::make(x-v.x,y-v.y,z-v.z);}
	/** The same as operator*=, except it returns a new vec3 instead of modifying this vec3. @see operator*=(const vec3<E>&) */
	inline vec3<E> operator*(const vec3<E>& v) const{return vec3<E>::make(x*v.x,y*v.y,z*v.z);}
	/** The same as operator/=, except it returns a new vec3 instead of modifying this vec3. @see operator/=(const vec3<E>&) */
	inline vec3<E> operator/(const vec3<E>& v) const{return vec3<E>::make(x/v.x,y/v.y,z/v.z);}
	/** Negation operator, returns a new vector with all components of this vector negated. */
	inline vec3<E> operator-() const{return vec3<E>::make(-x,-y,-z);}
	/** Returns true if all components of this vector are equal to the corresponding components of the passed vector.
	 * This uses the operator== of each component, and so if two vectors differ by only a small epsilon (due
     * to floating point errors or whatnot), they are still not equal. */
	inline bool operator==(const vec3<E> &v) const{return (x==v.x)&(y==v.y)&(z==v.z);}
	/** A synonym for !operator==(v). @see operator==(const vec3<E>&) const */
	inline bool operator!=(const vec3<E> &v) const{return (x!=v.x)|(y!=v.y)|(z!=v.z);}
	/** Returns the dot product of this vector and another vector. The dot product is the sum
	 * of the product of the corresponding components in each vector. */
	inline friend E dot(const vec3<E> &v1, const vec3<E> &v2){return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;}
	/** Returns a vec3 that is the cross product of v1 and v2. */
	inline friend vec3<E> cross(const vec3<E> &v1, const vec3<E> &v2){return vec3<E>::make(v1.y*v2.z-v2.y*v1.z, v1.z*v2.x-v2.z*v1.x, v1.x*v2.y-v2.x*v1.y);}
	/** Sets each component of this vector to the minimum of that component and the scalar e. @see min */
	inline friend vec3<E>& min(vec3<E> &v, E e){v.x = min(v.x, e); v.y = min(v.y, e); v.z = min(v.z, e); return v;}
	/** Sets each component of this vector to the maximum of that component and the scalar e. @see max */
	inline friend vec3<E>& max(vec3<E> &v, E e){v.x = max(v.x, e); v.y = max(v.y, e); v.z = max(v.z, e); return v;}
	/** Sets each component of this vector to the minimum of that component and the scalar e. @see min */
	inline friend vec3<E>& min(E e, vec3<E> &v){v.x = min(v.x, e); v.y = min(v.y, e); v.z = min(v.z, e); return v;}
	/** Sets each component of this vector to the maximum of that component and the scalar e. @see max */
	inline friend vec3<E>& max(E e, vec3<E> &v){v.x = max(v.x, e); v.y = max(v.y, e); v.z = max(v.z, e); return v;}
	/** Returns a new vector where each component is the minimum of the corresponding components in the passed vectors. @see min */
	inline friend vec3<E> min(const vec3<E> &v1, const vec3<E> &v2){return vec3<E>::make(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));}
	/** Returns a new vector where each component is the maximum of the corresponding components in the passed vectors. @see max */
	inline friend vec3<E> max(const vec3<E> &v1, const vec3<E> &v2){return vec3<E>::make(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));}
	/** Returns the minimum value of the passed vector. @see min */
	inline friend E min(const vec3<E> &v){return min3(v.x, v.y, v.z);}
	/** Returns the maximum value of the passed vector. @see max */
	inline friend E max(const vec3<E> &v){return max3(v.x, v.y, v.z);}
	/** Clamps each component in the vector between the scalars l and h. @see clamp */
	inline friend vec3<E>& clamp(vec3<E> &v, E l, E h){v.x = clamp(v.x, l, h); v.y = clamp(v.y, l, h); v.z = clamp(v.z, l, h); return v;}
	/** Creates a new vector where each component is the average of the corresponding components of the passed vectors. */
	inline friend vec3<E> average(const vec3<E> &v1, const vec3<E> &v2){return vec3<E>::make(average(v1.x, v2.x),average(v1.y, v2.y),average(v1.z, v2.z));}
	/** Returns a new vector with the base-e exponent of each component of the passed vector. */
	inline friend vec3<E> exp(const vec3<E> &v){return vec3<E>::make(core::exp(v.x), core::exp(v.y), core::exp(v.z));}
	/** Returns a new vector with the base-e logarithm of each component of the passed vector. */
	inline friend vec3<E> log(const vec3<E> &v){return vec3<E>::make(core::log(v.x), core::log(v.y), core::log(v.z));}
	/** Returns a new vector where each component is the corresponding component of the first vector raised to the corresponding component of the second vector. */
	inline friend vec3<E> pow(const vec3<E> &v1, const vec3<E> &v2){return vec3<E>::make(core::pow(v1.x, v2.x), core::pow(v1.y, v2.y), core::pow(v1.z, v2.z));}
    /** Converts the value from degrees to radians. */
    inline friend vec3<E> toRadians(const vec3<E> &d){return d*(M_PI/180.0);}
    /** Converts the value from radians to degrees. */
    inline friend vec3<E> toDegrees(const vec3<E> &r){return r*(180.0/M_PI);}
	/** Creates a vec3 with all the same components. */
	inline static vec3<E> make(E _v=E()){vec3<E> ret = {{{_v,_v,_v}}}; return ret;}
	/** Creates a vec3 from the corresponding components. */
	inline static vec3<E> make(E _x, E _y, E _z){vec3<E> ret = {{{_x,_y,_z}}}; return ret;}
	/** Creates a vec3 using the two components of _xy as the first two components and _z as the last component. */
	inline static vec3<E> make(const vec2<E> &_xy, E _z=E()){vec3<E> ret = {{{_xy[0],_xy[1],_z}}}; return ret;}
	/** Creates a vec3 using _x as the first component and the two components of _yz as the last two components. */
	inline static vec3<E> make(E _x, const vec2<E> &_yz){vec3<E> ret = {{{_x,_yz[0],_yz[1]}}}; return ret;}
	/** Returns a new vector where each component is the absolute value of the corresponding component in the passed vector. */
	inline friend vec3<E> abs(const vec3<E> &v){return vec3<E>::make(E(fabs(v.x)), E(fabs(v.y)), E(fabs(v.z)));}
	/** Returns a new vector where each component is the fractional value of the corresponding component in the passed vector. */
	inline friend vec3<E> frac(const vec3<E>& f){return vec3<E>::make(core::frac(f.x), core::frac(f.y), core::frac(f.z));}
	/** Creates a vec3 from the first three elements of a c array, performing conversions from the type of the elements of the c array to the type of the new vec3's components if necessary. */
	template<class T> inline static vec3<E> make(const T* v){vec3<E> ret = {{{E(v[0]),E(v[1]),E(v[2])}}}; return ret;}
	/** Creates a vec3 from another vec3, performing conversions from the type of the passed vec3's components to the type of the new vec3's components if necessary. */
	template<class T> inline static vec3<E> make(const vec3<T> &v){vec3<E> ret = {{{E(v.x),E(v.y),E(v.z)}}}; return ret;}

    /** Computes the tangent and binormal from the normal vector.
     * @note There are infinite tangents and binormals possible from a single normal by rotating about that normal.
     * This function chooses one of those orientations that is consistent and coherent. This means that one normal
     * will always return the same tangent and binormal, and two normals that are close will return two tangents and
     * binormals that are close.
     * @param normal [in] The normal vector. It must be normalized.
     * @param tangent [out] Stores the computed tangent in this reference.
     * @param binormal [out] Stores the computed binormal in this reference.*/
    inline friend void computeTangentFromNormal(const vec3<E>& normal, vec3<E>& tangent, vec3<E>& binormal){
        tangent = (fabs(normal.z) > 0.999)?vec3<E>::make(0,1,0):normalize(vec3<E>::make(normal.y,-normal.x,0));
        if(tangent.y < 0) tangent = -tangent;
        binormal = cross(normal, tangent);
    }
};

/** A 4D vector class. Contains four components of type E, which can be accessed using x, y, z, w; s, t, p, q; r, g, b, a
 * or _ar[0], _ar[1], _ar[2], _ar[3]. Additionally, a vec2 of any two contiguous variables can be accessed using two
 * characters from above, and a vec3 of any three contiguous variables can be accessed using three characters from
 * above. Examples of this are xy to retrieve the first two components, yz to retrieve the middle two and yzw to return
 * the last three. You can even retrieve the first two components of the vec3 using yzw.xy - however, keep in mind
 * that this is yz of the vec4 vector (as it is the first two components of yzw). You cannot
 * access xz, as these values are not contiguous (x is not right before y). However, some graphic cards support efficient
 * swizzle operations that do just that.<br><br>
 * Contains many utilities for manipulating 4D vectors, and linear algebra functions on
 * 4D vectors. @note The linear algebra functions may not work on non-floating point vectors.
 */
template<class E> class vec4 {
public:
    /** The dimension of the vector. A constant value of 4. */
    static const int dim=4;
    /** The type of each component of the vector. */
    typedef E _type;
#define vec4access(x,y,z,w,n) struct{E x, y, z, w;};\
		struct{vec2<E> x##y; E pad1##n, pad2##n;};\
		struct{E pad3##n; vec2<E> y##z; E pad4##n;};\
		struct{E pad5##n, pad6##n; vec2<E> z##w;};\
		struct{vec3<E> x##y##z; E pad7##n;};\
		struct{E pad8##n; vec3<E> y##z##w;};
	union{
	    vec4access(x,y,z,w,0);
	    vec4access(s,t,p,q,1);
	    vec4access(r,g,b,a,2);
		E _ar[dim];
	};
#undef vec4access
    /** Retrieves the i<sup>th</sup> component of this vector. No bounds checking is done, as this is synonymous to
	 * _ar[i]. A vec4 ALWAYS has a length of 4, so only indices 0 to 3 are valid. */
	inline E& operator[](int i) {return _ar[i];}
    /** The const form of operator[](int). @see operator[](int) */
	inline const E& operator[](int i) const {return _ar[i];}
	/** Adds a scalar value to all components of this vector. Calls operator+= on each component to add the scalar. */
	inline vec4<E>& operator+=(E v){x += v; y += v; z += v; w += v; return *this;}
	/** Subtracts a scalar value from all components of this vector. Calls operator-= on each component to subtract the scalar. */
	inline vec4<E>& operator-=(E v){x -= v; y -= v; z -= v; w -= v; return *this;}
	/** Multiplies a scalar value to all components of this vector. Calls operator*= on each component to multiply the scalar. */
	inline vec4<E>& operator*=(E v){x *= v; y *= v; z *= v; w *= v; return *this;}
	/** Divides a scalar value from all components of this vector. Calls operator/= on each component to divide the scalar. */
	inline vec4<E>& operator/=(E v){x /= v; y /= v; z /= v; w /= v; return *this;}
	/** The same as operator+=, except it returns a new vec4 instead of modifying this vec4. @see operator+=(E) */
	inline vec4<E> operator+(E v) const{return vec4<E>::make(x+v,y+v,z+v,w+v);}
	/** The same as operator-=, except it returns a new vec4 instead of modifying this vec4. @see operator-=(E) */
	inline vec4<E> operator-(E v) const{return vec4<E>::make(x-v,y-v,z-v,w-v);}
	/** The same as operator*=, except it returns a new vec4 instead of modifying this vec4. @see operator*=(E) */
	inline vec4<E> operator*(E v) const{return vec4<E>::make(x*v,y*v,z*v,w*v);}
	/** The same as operator/=, except it returns a new vec4 instead of modifying this vec4. @see operator/=(E) */
	inline vec4<E> operator/(E v) const{return vec4<E>::make(x/v,y/v,z/v,w/v);}
	/** The same as operator+, except the order of addition is reversed. @see operator+(E) const */
	inline friend vec4<E> operator+(E v, const vec4<E>& rhs){return vec4<E>::make(v+rhs.x,v+rhs.y,v+rhs.z,v+rhs.w);}
	/** The same as operator-, except the order of subtraction is reversed. @see operator-(E) const */
	inline friend vec4<E> operator-(E v, const vec4<E>& rhs){return vec4<E>::make(v-rhs.x,v-rhs.y,v-rhs.z,v-rhs.w);}
	/** The same as operator*, except the order of multiplication is reversed. @see operator*(E) const */
	inline friend vec4<E> operator*(E v, const vec4<E>& rhs){return vec4<E>::make(v*rhs.x,v*rhs.y,v*rhs.z,v*rhs.w);}
	/** The same as operator/, except the order of division is reversed. @see operator/(E) const */
	inline friend vec4<E> operator/(E v, const vec4<E>& rhs){return vec4<E>::make(v/rhs.x,v/rhs.y,v/rhs.z,v/rhs.w);}
	/** For every component of the vector, adds the corresponding component of the passed vector to it. */
	inline vec4<E>& operator+=(const vec4<E>& v){x += v.x; y += v.y; z += v.z; w += v.w; return *this;}
	/** For every component of the vector, subtracts the corresponding component of the passed vector from it. */
	inline vec4<E>& operator-=(const vec4<E>& v){x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;}
	/** For every component of the vector, multiplies the corresponding component of the passed vector to it. */
	inline vec4<E>& operator*=(const vec4<E>& v){x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this;}
	/** For every component of the vector, divides the corresponding component of the passed vector from it. */
	inline vec4<E>& operator/=(const vec4<E>& v){x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this;}
	/** The same as operator+=, except it returns a new vec4 instead of modifying this vec4. @see operator+=(const vec4<E>&) */
	inline vec4<E> operator+(const vec4<E>& v) const{return vec4<E>::make(x+v.x,y+v.y,z+v.z,w+v.w);}
	/** The same as operator-=, except it returns a new vec4 instead of modifying this vec4. @see operator-=(const vec4<E>&) */
	inline vec4<E> operator-(const vec4<E>& v) const{return vec4<E>::make(x-v.x,y-v.y,z-v.z,w-v.w);}
	/** The same as operator*=, except it returns a new vec4 instead of modifying this vec4. @see operator*=(const vec4<E>&) */
	inline vec4<E> operator*(const vec4<E>& v) const{return vec4<E>::make(x*v.x,y*v.y,z*v.z,w*v.w);}
	/** The same as operator/=, except it returns a new vec4 instead of modifying this vec4. @see operator/=(const vec4<E>&) */
	inline vec4<E> operator/(const vec4<E>& v) const{return vec4<E>::make(x/v.x,y/v.y,z/v.z,w/v.w);}
	/** Negation operator, returns a new vector with all components of this vector negated. */
	inline vec4<E> operator-() const{return vec4<E>::make(-x,-y,-z,-w);}
	/** Returns true if all components of this vector are equal to the corresponding components of the passed vector.
	 * This uses the operator== of each component, and so if two vectors differ by only a small epsilon (due
     * to floating point errors or whatnot), they are still not equal. */
	inline bool operator==(const vec4<E> &v) const{return (x==v.x)&(y==v.y)&(z==v.z)&(w==v.w);}
	/** A synonym for !operator==(v). @see operator==(const vec4<E>&) const */
	inline bool operator!=(const vec4<E> &v) const{return (x!=v.x)|(y!=v.y)|(z!=v.z)|(w!=v.w);}
	/** Returns the dot product of this vector and another vector. The dot product is the sum
	 * of the product of the corresponding components in each vector. */
	inline friend E dot(const vec4<E> &v1, const vec4<E> &v2){return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z+v1.w*v2.w;}
	/** Treats this vec4 as a homogeneous position vector, and returns the vec3 position of this vector.
	 * @return The first three components of this vector divided by the fourth (homogeneous divide). */
	inline friend vec3<E> toPosition(const vec4<E> &v){return v.xyz/v.w;}
	/** Sets each component of this vector to the minimum of that component and the scalar e. @see min */
	inline friend vec4<E>& min(vec4<E> &v, E e){v.x = min(v.x, e); v.y = min(v.y, e); v.z = min(v.z, e); v.w = min(v.w, e); return v;}
	/** Sets each component of this vector to the maximum of that component and the scalar e. @see max */
	inline friend vec4<E>& max(vec4<E> &v, E e){v.x = max(v.x, e); v.y = max(v.y, e); v.z = max(v.z, e); v.w = max(v.w, e); return v;}
	/** Sets each component of this vector to the minimum of that component and the scalar e. @see min */
	inline friend vec4<E>& min(E e, vec4<E> &v){v.x = min(v.x, e); v.y = min(v.y, e); v.z = min(v.z, e); v.w = min(v.w, e); return v;}
	/** Sets each component of this vector to the maximum of that component and the scalar e. @see max */
	inline friend vec4<E>& max(E e, vec4<E> &v){v.x = max(v.x, e); v.y = max(v.y, e); v.z = max(v.z, e); v.w = max(v.w, e); return v;}
	/** Returns a new vector where each component is the minimum of the corresponding components in the passed vectors. @see min */
	inline friend vec4<E> min(const vec4<E> &v1, const vec4<E> &v2){return vec4<E>::make(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z), min(v1.w, v2.w));}
	/** Returns a new vector where each component is the maximum of the corresponding components in the passed vectors. @see max */
	inline friend vec4<E> max(const vec4<E> &v1, const vec4<E> &v2){return vec4<E>::make(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z), max(v1.w, v2.w));}
	/** Returns the minimum value of the passed vector. @see min */
	inline friend E min(const vec4<E> &v){return min4(v.x, v.y, v.z, v.w);}
	/** Returns the maximum value of the passed vector. @see max */
	inline friend E max(const vec4<E> &v){return max4(v.x, v.y, v.z, v.w);}
	/** Returns a new vector where each component is the absolute value of the corresponding component in the passed vector. */
	inline friend vec4<E> abs(const vec4<E> &v){return vec4<E>::make(E(fabs(v.x)), E(fabs(v.y)), E(fabs(v.z)), E(fabs(v.w)));}
	/** Returns a new vector where each component is the fractional value of the corresponding component in the passed vector. */
	inline friend vec4<E> frac(const vec4<E>& f){return vec4<E>::make(core::frac(f.x), core::frac(f.y), core::frac(f.z), core::frac(f.w));}
	/** Clamps each component in the vector between the scalars l and h. @see clamp */
	inline friend vec4<E>& clamp(vec4<E> &v, E l, E h){v.x = clamp(v.x, l, h); v.y = clamp(v.y, l, h); v.z = clamp(v.z, l, h); v.w = clamp(v.w, l, h); return v;}
	/** Creates a new vector where each component is the average of the corresponding components of the passed vectors. */
	inline friend vec4<E> average(const vec4<E> &v1, const vec4<E> &v2){return vec4<E>::make(average(v1.x, v2.x),average(v1.y, v2.y),average(v1.z, v2.z),average(v1.w, v2.w));}
	/** Returns a new vector with the base-e exponent of each component of the passed vector. */
	inline friend vec4<E> exp(const vec4<E> &v){return vec4<E>::make(core::exp(v.x), core::exp(v.y), core::exp(v.z), core::exp(v.w));}
	/** Returns a new vector with the base-e logarithm of each component of the passed vector. */
	inline friend vec4<E> log(const vec4<E> &v){return vec4<E>::make(core::log(v.x), core::log(v.y), core::log(v.z), core::log(v.w));}
	/** Returns a new vector where each component is the corresponding component of the first vector raised to the corresponding component of the second vector. */
	inline friend vec4<E> pow(const vec4<E> &v1, const vec4<E> &v2){return vec4<E>::make(core::pow(v1.x, v2.x), core::pow(v1.y, v2.y), core::pow(v1.z, v2.z), core::pow(v1.w, v2.w));}
    /** Converts the value from degrees to radians. */
    inline friend vec4<E> toRadians(const vec4<E> &d){return d*(M_PI/180.0);}
    /** Converts the value from radians to degrees. */
    inline friend vec4<E> toDegrees(const vec4<E> &r){return r*(180.0/M_PI);}
	/** Creates a vec4 with all the same components, except w which is set to 1. */
	inline static vec4<E> make(E _v=E()){vec4<E> ret = {{{_v,_v,_v,E(1)}}}; return ret;}
	/** Creates a vec4 from the corresponding components. */
	inline static vec4<E> make(E _x, E _y, E _z, E _w=E(1)){vec4<E> ret = {{{_x,_y,_z,_w}}}; return ret;}
	/** Creates a vec4 using the two components of _xy as the first two components, _z as the third component and _w as the last component. */
	inline static vec4<E> make(const vec2<E> &_xy, E _z, E _w=E(1)){vec4<E> ret = {{{_xy[0],_xy[1],_z,_w}}}; return ret;}
	/** Creates a vec4 using _x as the first component, the two components of _yz as the middle two components and _w as the last component. */
	inline static vec4<E> make(E _x, const vec2<E> &_yz, E _w=E(1)){vec4<E> ret = {{{_x,_yz[0],_yz[1],_w}}}; return ret;}
	/** Creates a vec4 using _x as the first component, _y as the second component and the two components of _zw as the last two components. */
	inline static vec4<E> make(E _x, E _y, const vec2<E> &_zw){vec4<E> ret = {{{_x,_y,_zw[0],_zw[1]}}}; return ret;}
	/** Creates a vec4 using the two components of _xy as the first two components and the two components of _zw as the last two components. */
	inline static vec4<E> make(const vec2<E> &_xy, const vec2<E> &_zw){vec4<E> ret = {{{_xy[0],_xy[1],_zw[0],_zw[1]}}}; return ret;}
	/** Creates a vec4 using the three components of _xyw as the first three components and _w as the last component. */
	inline static vec4<E> make(const vec3<E> &_xyz, E _w=E(1)){vec4<E> ret = {{{_xyz[0],_xyz[1],_xyz[2],_w}}}; return ret;}
	/** Creates a vec4 using _x as the first component and the three components of _yzw as the last three components. */
	inline static vec4<E> make(E _x, const vec3<E> &_yzw){vec4<E> ret = {{{_x,_yzw[0],_yzw[1],_yzw[2]}}}; return ret;}
	/** Creates a vec4 from the first four elements of a c array, performing conversions from the type of the elements of the c array to the type of the new vec4's components if necessary. */
	template<class T> inline static vec4<E> make(const T* v){vec4<E> ret = {{{E(v[0]),E(v[1]),E(v[2]),E(v[3])}}}; return ret;}
	/** Creates a vec4 from another vec4, performing conversions from the type of the passed vec4's components to the type of the new vec4's components if necessary. */
	template<class T> inline static vec4<E> make(const vec4<T> &v){vec4<E> ret = {{{E(v.x),E(v.y),E(v.z),E(v.w)}}}; return ret;}
};

/** Returns the length of a vector. A synonym for sqrt(dot(v,v)). @see vec2::dot(const vec2<E>&,const vec2<E>&) */
template<class E> inline double length(const E &v){return sqrt((double)dot(v, v));}
/** Returns a new vector that is the passed vector normalized. A synonym for v/length(v). @see length @see vec2::operator/(E) const */
template<class E> inline E normalize(const E &v){return v/length(v);}
/** Normalizes a vector. A synonym for v /= length(v). @see length @see vec2::operator/=(E) */
template<class E> inline E& normalize_m(E &v){return v /= length(v);}
/** Returns the squared distance between two vectors treated as points. A synonym for dot(v1-v2,v1-v2). @see vec2::dot(const vec2<E>&,const vec2<E>&) */
template<class E> inline double distanceSq(const E &v1, const E &v2){E v = v1-v2; return dot(v,v);}
/** Returns the distance between two vectors treated as points. A synonym for length(v1-v2). @see length */
template<class E> inline double distance(const E &v1, const E &v2){return length(v1-v2);}

/** A vector of 2 chars. */ typedef vec2<char> char2; /** A vector of 3 chars. */  typedef vec3<char> char3; /** A vector of 4 chars. */ typedef vec4<char> char4;
/** A vector of 2 uchars. */ typedef vec2<uchar> uchar2; /** A vector of 3 uchars. */  typedef vec3<uchar> uchar3; /** A vector of 4 uchars. */ typedef vec4<uchar> uchar4;
/** A vector of 2 shorts. */ typedef vec2<short> short2; /** A vector of 3 shorts. */  typedef vec3<short> short3; /** A vector of 4 shorts. */ typedef vec4<short> short4;
/** A vector of 2 ushorts. */ typedef vec2<ushort> ushort2; /** A vector of 3 ushorts. */  typedef vec3<ushort> ushort3; /** A vector of 4 ushorts. */ typedef vec4<ushort> ushort4;
/** A vector of 2 ints. */ typedef vec2<int> int2; /** A vector of 3 ints. */  typedef vec3<int> int3; /** A vector of 4 ints. */ typedef vec4<int> int4;
/** A vector of 2 uints. */ typedef vec2<uint> uint2; /** A vector of 3 uints. */  typedef vec3<uint> uint3; /** A vector of 4 uints. */ typedef vec4<uint> uint4;
/** A vector of 2 longs. */ typedef vec2<long> long2; /** A vector of 3 longs. */  typedef vec3<long> long3; /** A vector of 4 longs. */ typedef vec4<long> long4;
/** A vector of 2 ulongs. */ typedef vec2<ulong> ulong2; /** A vector of 3 ulongs. */  typedef vec3<ulong> ulong3; /** A vector of 4 ulongs. */ typedef vec4<ulong> ulong4;
/** A vector of 2 longlongs. */ typedef vec2<longlong> longlong2; /** A vector of 3 longlongs. */  typedef vec3<longlong> longlong3; /** A vector of 4 longlongs. */ typedef vec4<longlong> longlong4;
/** A vector of 2 ulonglongs. */ typedef vec2<ulonglong> ulonglong2; /** A vector of 3 ulonglongs. */  typedef vec3<ulonglong> ulonglong3; /** A vector of 4 ulonglongs. */ typedef vec4<ulonglong> ulonglong4;
/** A vector of 2 floats. */ typedef vec2<float> float2; /** A vector of 3 floats. */  typedef vec3<float> float3; /** A vector of 4 floats. */ typedef vec4<float> float4;
/** A vector of 2 doubles. */ typedef vec2<double> double2; /** A vector of 3 doubles. */  typedef vec3<double> double3; /** A vector of 4 doubles. */ typedef vec4<double> double4;

/** Returns the luminance of a vector of floats treated as a color. A synonym for luminance(c.r, c.g, c.b). @see luminance(float,float,float) */
inline float luminance(const float3& c){return luminance(c.r, c.g, c.b);}
/** Returns the luminance of a vector of floats treated as a color. A synonym for luminance(c.r, c.g, c.b). @see luminance(float,float,float) */
inline float luminance(const float4& c){return luminance(c.r, c.g, c.b);}

#endif // CORE_VEC_H_INCLUDED
