#ifndef CORE_RENDERER_VERTEXFORMAT_H_INCLUDED
#define CORE_RENDERER_VERTEXFORMAT_H_INCLUDED

#include "NormalizeValue.h"
#include "TypeToken.h"
#include "vec.h"
#include "half_float.h"

#include <vector>

/** A function which converts a pointer to a vertex attribute in any format to a float4 value. */
typedef float4 (*AttribGetFunc)(const void*);
/** A function which sets the value of a pointer to a vertex attribute in any format to the passed float4 value. */
typedef void (*AttribSetFunc)(void*, float4 val);

/** A function which converts a pointer to a vertex attribute in any format to a float4 value. */
typedef uint (*IndexGetFunc)(const void*);
/** A function which sets the value of a pointer to a vertex attribute in any format to the passed float4 value. */
typedef void (*IndexSetFunc)(void*, uint val);

template<typename TYPE, uchar n_elem, bool normalized> class VertexAttrib ;
template<typename TYPE> class VertexAttrib<TYPE, 1, true> {
    public:
    typedef TYPE type; enum{n_elem=1, normalized=true};
    TYPE elem[1];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(normalizeValue<TYPE,float>(a[0]),0,0,1);
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = normalizeValue<float,TYPE>(val.x);
    }
};
template<typename TYPE> class VertexAttrib<TYPE, 1, false> {
    public:
    typedef TYPE type; enum{n_elem=1, normalized=false};
    TYPE elem[1];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(float(a[0]),0,0,1);
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = TYPE(val.x);
    }
};
template<typename TYPE> class VertexAttrib<TYPE, 2, true> {
    public:
    typedef TYPE type; enum{n_elem=2, normalized=true};
    TYPE elem[2];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(normalizeValue<TYPE,float>(a[0]),normalizeValue<TYPE,float>(a[1]),0,1);
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = normalizeValue<float,TYPE>(val.x); a[1] = normalizeValue<float,TYPE>(val.y);
    }
};
template<typename TYPE> class VertexAttrib<TYPE, 2, false> {
    public:
    typedef TYPE type; enum{n_elem=2, normalized=false};
    TYPE elem[2];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(float(a[0]),float(a[1]),0,1);
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = TYPE(val.x); a[1] = TYPE(val.y);
    }
};
template<typename TYPE> class VertexAttrib<TYPE, 3, true> {
    public:
    typedef TYPE type; enum{n_elem=3, normalized=true};
    TYPE elem[3];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(normalizeValue<TYPE,float>(a[0]),normalizeValue<TYPE,float>(a[1]),normalizeValue<TYPE,float>(a[2]),1);
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = normalizeValue<float,TYPE>(val.x); a[1] = normalizeValue<float,TYPE>(val.y);
        a[2] = normalizeValue<float,TYPE>(val.z);
    }
};
template<typename TYPE> class VertexAttrib<TYPE, 3, false> {
    public:
    typedef TYPE type; enum{n_elem=3, normalized=false};
    TYPE elem[3];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(float(a[0]),float(a[1]),float(a[2]),1);
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = TYPE(val.x); a[1] = TYPE(val.y); a[2] = TYPE(val.z);
    }
};
template<typename TYPE> class VertexAttrib<TYPE, 4, true> {
    public:
    typedef TYPE type; enum{n_elem=4, normalized=true};
    TYPE elem[4];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(normalizeValue<TYPE,float>(a[0]),normalizeValue<TYPE,float>(a[1]),normalizeValue<TYPE,float>(a[2]),
                            normalizeValue<TYPE,float>(a[3]));
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = normalizeValue<float,TYPE>(val.x); a[1] = normalizeValue<float,TYPE>(val.y);
        a[2] = normalizeValue<float,TYPE>(val.z); a[3] = normalizeValue<float,TYPE>(val.w);
    }
};
template<typename TYPE> class VertexAttrib<TYPE, 4, false> {
    public:
    typedef TYPE type; enum{n_elem=4, normalized=false};
    TYPE elem[4];
    static float4 getAttrib(const void* attrib){
        const TYPE* a = (const TYPE*)attrib;
        return float4::make(float(a[0]),float(a[1]),float(a[2]),float(a[3]));
    }
    static void setAttrib(void* attrib, float4 val){
        TYPE* a = (TYPE*)attrib;
        a[0] = TYPE(val.x); a[1] = TYPE(val.y); a[2] = TYPE(val.z); a[3] = TYPE(val.w);
    }
};

