#pragma once
// ****************************************************************************
// CREATED_ON	: 27-09-05
// CREATED_BY	: Martin Lutken
// ----------------------------------------------------------------------------
// COPYRIGHT	:
// ****************************************************************************


#include <math/base/v2.hpp>
#include <math/base/g33.hpp>


namespace lue::math {


/// DOXYS_ON

/**
Rectangle (2D) defined an upper-left corner, size. */
template
<
    typename T
>
class rectangle
{
    // ----------------
    // --- Typedefs ---
    // ----------------

public:
    using value_type                    = rectangle<T>;
    typedef value_type&						reference;
    typedef const value_type&				const_reference;
    typedef value_type*						pointer;
    typedef const value_type*				const_pointer;


    // --- vector typedefs ---
    typedef v2<T>					v2_value_type;
    typedef v2_value_type&			v2_reference;
    typedef const v2_value_type&	v2_const_reference;


    // --------------------------------------------
    // --- Constructors / Set values functions. ---
    // --------------------------------------------

    /// Default constructor with no initialization.
    rectangle() = default;


//    /// Copy constructor. Initializing with another rectangle.
//    rectangle(const rectangle& other) = default;

    /** Constructor with size onnly initialization. */
    explicit rectangle(const v2<T>& size)		///< [in] Rectangle size
        : top_left_(T{}, T{})
        , size_(size)
    {}

    /** Constructor with 2 points initialization. */
    rectangle(	const v2<T>& top_left,		///< [in] Rectangle upper left corner
              const v2<T>& size)		///< [in] Rectangle size
        : 	top_left_(top_left), size_(size)
    {}

    /// Constructor with 4 values initialization.
    rectangle(	T x,	///< [in] Rectangle top left corner X
                T y,	///< [in] Rectangle top left corner Y
                T w,	///< [in] Rectangle width
                T h)	///< [in] Rectangle height
        : top_left_(x, y)
        , size_(w, h) {}

    /// Cast to rectangle with different number-type 'T'.
    template <typename T1>
    operator rectangle<T1>() const
    {
        typedef rectangle<T1> rectangle_value_type;
        return rectangle_value_type( top_left(), size() );
    }


    // ------------------------
    // --- Access functions ---
    // ------------------------

    /// Access upper left corner of rectangle. \return Reference to upper left corner of rectangle.
    v2_reference		top_left()			{ return top_left_; }
    v2_const_reference	top_left()	const	{ return top_left_; }

    /// Access lower right corner of rectangle. \return Reference to lower right corner of rectangle.
    v2_reference		size()			{ return size_; }
    v2_const_reference	size()	const	{ return size_; }

    /// Access x-coordinate of rectangle. This is the same as rectangle::top_left().x().
    /// \return Reference to x-coordinate of rectangle (upper left corner x-coordinate).
    T&					x()			{ return top_left_.x(); }
    T					x()	const	{ return top_left_.x(); }

    /// Access y-coordinate of rectangle. This is the same as rectangle::top_left().y().
    /// \return Reference to x-coordinate of rectangle (upper left corner y-coordinate).
    T&					y()			{ return top_left_.y(); }
    T					y()	const	{ return top_left_.y(); }

    /// Access width of rectangle. This is the same as rectangle::size().x().
    /// \return Reference to width of rectangle (lower right corner x-coordinate).
    T&					width()			{ return size_.width(); }
    T					width()	const	{ return size_.width(); }

    /// Access height of rectangle. This is the same as rectangle::size().y().
    /// \return Reference to height of rectangle (lower right corner y-coordinate).
    T&					height()			{ return size_.height(); }
    T					height()	const	{ return size_.height(); }


    // ----------------------------
    // --- Set values functions ---
    // ----------------------------

    /** Set rectangle with  points. */
    void		set(v2_const_reference rect_top_left,		///< [in] Rectangle upper left corner
                    v2_const_reference rect_size)		///< [in] Rectangle size
    {
        top_left() = rect_top_left;
        size() = rect_size;
    }


    /** Set rectangle with 4 values. */
    void		set(T top_left_x,			///< [in] Rectangle upper left corner, X-coordinate.
                    T top_left_y,			///< [in] Rectangle upper left corner, Y-coordinate.
                    T width,			///< [in] Rectangle width.
                    T height)			///< [in] Rectangle height.
    {
        top_left().set(top_left_x, top_left_y);
        size().set(width, height);
    }



    // ----------------------------
    // --- Assignment operators ---
    // ----------------------------

