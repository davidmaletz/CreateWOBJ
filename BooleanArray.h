/** @file BooleanArray.h
 * Contains 1D, 2D and 3D arrays of boolean values.
 */

#ifndef CORE_BOOLEANARRAY_H_INCLUDED
#define CORE_BOOLEANARRAY_H_INCLUDED

/** A reference to a single bit of a uint value. Used as the return value for operator() of boolean arrays.
 * @see BooleanArray.operator[](ptr_size_t) @see BooleanArray2D.operator[](ptr_size_t,ptr_size_t) @see BooleanArray3D.operator[](ptr_size_t,ptr_size_t,ptr_size_t)
 */
class BitRef {
    private:
    uint& elem; ptr_size_t offset;
    public:
    /** Creates a bit reference.
     * @param e The uint value that contains the bit.
     * @param o The index of the bit in the uint. This must be 0 to the number of bits in the uint value.
     */
    inline BitRef(uint& e, ptr_size_t o) : elem(e), offset(pow2(o)) {}
    /** Assignment operator, sets the bit to the value of the boolean (true or false). */
    inline BitRef& operator=(bool b){
        if(b) elem |= offset;
        else elem &= ~offset;
        return *this;
    }
    /** Converts the bit reference to a boolean. Returns the value of the bit (true or false). */
    inline operator bool() const {return (elem & offset) != 0;}
};

/** An Array of boolean values. Internally stored as an Array of uint values, this class transparently
 * indexes individual bits from the uint values using the BitRef class. This class can pack 8 boolean values
 * per byte of storage (assuming 8-bit bytes), so is preferable over Array<bool> for dynamic storage of booleans.
 * @see Array
 */
class BooleanArray {
    public:
    enum{
        /** The number of bits per uint value. */
        nBits = sizeof(uint)*8
    };
    /** Creates a BooleanArray with an initial max size of at least ct booleans. The actual max size is
     * rounded up to the next multiple to nBits.
     */
    inline BooleanArray(ptr_size_t width=nBits) : array((ptr_size_t)ceil(((double)width)/nBits),0) {}
    /** Returns the width of this array in bits. The result is always a multiple of nBits. */
    inline ptr_size_t getWidth() const {return array.size()*nBits;}

	/** Returns a BitRef to the i<sup>th</sup> BIT (boolean value) of the array. @note this returns a single boolean,
	 * and the index is the bit index, not the uint index like with operator[].
	 * @return A BitRef for the i%nBits bit in the i/nBits uint. This value can be set through the BitRef class.
	 * @see BitRef @see operator[](ptr_size_t)
	 */
	inline BitRef operator[](ptr_size_t i){
	    return BitRef(array.operator[](i/nBits), i%nBits);
    }
    /** The const form of operator()(ptr_size_t). Returns a boolean value instead of a BitRef, as the value cannot be
     * changed. @see operator()(ptr_size_t) */
	inline bool operator[](ptr_size_t i) const {
	    return (array.operator[](i/nBits)&pow2(i%nBits)) != 0;
    }
	/** Resizes the length of this BooleanArray. The size (in bits) is resized to width
     * rounded up to the next multiple to nBits. Internally, this calls Array.resize.
     * @see Array.resize
     */
	inline void resize(ptr_size_t width){array.resize((int)ceil(((double)width)/nBits));}
    /** Sets all boolean values in this BooleanArray to the passed boolean value. */
	inline void setAll(bool b){memset(array.data(), (b)?uint_max:0, array.size()*sizeof(uint));}

	private: std::vector<uint> array;
};

#endif // CORE_BOOLEANARRAY_H_INCLUDED
