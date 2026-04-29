#pragma once
// ****************************************************************************
// CREATED_ON	: 17-06-03 17:00:19
// CREATED_BY	: Martin Lutken
// ----------------------------------------------------------------------------
// COPYRIGHT	:
// ****************************************************************************

// --- Include files ---
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

#include "lue_math.h"
#include "stdmath.hpp"
#include "lue_math_internal_utils.hpp"

namespace lue::math {


template <typename T> class m22;

/**
2D vector class. Works together with matrix (m22).  The parameter type 'T' is
used as the precision type (typically float or double would be used).

The functions and operator overloads that returns a v2 vector as a value
are written so that they construct and return the v2 in a single line statement
to ease the compiler's possibilities to do "return value optimization". */
template <typename T>
class v2
{
friend class m22<T>;

public:
    // ----------------
    // --- Typedefs ---
    // ----------------
    typedef v2<T>				value_type;
    typedef value_type&			reference;
    typedef const value_type&	const_reference;
    typedef value_type*			pointer;
    typedef const value_type*	const_pointer;
    typedef unsigned_int_t		index_type;

    // ----------------------------
    // --- Const Static Members ---
    // ----------------------------

    static const value_type V_ZERO;		///< Constant zero vector.
    static const value_type V_X;		///< Constant unit vector in the X-direction.
    static const value_type V_Y;		///< Constant unit vector in the Y-direction.


public:

    // --------------------
    // --- Constructors ---
    // --------------------

    /** Default constructor with no initialization. */
    constexpr v2() = default;

    constexpr v2(const v2& v) = default;

    /** Copy constructor. Initializtion with general (v2<T1>) vector.
        Also work as casting operator from general v3<>.*/
    template <typename T1>
    constexpr v2(const v2<T1>& v)
//	: m_d[X](static_cast<T>(v.x())), m_d[Y](static_cast<T>(v.y())) {}
    {
        m_d[X] = static_cast<T>(v.x());
        m_d[Y] = static_cast<T>(v.y());
    }

    /** Copy (move) constructor. Initialization with general (v2<T1>) vector. */
    template <typename T1>
    constexpr v2(v2<T1>&& v)
    {
        m_d[X] = static_cast<T>(v.x());
        m_d[Y] = static_cast<T>(v.y());
    }

    /** Assignment. Initializtion with general (v2<T1>) vector.
        Also work as casting operator from general v3<>.*/
    template <typename T1>
    constexpr v2& operator=(const v2<T1>& v)
    {
        if (&v != this) {
            m_d[X] = static_cast<T>(v.x());
            m_d[Y] = static_cast<T>(v.y());
        }
        return *this;
    }

    /** Assignment. Initialization with general (v2<T1>) vector.
        Also work as casting operator from general v2<>.*/
    template <typename T1>
    constexpr v2& operator=(v2<T1>&& v)
    {
        if (reinterpret_cast<void*>(&v) != reinterpret_cast<void*>(this)) {
            m_d[X] = static_cast<T>(v.x());
            m_d[Y] = static_cast<T>(v.y());
        }
        return *this;
    }

    /** Constructor with single value initialization. */
    constexpr explicit v2(const T Val)						///< [in] Both x, y are set to this value
    {
        m_d[X] = Val;
        m_d[Y] = Val;
    }

    /** Constructor with 2 value initialization. */
    constexpr v2(const T fx,								///< [in] x value.
                 const T fy) 								///< [in] y value.
    {
        m_d[X] = fx; m_d[Y] = fy;
    }

    template <typename T1>
    constexpr v2<T1> cast_to() const
    {
        return v2<T1>{ static_cast<T1>(x()), static_cast<T1>(y())};
    }


    template <class SIMPLE_STRUCT_TYPE>
    constexpr SIMPLE_STRUCT_TYPE     to_struct   () const { return {x(), y()};}

    // -----------------------
    // --- Access elements ---
    // -----------------------

    constexpr T*		data()          {return m_d[0];}
    constexpr const T*	data() const	{return m_d[0];}

    /** Reference to x-coordinate.
        \return Reference to x-coordinate of vector. */
    constexpr T&			x()			{return m_d[X];}
    constexpr const T&	x() const	{return m_d[X];}

    constexpr T&		width()			{return m_d[X];}
    constexpr const T&	width() const	{return m_d[X];}