    /** '=' operator, Assignment from another rectangle.
        \note Here we allways copy values to destination. In case this rectangle does not
              use the PRectangleStoreByValue - then it is still values that are copied - not
              pointers or references, that are assigned. */
    template < typename T1 >
    reference			operator =(const rectangle<T1>& rect)		///< [in] Right hand side operand.
    {
        top_left() = rect.top_left();
        size() = rect.size();
        return *this;
    }

    // ------------------------------------
    // --- ---
    // ------------------------------------
   /** Translate rectangle to other position.
       \return Reference to "this".*/
   reference			translate(v2_value_type v)		///< [in] Vector to translate with.
   {
       top_left() += v;
       return *this;
   }


    // ------------------------------------
    // --- Add/Substract math operators ---
    // ------------------------------------

//    /** '+=' operator, Add vector to rectangle. Translate rectangle to other position.
//        \return Reference to "this".*/
//    reference			operator +=(v2_value_type v)		///< [in] Vector to add to all points.
//    {
//        top_left() += v;
//        size() += v;
//        return *this;
//    }


//    /** '-=' operator, Subtract vector from rectangle. Translate rectangle to other position.
//        \return Reference to "this".*/
//    reference			operator -=(v2_value_type v)		///< [in] Vector to subtract to all points.
//    {
//        top_left() -= v;
//        size() -= v;
//        return *this;
//    }

//    /** '+' operator, Add vector to rectangle. Translate rectangle to other position.
//        \return rectangle translated to the new position. */
//    value_value_type	operator +(	v2_const_reference v		///< [in] Vector to add to all points.
//                              )const
//    {
//        return value_value_type( top_left() + v, size() + v);
//    }

//    /** '-' operator, Subtract vector from rectangle. Translate rectangle to other position.
//        \return rectangle translated to the new position. */
//    value_value_type	operator -(	v2_const_reference v		///< [in] Vector to subtract to all points.
//                              )const
//    {
//        return value_value_type( top_left() - v, size() - v);
//    }


    // ----------------------------------------------
    // --- Multiplication/division math operators ---
    // ----------------------------------------------
    /** '*' operator, Scale rectangle with a scalar.
        \return Rectangle which is scaled element wise in position and size using the scalar. */
    value_type	operator *(T scalar		///< [in] Right hand side operand.
                         ) const
    {
        const auto top_left = top_left_*scalar;
        const auto size = size_*scalar;
        return value_type(top_left, size);
    }

    /** '*' operator, Scale rectangle with a vector.
        \return Rectangle which is scaled element wise in position and size using the vector. */
    template <typename T1>
    value_type	operator *(v2<T1> vec		///< [in] Right hand side operand.
                         ) const
    {
        const auto top_left = top_left_*vec;
        const auto size = size_*vec;
        return value_type(top_left, size);
    }




    // ----------------------
    // --- Util functions ---
    // ----------------------

    /// Access width of rectangle. \return Width of rectangle.
    T					w()	const	{ return size_.width(); }
    T&					w()         { return size_.width(); }

    /// Get height of rectangle. \return Height of rectangle.
    T					h()	const	{ return size_.height(); }
    T&					h()         { return size_.height(); }

    /// Get lower left corner of rectangle. \return Copy (2D vector as value type) of lower left corner of rectangle.
    v2_value_type		lower_left() const		{ return v2_value_type( top_left().x(), top_left().y() + size_.height() ); }

    /// Get lower right corner of rectangle. \return Copy (2D vector as value type) of lower left corner of rectangle.
    v2_value_type		lower_right() const		{ return v2_value_type( top_left().x() + size_.width(), top_left().y() + size_.height() ); }

    /// Get upper right corner of rectangle. \return Copy (2D vector as value type) of upper right corner of rectangle.
    v2_value_type		upper_right() const		{ return v2_value_type( top_left().x() + size_.width(), top_left().y() ); }

//    /** Get vector from rectangle upper left corner to lower right corner (diagonal) \return Vector from upper left corner to lower right corner. */
//    v2_value_type		vec_ul_size() const 	{ return size() - top_left();}

//    /** Get vector from rectangle lower right corner to upper left corner (diagonal) \return Vector from lower right corner to upper left corner. */
//    v2_value_type		vec_lr_top_left() const 	{ return top_left() - size();}

    /** Get center of rectangle \return Center of rectangle. */
    v2_value_type		center() const 		{ return (top_left() + size()) / T(2) ;}


    /** Get area of rectangle.
        \return Area of rectangle, which is allways positive or zero. */
    T				area() const
    {
        return w()*h();
    }

