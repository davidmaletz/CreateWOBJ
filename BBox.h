/** @file BBox.h
 * Template classes for 2D and 3D axis-aligned bounding boxes.
 */

#ifndef CORE_BBOX_H_INCLUDED
#define CORE_BBOX_H_INCLUDED

/** A 2D axis-aligned bounding box. The template parameter determines what format to store the coordinates
 * (int, float, double etc). Stores the bottom left and top right corners of the bounding box, and contains
 * many utility functions for handling 2D bounding boxes.
 * A set of examples and unit tests for the BBox2D class can be found here: BBoxTest.h
 */
template <class E> class BBox2D {
    public:
    /** The bottom left corner of the bounding box. */
    vec2<E> botLeft,
    /** The top right corner of the bounding box. */
    topRight;
    /** Default constructor, initializes to an empty bounding box. The bottom left corner is set to the maximum
     * value of the coordinates, and the top right corner is set to the minimum value of the coordinates. This
     * means the bounding box is empty (bottom left corner >= top right corner), but this specific degenerate (empty)
     * bounding box will not add any area when used in operator+ and operator+=.
     */
    inline BBox2D() : botLeft(vec2<E>::make(MAX_VALUE(E),MAX_VALUE(E))), topRight(vec2<E>::make(MIN_VALUE(E),MIN_VALUE(E))){}
    /** Constructs a bounding box from the bottom left and top right corners.
     * @param bl The bottom left corner. @param tr The top right corner.
     */
    inline BBox2D(const vec2<E> &bl, const vec2<E> &tr) : botLeft(bl), topRight(tr) {}
    /** Constructs a bounding box from the bottom left and top right corners.
     * This allows you to pass the individual left, bottom, right and top coordinates of the bounding box.
     * @param minx The minimum x coordinate, meaning the left coordinate of the bounding box.
     * @param miny The minimum y coordinate, meaning the bottom coordinate of the bounding box.
     * @param minx The maximum x coordinate, meaning the right coordinate of the bounding box.
     * @param miny The maximum y coordinate, meaning the top coordinate of the bounding box.
     * @see BBox2D(const vec2<E> &, const vec2<E> &)
     */
    inline BBox2D(E minx, E miny, E maxx, E maxy) :
        botLeft(vec2<E>::make(minx, miny)), topRight(vec2<E>::make(maxx, maxy)) {}
    /** Creates a BBox2D from the passed BBox2D, converting types if necessary. */
    template<class T> inline BBox2D(const BBox2D<T>& b) : botLeft(vec2<E>::make(b.botLeft)), topRight(vec2<E>::make(b.topRight)) {}

    /** Determines if this bounding box is empty.
     * @return True if the bounding box is empty, meaning that the left coordinate is greater than or equal
     * to the right coordinate or the bottom coordinate is greater than or equal to the top coordinate. In either case
     * the bounding box is degenerate, having no area.
     */
    inline bool empty() const {return (botLeft.x >= topRight.x) | (botLeft.y >= topRight.y);}
    /** Determines if this bounding box is valid.
     * @return True if the bounding box is valid, meaning that the box contains at least one point. */
    inline bool valid() const {return (botLeft.x <= topRight.x) & (botLeft.y <= topRight.y);}
    /** Determines if this bounding box contains a coordinate along a single axis.
     * @param x The coordinate along the axis to check.
     * @param axis The axis the coordinate is along.
     * @return True if the bounding box contains the coordinate along the passed axis. For example,
     * if axis was X_AXIS, then this would return true if the coordinate was between the left and right
     * coordinates of this bounding box.
     */
	inline bool contains(const E x, AXIS axis) const {return (x >= botLeft[axis]) & (x <= topRight[axis]);}
	/** Determines if this bounding box contains a single point. This is true if the bounding box contains the
	 * x coordinate of the point along the X_AXIS and the y coordinate of the point along the Y_AXIS.
	 * @see contains(const E, AXIS) const
	 */
	inline bool contains(const vec2<E> &pt) const {return contains(pt.x, X_AXIS) & contains(pt.y, Y_AXIS);}
	/** Determines if this bounding box completely contains another bounding box. This will only return
	* true if the entire area of the passed bounding box is contained within this bounding box. @see intersects */
	inline bool contains(const BBox2D<E> &b) const {return (b.botLeft.x >= botLeft.x) & (b.topRight.x <= topRight.x) & (b.botLeft.y >= botLeft.y) & (b.topRight.y <= topRight.y);}
	/** Determines if this bounding box intersects another bounding box. This will return true if any part of this bounding
	* box intersects any part of the passed bounding box. This function is symmetric, meaning if a intersects b, b intersects a.
	*/
	inline bool intersects(const BBox2D<E> &b) const {return (b.topRight.x >= botLeft.x) & (b.botLeft.x <= topRight.x) & (b.topRight.y >= botLeft.y) & (b.botLeft.y <= topRight.y);}

	/** Returns a bounding box for the intersection of two axis aligned bounding boxes. The returned bounding box contains
	 * any area that both bounding boxes contain. If the bounding boxes do not intersect, the resulting bounding box
	 * will be empty.
	 * @see intersects
	 */
    inline BBox2D<E> intersection(const BBox2D<E>& b) const {return BBox2D<E>(max(botLeft,b.botLeft), min(topRight,b.topRight));}
  
    /** Adds another bounding box to this bounding box. The bounds of this bounding box will be expanded to the smallest
     * size that can contain both this bounding box and the passed bounding box.
     * @note A degenerate bounding box (empty() returns true) may still add to the combined area. Only a default
     * constructed bounding box is guaranteed not to add to the combined area of one bounding box and another.
     */
	BBox2D<E>& operator +=(const BBox2D<E> &b){
		botLeft.x = min(botLeft.x, b.botLeft.x);
		botLeft.y = min(botLeft.y, b.botLeft.y);
		topRight.x = max(topRight.x, b.topRight.x);
		topRight.y = max(topRight.y, b.topRight.y);
		return *this;
	}
	/** Returns the smallest bounding box that can contain both this bounding box and the passed bounding box.
	 * This is the same as operator+=, except it does not modify this bounding box, and instead returns a new one.
	 * @see operator+=(const BBox2D<E> &)
	 */
	BBox2D<E> operator+(const BBox2D<E> &b) const {
		vec2<E> bl, tr;
		bl.x = min(botLeft.x, b.botLeft.x);
		bl.y = min(botLeft.y, b.botLeft.y);
		tr.x = max(topRight.x, b.topRight.x);
		tr.y = max(topRight.y, b.topRight.y);
		return BBox2D<E>(bl, tr);
	}
    /** Adds a point to this bounding box. The bounds of this bounding box will be expanded to the smallest
     * size that can contain both this bounding box and the passed point.
     * @note A degenerate bounding box (empty() returns true) may still add to the combined area. Only a default
     * constructed bounding box is guaranteed not to add to the combined area of one bounding box and a point.
     */
	BBox2D<E>& operator +=(const vec2<E> &v){
		botLeft.x = min<E>(botLeft.x, v.x);
		botLeft.y = min<E>(botLeft.y, v.y);
		topRight.x = max<E>(topRight.x, v.x);
		topRight.y = max<E>(topRight.y, v.y);
		return *this;
	}
	/** Returns the smallest bounding box that can contain both this bounding box and the passed point.
	 * This is the same as operator+=, except it does not modify this bounding box, and instead returns a new one.
	 * @see operator+=(const vec2<E> &)
	 */
	BBox2D<E> operator+(const vec2<E> &v) const {
		vec2<E> bl, tr;
		bl.x = min<E>(botLeft.x, v.x);
		bl.y = min<E>(botLeft.y, v.y);
		tr.x = max<E>(topRight.x, v.x);
		tr.y = max<E>(topRight.y, v.y);
		return BBox2D<E>(bl, tr);
	}
	/** Returns a random point within the area of this bounding box. Uses c rand, which should be set using srand
	 * to change the order of the sampled points. If this bounding box is degenerate, the result is undefined.
	 */
	vec2<E> sample() const {
		vec2<E> ret;
		ret.x = botLeft.x+randi()*(topRight.x-botLeft.x);
		ret.y = botLeft.y+randi()*(topRight.y-botLeft.y);
		return ret;
	}

    inline vec2<E> getClosest(const vec2<E>& v) const {
        vec2<E> c = center(); vec2<E> closest = v-c; vec2<E> half_dim = (topRight-botLeft)*E(0.5); closest = max(closest, -half_dim);
        closest = min(closest, half_dim); return closest+c;
    }
    /** Returns the mimimum distance between this bounding box and a point. If this bounding box contains
     * the passed point, it will return 0. If this bounding box is degenerate, the result is undefined.
     */
	inline double distance(const vec2<E>& v) const {if(contains(v)) return 0; else return core::distance(v, getClosest(v));}

    /** Returns the length of this bounding box along the passed axis.
     * If this bounding box is degenerate, this may return a number less than or equal to 0. */
	inline E length(AXIS axis) const {return topRight[axis]-botLeft[axis];}
	/** A synonym for length(X_AXIS). @see length */
	inline E width() const {return length(X_AXIS);}
	/** A synonym for length(Y_AXIS). @see length */
	inline E height() const {return length(Y_AXIS);}
	/** Returns the width times the height of this bounding box (the area).
	 * If this bounding box is degenerate, it may return a positive or negative value (as it uses the result of
     * width() and height(), which can be less than or equal to 0 if the bounding box is degenerate). The true area
	 * should be 0 if empty() returns true. @see width @see height
	 */
	inline E area() const {return width()*height();}
	/** Returns the center of this bounding box.
	 * The result is halfway between the bottom left and top right corners. If this bounding box is degenerate, the result is undefined.
	 */
	inline vec2<E> center() const {return (botLeft+topRight)*E(0.5);}
	inline void expand(const vec2<E>& v){botLeft -= v; topRight += v;}
	inline void scale(const vec2<E>& v){vec2<E> c = center(); botLeft = (botLeft-c)*v+c; topRight = (topRight-c)*v+c;}

    /** Returns true if this bounding box has the exact same bottom left and top right corners as the passed bounding box.
     * Note that degenerate bounding boxes might not be equal even though they are both empty.
     */
	inline bool operator==(const BBox2D<E> &b) const {return (b.botLeft == botLeft) & (b.topRight == topRight);}
	/** A synonym for !operator==. @see operator==(const BBox2D<E> &) const */
	inline bool operator!=(const BBox2D<E> &b) const {return (b.botLeft != botLeft) | (b.topRight != topRight);}

};