    /** Reference to y-coordinate.
        \return Reference to y-coordinate of vector. */
    constexpr T&			y()             {return m_d[Y];}
    constexpr const T&	y() const       {return m_d[Y];}
    constexpr T&			height()		{return m_d[Y];}
    constexpr const T&	height() const	{return m_d[Y];}

    /** Set x-coordinate. */
    constexpr void		x(T x)          {m_d[X] = x;}
    constexpr void		width(T x)		{m_d[X] = x;}
    /** Set y-coordinate. */
    constexpr void		y(T y)          {m_d[Y] = y;}
    constexpr void		height(T y)     {m_d[Y] = y;}


    /** [] Operator, Acces individual elements of a v2 vector.
        \return Reference to indexed element (coordinate) of vector. */
    constexpr T&					operator[](const index_type i)				///< [in] Index value. Should be in range 0..1 ~ X, Y.
    { return m_d[i]; }
    constexpr const T&			operator[](const index_type i) const
    { return m_d[i]; }


    // ------------------------------------
    // --- Simple Set And Get Functions ---
    // ------------------------------------

    /// Set both x,y to same value.
    constexpr void		set(const T Val)		///< [in] Both x, y are set to this value
    {
        m_d[X] = Val; m_d[Y] = Val;
    }

    /// Sets the 2 values of a vector.
    constexpr void		set(const T fx,			///< [in] New x value.
                    const T fy)			///< [in] New y value.
    {
        m_d[X] = fx; m_d[Y] = fy;
    }


    // ----------------------------
    // --- Assignment operators ---
    // ----------------------------
    /** '=' operator, Assignment from other vector.
        \return Reference to "this".*/
    constexpr reference			operator =(const_reference v)	///< [in] Right hand side operand.
    {
        m_d[X] = v.x();
        m_d[Y] = v.y();
        return *this;
    }

    // ----------------------
    // --- Math operators ---
    // ----------------------

    /** Unary '-' operator, Negate vector.
        \return Vector holding the result of the negation. */
    constexpr value_type		operator -() const
    {
        return value_type(-x(), -y());
    }

    /** '+=' operator, Add vector to this.
        \return Reference to "this".*/
    constexpr reference			operator +=(const_reference v)	///< [in] Right hand side operand.
    {
        m_d[X] += v.x();
        m_d[Y] += v.y();
        return *this;
    }

    /** '-=' operator, Substract vector from this.
        \return Reference to "this".*/
    constexpr reference			operator -=(const_reference v)	///< [in] Right hand side operand.
    {
        m_d[X] -= v.x();
        m_d[Y] -= v.y();
        return *this;
    }

    /** '*=' operator, Scales the vector with a scalar. (I.e. multiplying the
        elements with a scalar.)
        \return Reference to "this".*/
    constexpr reference			operator *=(T Val)		///< [in] Right hand side operand. Scalar used to scale the vector.
    {
        m_d[X] *= Val;
        m_d[Y] *= Val;
        return *this;
    }

    /** '/=' operator, Divides the elements of the vector with a scalar.
        \return Reference to "this".*/
    constexpr reference			operator /=(T Val)		///< [in] Right hand side operand. Dividend used to scale the vector.
    {
        T fV = T(1) / Val;
        m_d[X] *= fV;
        m_d[Y] *= fV;
        return *this;
    }

    /** '-' operator, Substract two vectors
        \return Vector holding the result of the substraction. */
    constexpr value_type		operator -(const_reference v		///< [in] Right hand side operand.
                              ) const
    {
        return value_type(x() - v.x(), y() - v.y());
    }

    /** '+' operator, Add two vectors
        \return Vector holding the result of the addition. */
    constexpr value_type		operator +(	const_reference v		///< [in] Right hand side operand.
                              ) const
    {
        return value_type(x() + v.x(), y() + v.y());
    }

    /** '*' operator, Element wise multiplication of two vectors.
     *  @sa dot() Dot product
        \return Vector which is the two vectors multiplied element wise. */
    template <typename T1>
    value_type	operator *(v2<T1> v		///< [in] Right hand side operand.
                ) const
    {
        const auto x1 = x()*static_cast<T>(v.x());
        const auto y1 = y()*static_cast<T>(v.y());
        return value_type(x1, y1);
    }

    /** '/' operator, Element wise division of two vectors.
        \return Vector which is the two vectors divided element wise. */
    template <typename T1>
    constexpr value_type	operator /(v2<T1> v		///< [in] Right hand side operand.
                         ) const
    {
        const auto x1 = x()/static_cast<T>(v.x());
        const auto y1 = y()/static_cast<T>(v.y());
        return value_type(x1, y1);
    }