class AttribType {
    public:
    union{
        struct{
        /** The generic index of the vertex attribute. @see VertexAttribIndices */ ushort offset:6,
        /** The number of bytes needed for this vertex attribute. */ bpa:6,
        /** The number of elements (1 to 4) in this vertex attribute. */ numElements:3,
        /** Whether or not this vertex attribute should be normalized. */ normalized:1;
        };
        ushort attrib_info;
    };
    /** The type of each element in this vertex attribute. @see TypeToken */ TypeToken elementType;
    /** The AttribGetFunc for getting this vertex attribute. @see AttribGetFunc */ AttribGetFunc getAttrib;
    /** The AttribSetFunc for setting this vertex attribute. @see AttribSetFunc */ AttribSetFunc setAttrib;
    /** Returns whether this AttribType is equal to the passed AttribType or not. */
	inline bool operator==(const AttribType &mf) const {return (offset == mf.offset) & (elementType == mf.elementType) & (numElements == mf.numElements) & (normalized == mf.normalized);}
	/** Returns whether this AttribType is not equal to the passed AttribType or not. */
	inline bool operator!=(const AttribType &mf) const{return !operator==(mf);}
};

/** Creates an AttribType object from the passed generic index and the template parameters.
 * @param _index The generic index of the vertex attribute. @see VertexAttribIndices
 * @param TYPE The type of each element of the attribute.
 * @param n_elem The number of elements in the attribute, must be one to four.
 * @param normalized If true, integer TYPE values will be normalized from -1 to 1, and unsigned integer TYPE values
 * will be normalized from 0 to 1 when converted to float values. Otherwise, the conversion to float values will store
 * the actual integer value. For example, a uchar4 color should be normalized to get colors in the range of 0 to 1,
 * but ushort2 pixel positions (for GUI elements) should not be normalized. */
template<typename TYPE, uchar n_elem, bool normalized> inline AttribType createAttribType(uchar _offset){
    typedef VertexAttrib<TYPE, n_elem, normalized> VA;
    AttribType ret = {{{_offset, sizeof(TYPE)*n_elem, n_elem, normalized}}, typeToken<TYPE>(), &VA::getAttrib, &VA::setAttrib}; return ret;
}

class VertexFormat {
	friend class VertexBuffer;
	std::vector<AttribType> attributes; uchar bpv;
public:
	inline VertexFormat() : bpv(0){}
	template<typename TYPE, uchar n_elem, bool normalized> inline void addAttribute(){
		AttribType type = createAttribType<TYPE, n_elem, normalized>(bpv);
		attributes.push_back(type); bpv += type.bpa;
	}
	inline uchar getBytesPerVertex() const {return bpv;}
};

class IndexFormat {
	friend class IndexBuffer;
	uchar bpi;
	template<typename TYPE> static uint getIndex(const void* data){
		TYPE* a = (TYPE*)data; return a[0];
	}
	template<typename TYPE> static void setIndex(void* data, uint value){
		TYPE* a = (TYPE*)data; a[0] = (TYPE)value;
	}
public:
	IndexFormat(int vertex_count){
		if(vertex_count < uchar_max){bpi = 1; get = &getIndex<uchar>; set = &setIndex<uchar>;}
		else if(vertex_count < ushort_max){bpi = 2; get = &getIndex<ushort>; set = &setIndex<ushort>;}
		else {bpi = 4; get = &getIndex<uint>; set = &setIndex<uint>;}
	}
	IndexGetFunc get; IndexSetFunc set;
};

class VertexBuffer {
	void* data; const VertexFormat* format; int vertices;
	inline void* offset(int vertex, int attribute) const {return bufferOffset(data, vertex*format->bpv+format->attributes[attribute].offset);}
public:
	VertexBuffer(const VertexFormat* fmt, int vert) : data(malloc(fmt->bpv*vert)), format(fmt), vertices(vert) {memset(data, 0, fmt->bpv*vert);}
	~VertexBuffer(){free(data);}
	inline void set(int vertex, int attribute, const float4& value){
		format->attributes[attribute].setAttrib(offset(vertex, attribute), value);
	}
	inline float4 get(int vertex, int attribute) const {
		return format->attributes[attribute].getAttrib(offset(vertex, attribute));
	}
	inline int getVertexCount() const {return vertices;}
	inline const void* getBytes() const {return data;}
	inline int getSize() const {return format->bpv*vertices;}
};

class IndexBuffer {
	void* data; const IndexFormat* format; int indices;
	inline void* offset(int i) const {return bufferOffset(data, i*format->bpi);}
public:
	IndexBuffer(const IndexFormat* fmt, int count) : data(malloc(fmt->bpi*count)), format(fmt), indices(count) {memset(data, 0, fmt->bpi*count);}
	~IndexBuffer(){free(data);}
	inline void set(int i, uint value){format->set(offset(i), value);}
	inline uint get(int i) const {return format->get(offset(i));}
	inline int getIndexCount() const {return indices;}
	inline const void* getBytes() const {return data;}
	inline int getSize() const {return format->bpi*indices;}
};

#endif // CORE_RENDERER_VERTEXFORMAT_H_INCLUDED