/** A 3D axis-aligned bounding box. The template parameter determines what format to store the coordinates
 * (int, float, double etc). Stores the bottom left and top right corners of the bounding box, and contains
 * many utility functions for handling 3D bounding boxes.
 * A set of examples and unit tests for the BBox3D class can be found here: BBoxTest.h
 */
template <class E> class BBox3D {
    public:
    /** The bottom left near corner of the bounding box. From now on, this will be referred to the bottom left
     * corner, but keep in mind this also contains the minimum z value (near). */
    vec3<E> botLeft,
    /** The top right far corner of the bounding box. From now on, this will be referred to the top right
     * corner, but keep in mind this also contains the maximum z value (far). */
    topRight;
    /** Default constructor, initializes to an empty bounding box. The bottom left corner is set to the maximum
     * value of the coordinates, and the top right corner is set to the minimum value of the coordinates. This
     * means the bounding box is empty (bottom left corner >= top right corner), but this specific degenerate (empty)
     * bounding box will not add any volume when used in operator+ and operator+=.
     */
    inline BBox3D() : botLeft(vec3<E>::make(MAX_VALUE(E),MAX_VALUE(E),MAX_VALUE(E))), topRight(vec3<E>::make(MIN_VALUE(E),MIN_VALUE(E),MIN_VALUE(E))){}
    /** Constructs a bounding box from the bottom left and top right corners.
     * @param bl The bottom left near corner. @param tr The top right far corner.
     */
    inline BBox3D(const vec3<E> &bl, const vec3<E> &tr) : botLeft(bl), topRight(tr) {}
    /** Constructs a bounding box from the bottom left and top right corners.
     * This allows you to pass the individual left, bottom, near, right and top, far coordinates of the bounding box.
     * @param minx The minimum x coordinate, meaning the left coordinate of the bounding box.
     * @param miny The minimum y coordinate, meaning the bottom coordinate of the bounding box.
     * @param minz The minimum z coordinate, meaning the near coordinate of the bounding box.
     * @param minx The maximum x coordinate, meaning the right coordinate of the bounding box.
     * @param miny The maximum y coordinate, meaning the top coordinate of the bounding box.
     * @param miny The maximum z coordinate, meaning the far coordinate of the bounding box.
     * @see BBox3D(const vec3<E> &, const vec3<E> &)
     */
    inline BBox3D(E minx, E miny, E minz, E maxx, E maxy, E maxz) :
        botLeft(vec3<E>::make(minx, miny, minz)), topRight(vec3<E>::make(maxx, maxy, maxz)) {}
    /** Creates a BBox3D from the passed BBox3D, converting types if necessary. */
    template<class T> inline BBox3D(const BBox3D<T>& b) : botLeft(vec3<E>::make(b.botLeft)), topRight(vec3<E>::make(b.topRight)) {}
    /** Determines if this bounding box is empty.
     * @return True if the bounding box is empty, meaning that the left coordinate is greater than or equal
     * to the right coordinate, the bottom coordinate is greater than or equal to the top coordinate or
     * the near coordinate is greater than or equal to the far coordinate. In any case
     * the bounding box is degenerate, having no volume.
     */
    inline bool empty() const {return (botLeft.x >= topRight.x) | (botLeft.y >= topRight.y) | (botLeft.z >= topRight.z);}
    /** Determines if this bounding box is valid.
     * @return True if the bounding box is valid, meaning that the box contains at least one point. */
    inline bool valid() const {return (botLeft.x <= topRight.x) & (botLeft.y <= topRight.y) & (botLeft.z <= topRight.z);}
    /** Determines if this bounding box contains a coordinate along a single axis.
     * @param x The coordinate along the axis to check.
     * @param axis The axis the coordinate is along.
     * @return True if the bounding box contains the coordinate along the passed axis. For example,
     * if axis was X_AXIS, then this would return true if the coordinate was between the left and right
     * coordinates of this bounding box.
     */
	inline bool contains(const E x, AXIS axis) const {return (x >= botLeft[axis]) & (x <= topRight[axis]);}
	/** Determines if this bounding box contains a single point. This is true if the bounding box contains the
	 * x coordinate of the point along the X_AXIS, the y coordinate of the point along the Y_AXIS and the z
	 * coordinate of the point along the Z_AXIS.
	 * @see contains(const E, AXIS) const
	 */
	inline bool contains(const vec3<E> &pt) const {return contains(pt.x, X_AXIS) & contains(pt.y, Y_AXIS) & contains(pt.z, Z_AXIS);}
	/** Determines if this bounding box completely contains another bounding box. This will only return
	* true if the entire volume of the passed bounding box is contained within this bounding box. @see intersects */
	inline bool contains(const BBox3D<E> &b) const {return (b.botLeft.x >= botLeft.x) & (b.topRight.x <= topRight.x) & (b.botLeft.y >= botLeft.y) & (b.topRight.y <= topRight.y) & (b.botLeft.z >= botLeft.z) & (b.topRight.z <= topRight.z);}
	/** Determines if this bounding box intersects another bounding box. This will return true if any part of this bounding
	* box intersects any part of the passed bounding box. This function is symmetric, meaning if a intersects b, b intersects a.
	*/
	inline bool intersects(const BBox3D<E> &b) const {return (b.topRight.x >= botLeft.x) & (b.botLeft.x <= topRight.x) & (b.topRight.y >= botLeft.y) & (b.botLeft.y <= topRight.y) & (b.topRight.z >= botLeft.z) & (b.botLeft.z <= topRight.z);}

	/** Returns a bounding box for the intersection of two axis aligned bounding boxes. The returned bounding box contains
	 * any volume that both bounding boxes contain. If the bounding boxes do not intersect, the resulting bounding box
	 * will be empty.
	 * @see intersects
	 */
    inline BBox3D<E> intersection(const BBox3D<E>& b) const {return BBox3D<E>(max(botLeft,b.botLeft), min(topRight,b.topRight));}

    /** Adds another bounding box to this bounding box. The bounds of this bounding box will be expanded to the smallest
     * size that can contain both this bounding box and the passed bounding box.
     * @note A degenerate bounding box (empty() returns true) may still add to the combined volume. Only a default
     * constructed bounding box is guaranteed not to add to the combined volume of one bounding box and another.
     */
	BBox3D<E>& operator +=(const BBox3D<E> &b){
		botLeft.x = min(botLeft.x, b.botLeft.x);
		botLeft.y = min(botLeft.y, b.botLeft.y);
		botLeft.z = min(botLeft.z, b.botLeft.z);
		topRight.x = max(topRight.x, b.topRight.x);
		topRight.y = max(topRight.y, b.topRight.y);
		topRight.z = max(topRight.z, b.topRight.z);
		return *this;
	}
	/** Returns the smallest bounding box that can contain both this bounding box and the passed bounding box.
	 * This is the same as operator+=, except it does not modify this bounding box, and instead returns a new one.
	 * @see operator+=(const BBox3D<E> &)
	 */
	BBox3D<E> operator+(const BBox3D<E> &b) const {
		vec3<E> bl, tr;
		bl.x = min(botLeft.x, b.botLeft.x);
		bl.y = min(botLeft.y, b.botLeft.y);
		bl.z = min(botLeft.z, b.botLeft.z);
		tr.x = max(topRight.x, b.topRight.x);
		tr.y = max(topRight.y, b.topRight.y);
		tr.z = max(topRight.z, b.topRight.z);
		return BBox3D<E>(bl, tr);
	}
    /** Adds a point to this bounding box. The bounds of this bounding box will be expanded to the smallest
     * size that can contain both this bounding box and the passed point.
     * @note A degenerate bounding box (empty() returns true) may still add to the combined volume. Only a default
     * constructed bounding box is guaranteed not to add to the combined volume of one bounding box and a point.
     */
	BBox3D<E>& operator +=(const vec3<E> &v){
		botLeft.x = min<E>(botLeft.x, v.x);
		botLeft.y = min<E>(botLeft.y, v.y);
		botLeft.z = min<E>(botLeft.z, v.z);
		topRight.x = max<E>(topRight.x, v.x);
		topRight.y = max<E>(topRight.y, v.y);
		topRight.z = max<E>(topRight.z, v.z);
		return *this;
	}
	/** Returns the smallest bounding box that can contain both this bounding box and the passed point.
	 * This is the same as operator+=, except it does not modify this bounding box, and instead returns a new one.
	 * @see operator+=(const vec3<E> &)
	 */
	BBox3D<E> operator+(const vec3<E> &v) const {
		vec3<E> bl, tr;
		bl.x = min<E>(botLeft.x, v.x);
		bl.y = min<E>(botLeft.y, v.y);
		bl.z = min<E>(botLeft.z, v.z);
		tr.x = max<E>(topRight.x, v.x);
		tr.y = max<E>(topRight.y, v.y);
		tr.z = max<E>(topRight.z, v.z);
		return BBox3D<E>(bl, tr);
	}
	/** Returns a random point within the volume of this bounding box. Uses c rand, which should be set using srand
	 * to change the order of the sampled points. If this bounding box is degenerate, the result is undefined.
	 */
	vec3<E> sample() const {
		vec3<E> ret;
		ret.x = botLeft.x+randi()*(topRight.x-botLeft.x);
		ret.y = botLeft.y+randi()*(topRight.y-botLeft.y);
		ret.z = botLeft.z+randi()*(topRight.z-botLeft.z);
		return ret;
	}

    inline vec3<E> getClosest(const vec3<E>& v) const {
        vec3<E> c = center(); vec3<E> closest = v-c; vec3<E> half_dim = (topRight-botLeft)*E(0.5); closest = max(closest, -half_dim);
        closest = min(closest, half_dim); return closest+c;
    }
    /** Returns the mimimum distance between this bounding box and a point. If this bounding box contains
     * the passed point, it will return 0. If this bounding box is degenerate, the result is undefined.
     */
	inline double distance(const vec3<E>& v) const {if(contains(v)) return 0; else return core::distance(v, getClosest(v));}

    /** Returns the length of this bounding box along the passed axis.
     * If this bounding box is degenerate, this may return a number less than or equal to 0. */
	inline E length(AXIS axis) const {return topRight[axis]-botLeft[axis];}
	/** A synonym for length(X_AXIS). @see length */
	inline E width() const {return length(X_AXIS);}
	/** A synonym for length(Y_AXIS). @see length */
	inline E height() const {return length(Y_AXIS);}
	/** A synonym for length(Z_AXIS). @see length */
	inline E depth() const {return length(Z_AXIS);}
	/** Returns the surface area of this bounding box. The surface area @f$ = 2*w*h+2*w*d+2*h*d @f$ where w is
	 * the result of width(), h is the result of height() and d is the result of depth().
	 * If this bounding box is degenerate, it may return a positive or negative value (as it uses the result of
     * width(), height() and depth(), which can be less than or equal to 0 if the bounding box is degenerate). The true surface area
	 * should be 0 if empty() returns true. @see width @see height @see depth
	 */
	inline E area() const {
	    E w = width(), h = height(), d = depth();
	    return 2*w*h+2*w*d+2*h*d;
	}
	/** Returns the volume of this bounding box (the width times the height times the depth).
	 * If this bounding box is degenerate, it may return a positive or negative value (as it uses the result of
     * width(), height() and depth(), which can be less than or equal to 0 if the bounding box is degenerate). The true volume
	 * should be 0 if empty() returns true. @see width @see height @see depth
	 */
	inline E volume() const {return width()*height()*depth();}
	/** Returns the center of this bounding box.
	 * The result is halfway between the bottom left and top right corners. If this bounding box is degenerate, the result is undefined.
	 */
	inline vec3<E> center() const {return (botLeft+topRight)*E(0.5);}
	inline void expand(const vec3<E>& v){botLeft -= v; topRight += v;}
	inline void scale(const vec3<E>& v){vec3<E> c = center(); botLeft = (botLeft-c)*v+c; topRight = (topRight-c)*v+c;}

    /** Returns true if this bounding box has the exact same bottom left and top right corners as the passed bounding box.
     * Note that degenerate bounding boxes might not be equal even though they are both empty.
     */
	inline bool operator==(const BBox3D<E> &b) const {return (b.botLeft == botLeft) & (b.topRight == topRight);}
	/** A synonym for !operator==. @see operator==(const BBox3D<E> &) const */
	inline bool operator!=(const BBox3D<E> &b) const {return (b.botLeft != botLeft) | (b.topRight != topRight);}
};

#endif // CORE_BBOX_H_INCLUDED