    /** Dot product of two vectors.
        \return Scalar (number) holding the result of the dot product. */
    constexpr T				dot(const_reference v		///< [in] Right hand side operand.
                        ) const
    {
        return ( x()*v.x() + y()*v.y());
    }

    /** '*' operator, Scales the vector with a scalar
        \return Vector holding the result of the scaling. */
    constexpr value_type		operator *(T Val			///< [in] Right Hand Side operand (scalar) to scale with.
                              ) const
    {
        return value_type( x()*Val, y()*Val );
    }

    /** Sets the x-value of the vector and scales the y value uniformly in accordance.
        \return Vector holding the result of the scaling. */
    constexpr value_type		uniform_scale_x (T x_value			///< [in] Set this x value and scale uniformly accordingly.
                                                ) const
    {
        const double scale_fac = static_cast<double>(x_value) / static_cast<double>(x());
        const double yf = static_cast<double>(y()) * scale_fac;
        return value_type(x_value, static_cast<T>(yf) );
    }

    /** Sets the y-value of the vector and scales the x value uniformly in accordance.
        \return Vector holding the result of the scaling. */
    constexpr value_type		uniform_scale_y (T y_value			///< [in] Set this y value and scale uniformly accordingly.
                                                ) const
    {
        const double scale_fac = static_cast<double>(y_value) / static_cast<double>(y());
        const double xf = static_cast<double>(x()) * scale_fac;
        return value_type(static_cast<T>(xf), y_value);
    }

    /** '/' operator, Divides the vector with a scalar
        \return Vector holding the result (each element divided by Val). */
    constexpr value_type		operator /(T Val			///< [in] Right Hand Side operand (scalar) to divide with.
                              ) const
    {
        T fV = T(1) / Val;
        return value_type( x()*fV, y()*fV );
    }

    // -----------------------------
    // --- Member Math functions ---
    // -----------------------------
    /** Aspect ratio.
        \return Aspect ratio width() / height(). */
    template <typename T1>
    constexpr T1	aspect_ratio() const
    {
        const auto w1 = static_cast<T1>(width());
        const auto h1 = static_cast<T1>(height());
        return w1/h1;
    }

    /** Length of vector (squared).
        \return Eucludian (XX how to spell) length (squared) of the vector. */
    constexpr T			len2() const
    {
        return (x()*x() + y()*y());
    }


    /** Length of vector.
        \return Eucludian (XX how to spell) length of the vector. */
    constexpr T			len() const
    {
        return static_cast<T>(Sqrt(len2()));
    }

    /** Distance to point (squared). Calculates the distance (squared) from the
        point in 2D space represented by this vector to another. This version is
        generally faster than dist() and is recommended whenever it's feasible.
        \return The distance from this point to the input point. */
    constexpr T			dist2(const_reference v		///< [in] Vector representing the other point in 2D space.
                     ) const
    {
        return (*this - v).len2();
    }

    /** Distance to point. Calculates the distance from the point in 2D space
        represented by this vector to another.
        \return The distance from this point to the input point. */
    constexpr T			dist(const_reference v		///< [in] Vector representing the other point in 2D space.
                    ) const
    {
        return (*this - v).len();
    }


    /** Angle to other vector. Calculates the angle of this vector with another.
        \return Angle to input vector in radians. */
    constexpr T			angle(const_reference v		///< [in] Vector to calc. angle to.
                     ) const
    {
        return Acos((*this)*v / ( len()*v.len() ) );
    }


    // ----------------------
    // --- Util functions ---
    // ----------------------
    /** Fills vector with zeroes. */
    constexpr void				reset()
    {
        m_d[X] = static_cast<T>(0);
        m_d[Y] = static_cast<T>(0);
    }

    /** Normalizes the vector. Ie. making it length 1.
        \return The original length of the vector. */
    constexpr T				normalize()
    {
        const T fOrgLen = len();
        if (fOrgLen <= constants<T>::EPSILON())
        {
            reset();
            return static_cast<T>(0);
        }
        else
        {
            const T fInvLen = T(1) / fOrgLen;
            m_d[X] *= fInvLen;
            m_d[Y] *= fInvLen;
            return fOrgLen;
        }
    }

    /** Returns Normalized copy of the vector. Ie. with length 1.
        */
    constexpr value_type				normalized() const
    {
        const T fOrgLen = len();
        if (fOrgLen <= constants<T>::epsilon())
        {
            return value_type(T(0),T(0));
        }
        else
        {
            const T fInvLen = T(1) / fOrgLen;
            return value_type (m_d[X]*fInvLen, m_d[Y] * fInvLen);
        }
    }