   /** Test if point is inside rectangle.
   \return True if point given is inside the rectangle. */
   bool_t			point_inside(v2_const_reference p0	///< [in] Point to test
                               ) const
   {
       v2_value_type p0T = p0 - center();	// Simulate that rectangle has center in origo.
//		return (std::abs(p0T.x()) <= (w() / T(2)) ) && (std::abs(p0T.y()) <= (h() / T(2)) );
       return (std::abs(T(2)*p0T.x()) <= w() ) && (std::abs(T(2)*p0T.y()) <= h() );
   }

    // -------------------------
    // --- Compare operators ---
    // -------------------------

    /** '==' operator. Tests two rectangles for equality.
        EPSILON<T>() is used as threshold to avoid numeric instabilities. Use
        the 'equal_exact' member function if exact comparision is required.
        \return True if all corresponding elements of the two rectangles is no further,
        than EPSILON<T>() apart (absolute value), false otherwise. */
    bool_t	operator ==(const_reference r			///< [in] Right operand.
                       ) const
    {
        return	(top_left() == r.top_left()) &&
                (size() == r.size());
    }

    /** '!=' operator. Tests two rectangles for in-equality.
        EPSILON<T>() is used as threshold to avoid numeric instabilities. Use
        the 'equal_exact' member function if exact comparision is required.
        \return True if just one corresponding pair of elements of the two
        rectangles is further than EPSILON<T>() apart (absolute value),
        false otherwise. */
    bool_t	operator !=(const_reference r			///< [in] Right operand.
                       ) const
    {
        return	(top_left() != r.top_left()) ||
                (size() != r.size());
    }


    // -------------------------
    // --- Compare functions ---
    // -------------------------


    /** Equal with exact comparision. Tests two rectangles for equality.
        No epsilon value is used. Use with care when comparing floating point
        type values with this function.
        \return True if all corresponding elements of the two rectangles are exactly
        equal, false otherwise. */
    bool_t		equal_exact(const_reference r)		///< [in] Rectangle to compare with
    {
        return	top_left().equal_exact(r.top_left()) &&
                size().equal_exact(r.size());
    }


    /** Equal with custom epsilon comparision. Tests two vectors for equality.
        Supplied epsilon value is used.
        \return True if all corresponding elements of the two vectors is no further,
        than epsilon apart (absolute value), false otherwise.. */
    bool_t		equal_epsilon(const_reference r,	///< [in] Rectangle to compare with
                              T epsilon				///< [in] Epsilon value to use when comparing
                             ) const
    {
        return	top_left().equal_epsilon(r.top_left(), epsilon) &&
                size().equal_epsilon(r.size(), epsilon);
    }


    // -----------------------
    // --- Debug Functions ---
    // -----------------------

    /// Convert rectangle to a string for debug print.
    std::string			str(int_t iDecimals= -1	///< [in] Number of decimals
                           ) const
    {
        std::string s = "UL: " + top_left().str(iDecimals);
                    s += "\nLR: " + size().str(iDecimals);
        return s;
    }

private:
    // ------------
    // --- Data ---
    // ------------
    v2<T>     top_left_    {0,0};
    v2<T>     size_        {0,0};

};


// -----------------
// --- Operators ---
// -----------------

//// '*' operator, Scalar - rectangle multiplication. Scales the rectangle with a scalar.
////template < typename T, template <class> class StoragePolicy >
//template < typename T, template <class> class StoragePolicy >
//inline rectangle<T, PRectangleStoreByValue>
//    operator *(	T Val,		 							// [in] Left Hand Side operand (scalar).
//                const rectangle<T, StoragePolicy>& t)	// [in] Right hand side operand rectangle.
//{
//    return t*Val;	// Implement using operator rectangle * T
//}



// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write rectangle to stream.
    \return Reference to (modified) ostream. */
template < typename T>
inline std::ostream&
    operator <<(std::ostream& os,		///< [out] Output stream to write to.
                const rectangle<T>& r)	///< [in] Rectangle to write.
{
    os<<r.top_left()<<r.size();
    return os;
}

/** '>>' operator. Read rectangle from input stream. */
template < typename T>
inline std::istream&
    operator >>(std::istream& is,	///< [in]  Input stream to read from.
                rectangle<T>& r)	///< [out] Rectangle recieve the data from the stream.
{
    is>>r.top_left()>>r.size();
    return is;
}

using rectf = rectangle<float>;
using rectd = rectangle<double>;
using recti32 = rectangle<std::int32_t>;
using recti64 = rectangle<std::int64_t>;


}