    /** Sets the length of the vector to the given value.
        \return The original length of the vector. */
    constexpr T				set_len(T fLen)		///< [in] New length
    {
        const T fOrgLen = normalize();
        *this			*= fLen;
        return fOrgLen;
    }



//    /** Pulls a vector towards a destination vector. But only a fixed angle
//        at a time. Returns true when the destination vector is reached
//        otherwise false.
//        \return True if vector has reached destination vector.
//        \note NOT_IMPLEMENTED YET XXX. */
//    bool_t				pull(const_reference vDest,	///< [in] Destination vector we are pulling towards.
//                             T fMaxMoveAng)			///< [in] Maximum angle to move the vector.
//    {
//        STATIC_CHECK(0, v2_pull_NOT_IMPLEMENTED_YET);
//        return true;
//    }


    // -------------------------
    // --- Compare operators ---
    // -------------------------

    /** '==' operator, Equality test.
        constants<T>::epsilon() is used as threshold to avoid numeric instabilities.
        Use	the 'equal_exact' member function if exact comparision is required.
        \return True if all corresponding elements of the two vectors is no further,
        than constants<T>::epsilon() apart (absolute value), false otherwise. */
    constexpr bool_t	operator ==(const_reference v		///< [in] Right operand.
                        ) const
    {
        return	( Abs(x() - v.x()) < constants<T>::epsilon() ) &&
                ( Abs(y() - v.y()) < constants<T>::epsilon() );
    }

    /** '!=' operator, In-equality test.
        constants<T>::epsilon() is used as threshold to avoid numeric instabilities.
        Use	the 'equal_exact' member function if exact comparision is required.
        \return True if just one corresponding pair of elements of the two
        vectors is further than constants<T>::epsilon() apart (absolute value),
        false otherwise. */
    constexpr bool_t	operator !=(const_reference v		///< [in] Right operand.
                        ) const
    {

        return	( Abs(x() - v.x()) > constants<T>::epsilon() ) ||
                ( Abs(y() - v.y()) > constants<T>::epsilon() );
    }

    /** '<' operator, Length comparision. Returns true if the length of the left hand
        side vector is less than the length of the right hand side vector. Uses the
        square of the length of the two vectors for speed optimization reasons. No
        epsilon value is used here, but see operator '<='.
        \return True if lenght of LHS is less than the length of RHS, false otherwise.*/
    constexpr bool_t	operator <(const_reference v		///< [in] Right operand.
                      ) const
    {
        return len2() < v.len2();
    }

    /** '>' operator, Length comparision. Returns true if the length of the left hand
        side vector is greater than the length of the right hand side vector. Uses
        the square of the length of the two vectors for speed optimization reasons. No
        epsilon value is used here, but see operator '>='.
        \return True if lenght of LHS is greater than the length of RHS, false otherwise.*/
   constexpr  bool_t	operator >(const_reference v		///< [in] Right operand.
                      ) const
    {
        return len2() > v.len2();
    }

    /** '<=' operator, Length comparision. Returns true if the length of the left hand side vector
        is less than or equal to the length of the right hand side vector. Uses
        the square of the length of the two vectors for speed optimization reasons.
        constants<T>::epsilon() is used as threshold to avoid numeric instabilities.
        See also operator '<' for exact numeric comparision without epsilon value.
        \return Truth value of: ( v0.len2() - v1.len2() ) <= constants<T>::epsilon().*/
    constexpr bool_t	operator <=(const_reference v		///< [in] Right operand.
                        ) const
    {
        return ( len2() - v.len2() ) <= constants<T>::epsilon();
    }


    /** '>=' operator, Length comparision. Returns true if the length of the left hand side vector
        is greater than or equal to the length of the right hand side vector. Uses
        the square of the length of the two vectors for speed optimization reasons.
        constants<T>::epsilon() is used as threshold to avoid numeric instabilities.
        See also operator '>' for exact numeric comparision without epsilon value.
        \return Truth value of: ( v0.len2() - v1.len2() ) >= -constants<T>::epsilon().*/
    constexpr bool_t	operator >=(const_reference v		///< [in] Right operand.
                        ) const
    {
        return ( len2() - v.len2() ) >= -constants<T>::epsilon();
    }


    // -------------------------
    // --- Compare functions ---
    // -------------------------

    /** Equal with exact comparision. Tests two vectors for equality.
        No epsilon value is used. Use with care when comparing floating point
        type values with this function.
        \return True if all corresponding elements of the two vectors are exactly
        equal, false otherwise. */
    constexpr bool_t		equal_exact(const_reference v		///< [in] Vector to compare with
                           )const
    {
        return (x() == v.x()) && (y() == v.y());
    }

    /** Equal with custom epsilon comparision. Tests two vectors for equality.
        Supplied epsilon value is used.
        \return True if all corresponding elements of the two vectors is no further,
        than epsilon apart (absolute value), false otherwise.. */
    constexpr bool_t		equal_epsilon(const_reference v,	///< [in] Vector to compare with
                              const T epsilon		///< [in] Epsilon value to use when comparing
                             )const
    {
        return	( Abs(x() - v.x()) <= epsilon ) &&
                ( Abs(y() - v.y()) <= epsilon );
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    // -----------------------
    // --- Debug Functions ---
    // -----------------------

    /// Convert vector to a string for debug print.
    std::string				str( int_t iDecimals= -1	///< [in] Number of decimals
                               ) const
    {
        char sz[30];
        const float_t fx = (float_t)x();
        const float_t fy = (float_t)y();
        switch ( iDecimals )
        {
            case 0	: sprintf(sz, "[%.0f, %.0f]", fx, fy); break;
            case 1	: sprintf(sz, "[%.1f, %.1f]", fx, fy); break;
            case 2	: sprintf(sz, "[%.2f, %.2f]", fx, fy); break;
            case 3	: sprintf(sz, "[%.3f, %.3f]", fx, fy); break;
            case 4	: sprintf(sz, "[%.4f, %.4f]", fx, fy); break;
            case 5	: sprintf(sz, "[%.5f, %.5f]", fx, fy); break;
            case 6	: sprintf(sz, "[%.6f, %.6f]", fx, fy); break;
            default	: sprintf(sz, "[%f, %f]", fx, fy);
        }
        return std::string(sz);
    }


    // -------------------
    // --- Member Data ---
    // -------------------

private:
    T	m_d[2];
};


// ----------------------------------------------
// --- Initialization Of Static Const Members ---
// ----------------------------------------------

template <typename T>
const v2<T> v2<T>::V_ZERO	=  v2<T>(T(0), T(0));

template <typename T>
const v2<T> v2<T>::V_X		=  v2<T>(T(1), T(0));

template <typename T>
const v2<T> v2<T>::V_Y		=  v2<T>(T(0), T(1));

// -----------------
// --- Operators ---
// -----------------

/** '*' operator, Scales the vector with a scalar.
    \return */
template <typename T>
constexpr inline v2<T>	operator *(	T Val, 			///< [in] Left Hand Side operand (scalar).
                            const v2<T>& v)		///< [in] Right hand side operand (v2).
{
    return v*Val;	// Implement using operator v2<T>*T
}


// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write vector to stream.
    \return Reference to (modified) ostream. */
template <typename T>
inline std::ostream&	operator <<(std::ostream& os,		///< [out] Output stream to write to.
                                    const v2<T>& v)			///< [in] Vector to write.
{
    os << "[" << v.x() << ", " << v.y() << "]";
    return os;
}

/** '>>' operator, Read input from stream.
@todo math::v4 Handle commas and brackets when reading
@see https://codereview.stackexchange.com/questions/216343/overloading-istream-to-read-comma-separated-input
@return Reference to (modified) istream.
*/
template <typename T>
inline std::istream&		operator >>(std::istream& is,		///< [in]  Input stream to read from.
                                        v2<T>& v)				///< [out] Vector recieve the data from the stream.
{
    is >> v.x() >> v.y();
    return is;
}

template <typename T>
inline std::string to_string(const v2<T>& v)
{
    return v.to_string();
}

// --- Specializations of selected functions ---


/// The general version of this would not work with integers.
template<>
constexpr inline v2<int_t>&	v2<int_t>::operator/=(int_t Val)
{
    m_d[X] /= Val;
    m_d[Y] /= Val;
    return *this;
}

/// The general version of this would not work with integers.
template<>
constexpr inline v2<int_t>	v2<int_t>::operator/(int_t Val) const
{
    return v2<int_t>( x() / Val, y() / Val );
}

using v2f = v2<float>;
using v2d = v2<double>;
using v2i32 = v2<std::int32_t>;
using v2i64 = v2<std::int64_t>;


}
