#pragma once
// ****************************************************************************
// CREATED_ON	: 17-06-03 17:00:19
// CREATED_BY	: Martin Lutken
// ---------------------------------------------------------------------------- 
// COPYRIGHT	: 
// ****************************************************************************

// --- Include files ---
#include <iostream>
#include <string>
#include <cstdint>

#include <math/base/lue_math.h>
#include "stdmath.hpp"
#include <math/base/lue_math_internal_utils.hpp>


namespace lue::math {


template <typename T> class m44;

/** 
4D vector class. Works together with matrix (m44).  The parameter type 'T' is 
used as the precision type (typically float or double would be used).

The functions and operator overloads that returns a v4 vector as a value 
are written so that they construct and return the v4 in a single line statement 
to ease the compiler's possibilities to do "return value optimization". */
template <typename T>
class v4
{
    friend class m44<T>;

public:	
	// ----------------
	// --- Typedefs ---
	// ----------------
	typedef v4<T>				value_type;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef size_t		index_type;

    struct raw_vec_t
    {
        T	x_, y_, z_, w_;
    };

	// ----------------------------
	// --- Const Static Members ---
	// ----------------------------

	static const value_type V_ZERO;		///< Constant zero vector.
	static const value_type V_X;		///< Constant unit vector in the X-direction.
	static const value_type V_Y;		///< Constant unit vector in the Y-direction.
	static const value_type V_Z;		///< Constant unit vector in the Z-direction.
	static const value_type V_W;		///< Constant unit vector in the W-direction.

	
public:

	// --------------------
	// --- Constructors ---
	// --------------------

    /** Default constructor with no initialization. */
    constexpr v4() = default;

    constexpr v4 (const v4& other) = default;

	/** Copy constructor. Initializtion with general (v4<>) vector.
		Also work as casting operator from general v4<>.*/
	template <typename T1>
    constexpr v4(const v4<T1>& v)
	:	m_x(static_cast<T>(v.x())), m_y(static_cast<T>(v.y())), 
		m_z(static_cast<T>(v.z())), m_w(static_cast<T>(v.w())) {}

	/** Constructor with single value initialization. */
    constexpr explicit v4(const T fVal)						///< [in] Both x, y, z, w are set to this value
		: m_x(fVal), m_y(fVal), m_z(fVal), m_w(fVal) {}	

	/** Constructor with 4 value initialization. */
    constexpr v4(	const T fx,									///< [in] x value.
		const T fy, 								///< [in] y value.
		const T fz, 								///< [in] z value.
		const T fw) 								///< [in] w value.
		: m_x(fx), m_y(fy), m_z(fz), m_w(fw) {}	


	// -----------------------
	// --- Access elements ---
	// -----------------------

	/** Reference to x-coordinate.
		\return Reference to x-coordinate of vector. */
    constexpr T&			x()			{return m_x;}
    constexpr const T&      x() const	{return m_x;}

	/** Reference to y-coordinate.
		\return Reference to y-coordinate of vector. */
    constexpr T&			y()			{return m_y;}
    constexpr const T&      y() const	{return m_y;}

	/** Reference to z-coordinate.
		\return Reference to z-coordinate of vector. */
    constexpr T&			z()			{return m_z;}
    constexpr const T&      z() const	{return m_z;}

	/** Reference to w-coordinate.
		\return Reference to w-coordinate of vector. */
    constexpr T&			w()			{return m_w;}
    constexpr const T&      w() const	{return m_w;}


    /** Set x-coordinate. */
    void		x(T x)          {d[X] = x;}
    /** Set y-coordinate. */
    void		y(T y)          {d[Y] = y;}
    /** Set z-coordinate. */
    void		z(T z)          {d[Z] = z;}
    /** Set w-coordinate. */
    void		w(T w)          {d[W] = w;}


	/** [] Operator, Acces individual elements of a vector.
		\return Reference to indexed element (coordinate) of vector. */
	T&					operator[](const index_type i)				///< [in] Index value. Should be in range 0..3 ~ X,Y,Z,W.
	{ return d[i]; }
	const T&			operator[](const index_type i) const
	{ return d[i]; }


	// ------------------------------------
	// --- Simple Set And Get Functions ---
	// ------------------------------------

	/// Set all elements to same value.
	void		set(const T fVal)		///< [in] All x, y, z, w are set to this value
	{
		m_x = fVal; m_y = fVal; m_z = fVal; m_w = fVal;
	}

	/// Sets the 4 values of a vector.
	void		set(const T fx,			///< [in] New x value.
					const T fy,			///< [in] New y value.
					const T fz,			///< [in] New z value.
					const T fw)			///< [in] New w value.
	{
		m_x = fx; m_y = fy; m_z = fz;  m_w = fw;
	}


	// ----------------------------
	// --- Assignment operators ---
	// ----------------------------
	/** '=' operator, Assignment from other vector.
		\return Reference to "this".*/
	reference			operator =(const_reference v)	///< [in] Right hand side operand.
	{
		m_x = v.x(); 
		m_y = v.y(); 
		m_z = v.z(); 
		m_w = v.w(); 
		return *this;
	}

	// ----------------------
	// --- Math operators ---
	// ----------------------

	/** Unary '-' operator, Negate vector.
		\return Vector holding the result of the negation. */
	value_type		operator -() const
	{
		return value_type(-x(), -y(), -z(), -w());
	}

	/** '+=' operator, Add vector to this.
		\return Reference to "this".*/
	reference			operator +=(const_reference v)	///< [in] Right hand side operand.
	{
		m_x += v.x(); 
		m_y += v.y(); 
		m_z += v.z(); 
		m_w += v.w(); 
		return *this;
	}

	/** '-=' operator, Substract vector from this.
		\return Reference to "this".*/
	reference			operator -=(const_reference v)	///< [in] Right hand side operand.
	{
		m_x -= v.x();
		m_y -= v.y();
		m_z -= v.z();
		m_w -= v.w();
		return *this;
	}

	/** '*=' operator, Scales the vector with a scalar. (I.e. multiplying the 
		elements with a scalar.) 
		\return Reference to "this".*/
	reference			operator *=(const T fVal)		///< [in] Right hand side operand. Scalar used to scale the vector.
	{
		m_x *= fVal;
		m_y *= fVal;
		m_z *= fVal;
		m_w *= fVal;
		return *this;
	}

	/** '/=' operator, Divides the elements of the vector with a scalar. 
		\return Reference to "this".*/
	reference			operator /=(const T fVal)		///< [in] Right hand side operand. Dividend used to scale the vector.
	{
		T fV = T(1) / fVal;
		m_x *= fV;
		m_y *= fV;
		m_z *= fV;
		m_w *= fV;
		return *this;
	}

	/** '-' operator, Substract two vectors 
		\return Vector holding the result of the substraction. */
	value_type		operator -(const_reference v		///< [in] Right hand side operand.
							  ) const
	{
		return value_type(x() - v.x(), y() - v.y(), z() - v.z(), w() - v.w());
	}

	/** '+' operator, Add two vectors 
		\return Vector holding the result of the addition. */
	value_type		operator +(	const_reference v		///< [in] Right hand side operand.
							  ) const
	{
		return value_type(x() + v.x(), y() + v.y(), z() + v.z(), w() + v.w());
	}

	/** '*' operator, Dot product of two vectors.
		\return Scalar (number) holding the result of the dot product. */
	T				operator *(const_reference v		///< [in] Right hand side operand.
							  ) const
	{
		return ( x()*v.x() + y()*v.y() + z()*v.z() + w()*v.w());
	}

	/** '*' operator, Scales the vector with a scalar  
		\return Vector holding the result of the scaling. */
	value_type		operator *(const T fVal			///< [in] Right Hand Side operand (scalar) to scale with.
							  ) const
	{
		return value_type( x()*fVal, y()*fVal, z()*fVal, w()*fVal );
	}

//	/** Calculates the cross product of two vectors returning the result as a
//		value (v4). */
//	value_type	cross(const_reference v			///< [in] Right hand side operand.
//					 ) const
//	{
//		static_assert(false, "v4::cross() NOT_IMPLEMENTED_YET");
//	}
	

	// -----------------------------
	// --- Member Math functions ---
	// -----------------------------

	/** Length of vector (squared). 
		\return Eucludian (XX how to spell) length (squared) of the vector. */
	T			len2() const
	{
		return (x()*x() + y()*y() + z()*z());
	}

	/** Length of vector. 
		\return Eucludian (XX how to spell) length of the vector. */
	T			len() const
	{
		return static_cast<T>(Sqrt(len2()));
	}

	/** Distance to point (squared). Calculates the distance (squared) from the 
		point in 4D space represented by this vector to another. This version is 
		generally faster than dist() and is recommended whenever it's feasible.
		\return The distance from this point to the input point. */
	T			dist2(const_reference v		///< [in] Vector representing the other point in 4D space.
					 ) const
	{
		return (*this - v).len2();
	}

	/** Distance to point. Calculates the distance from the point in 4D space 
		represented by this vector to another. 
		\return The distance from this point to the input point. */
	T			dist(const_reference v		///< [in] Vector representing the other point in 4D space.
					) const
	{
		return (*this - v).len();
	}


	/** Angle to other vector. Calculates the angle of this vector with another.
		\return Angle to input vector in radians. */
	T			angle(const_reference v		///< [in] Vector to calc. angle to.
					 ) const
	{
		return Acos((*this)*v / ( len()*v.len() ) );
	}


	// ----------------------
	// --- Util functions ---
	// ----------------------
	/** Fills vector with zeroes. */
	void				reset()
	{
		m_x = static_cast<T>(0); 
		m_y = static_cast<T>(0);
		m_z = static_cast<T>(0);
		m_w = static_cast<T>(0);
	}

	/** Normalizes the vector. Ie. making it length 1.
		\return The original length of the vector. */
	T				normalize()
	{
		const T fOrgLen = len();	
		if (fOrgLen <= constants<T>::epsilon())
		{
			reset();
			return static_cast<T>(0);
		}
		else
		{
			const T fInvLen = T(1) / fOrgLen;
			m_x *= fInvLen; 
			m_y *= fInvLen;
			m_z *= fInvLen;
			m_w *= fInvLen;
			return fOrgLen;
		}
	}

	/** Sets the length of the vector to the given value. 
		\return The original length of the vector. */
	T				set_len(const T fLen)		///< [in] New length
	{
		const T fOrgLen = normalize();
		*this			*= fLen;
		return fOrgLen;
	}

    /** Linear interpolation towards a destination vector.
        \return Interpolated vector.
        \note NOT_IMPLEMENTED_YET.*/
    value_type	lerp(const_reference max,       ///< [in] Destination vector we are pulling towards.
                     T interpolation_fraction)	///< [in] Maximum angle to move the vector.
    {
        return value_type(lerp(x(), max.x(),interpolation_fraction),
                          lerp(y(), max.y(),interpolation_fraction),
                          lerp(z(), max.z(),interpolation_fraction),
                          lerp(w(), max.w(),interpolation_fraction));
    }

	/** Pulls a vector towards a destination vector. But only a fixed angle 
		at a time. Returns true when the destination vector is reached 
		otherwise false. 
		\return True if vector has reached destination vector.
		\note NOT_IMPLEMENTED_YET.*/
    bool				pull_rotate(const_reference vDest,	///< [in] Destination vector we are pulling towards.
                                    const T fMaxMoveAng)	///< [in] Maximum angle to move the vector.
	{
        static_assert(0, "v4_pull_NOT_IMPLEMENTED_YET");
		return true;
	}


	// -------------------------
	// --- Compare operators ---
	// -------------------------

	/** '==' operator, Equality test. 
		constants<T>::epsilon() is used as threshold to avoid numeric instabilities. 
		Use	the 'equal_exact' member function if exact comparision is required. 
		\return True if all corresponding elements of the two vectors is no further,
		than constants<T>::epsilon() apart (absolute value), false otherwise. */
	bool	operator ==(const_reference v		///< [in] Right operand.    
						) const
	{
		return	( Abs(x() - v.x()) < constants<T>::epsilon() ) && 
				( Abs(y() - v.y()) < constants<T>::epsilon() ) &&
				( Abs(z() - v.z()) < constants<T>::epsilon() ) &&
				( Abs(w() - v.w()) < constants<T>::epsilon() );
	}

	/** '!=' operator, In-equality test. 
		constants<T>::epsilon() is used as threshold to avoid numeric instabilities. 
		Use	the 'equal_exact' member function if exact comparision is required. 
		\return True if just one corresponding pair of elements of the two 
		vectors is further than constants<T>::epsilon() apart (absolute value), 
		false otherwise. */
	bool	operator !=(const_reference v		///< [in] Right operand.    
						) const
	{

		return	( Abs(x() - v.x()) > constants<T>::epsilon() ) || 
				( Abs(y() - v.y()) > constants<T>::epsilon() ) ||
				( Abs(z() - v.z()) > constants<T>::epsilon() ) ||
				( Abs(w() - v.w()) > constants<T>::epsilon() );
	}

	/** '<' operator, Length comparision. Returns true if the length of the left hand 
		side vector is less than the length of the right hand side vector. Uses the 
		square of the length of the two vectors for speed optimization reasons. No 
		epsilon value is used here, but see operator '<='.
		\return True if lenght of LHS is less than the length of RHS, false otherwise.*/
	bool	operator <(const_reference v		///< [in] Right operand.    
					  ) const
	{	
		return len2() < v.len2();
	}

	/** '>' operator, Length comparision. Returns true if the length of the left hand 
		side vector is greater than the length of the right hand side vector. Uses 
		the square of the length of the two vectors for speed optimization reasons. No 
		epsilon value is used here, but see operator '>='.
		\return True if lenght of LHS is greater than the length of RHS, false otherwise.*/
	bool	operator >(const_reference v		///< [in] Right operand.    
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
	bool	operator <=(const_reference v		///< [in] Right operand.    
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
	bool	operator >=(const_reference v		///< [in] Right operand.    
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
	bool		equal_exact(const_reference v		///< [in] Vector to compare with
						   )const
	{
		return (x() == v.x()) && (y() == v.y()) && (z() == v.z()) && (w() == v.w());
	}

	/** Equal with custom epsilon comparision. Tests two vectors for equality.
		Supplied epsilon value is used. 
		\return True if all corresponding elements of the two vectors is no further,
		than epsilon apart (absolute value), false otherwise.. */
	bool		equal_epsilon(const_reference v,	///< [in] Vector to compare with
							  const T epsilon		///< [in] Epsilon value to use when comparing
							 )const
	{
		return	( Abs(x() - v.x()) <= epsilon ) && 
				( Abs(y() - v.y()) <= epsilon ) &&
				( Abs(z() - v.z()) <= epsilon ) &&
				( Abs(w() - v.w()) <= epsilon );
	}

    operator const  raw_vec_t&   () const   { return d; }
    operator        raw_vec_t&   ()         { return d; }

	// -----------------------
	// --- Debug Functions ---
	// -----------------------

	/// Convert vector to a string for debug print.
	std::string				str(const int iDecimals= -1	///< [in] Number of decimals
							   ) const
	{
		char sz[50];
		const float_t fx = (float_t)x();
		const float_t fy = (float_t)y();
		const float_t fz = (float_t)z();
		const float_t fw = (float_t)w();
		switch ( iDecimals )
		{
			case 0	: sprintf(sz, "[%.0f, %.0f, %.0f, %.0f]", fx, fy, fz, fw); break;
			case 1	: sprintf(sz, "[%.1f, %.1f, %.1f, %.1f]", fx, fy, fz, fw); break;
			case 2	: sprintf(sz, "[%.2f, %.2f, %.2f, %.2f]", fx, fy, fz, fw); break;
			case 3	: sprintf(sz, "[%.3f, %.3f, %.3f, %.3f]", fx, fy, fz, fw); break;
			case 4	: sprintf(sz, "[%.4f, %.4f, %.4f, %.4f]", fx, fy, fz, fw); break;
			case 5	: sprintf(sz, "[%.5f, %.5f, %.5f, %.5f]", fx, fy, fz, fw); break;
			case 6	: sprintf(sz, "[%.6f, %.6f, %.6f, %.6f]", fx, fy, fz, fw); break;
			default	: sprintf(sz, "[%f, %f, %f, %f]", fx, fy, fz, fw);
		}
		return std::string(sz);
	}
	

	// -------------------
	// --- Member Data ---
	// -------------------

private:
	union 
	{
		struct 
		{
			T	m_x, m_y, m_z, m_w;
		};
		T	d[4];
	};

};


// ----------------------------------------------
// --- Initialization Of Static Const Members ---
// ----------------------------------------------

template <class T>
const v4<T> v4<T>::V_ZERO	=  v4<T>(T(0), T(0), T(0), T(0));

template <class T>
const v4<T> v4<T>::V_X		=  v4<T>(T(1), T(0), T(0), T(0));

template <class T>
const v4<T> v4<T>::V_Y		=  v4<T>(T(0), T(1), T(0), T(0));

template <class T>
const v4<T> v4<T>::V_Z		=  v4<T>(T(0), T(0), T(1), T(0));

template <class T>
const v4<T> v4<T>::V_W		=  v4<T>(T(0), T(0), T(0), T(1));

// -----------------
// --- Operators ---
// -----------------

/** '*' operator, Scales the vector with a scalar. 
	\return */
template <typename T>
inline v4<T>	operator *(	const T fVal, 		///< [in] Left Hand Side operand (scalar).
							const v4<T>& v)		///< [in] Right hand side operand (v4). 
{
	return v*fVal;	// Implement using operator v4<T>*T
}


// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write vector to stream. 
	\return Reference to (modified) ostream. */
template <typename T>
inline std::ostream&	operator <<(std::ostream& os,		///< [out] Output stream to write to.
									const v4<T>& v)			///< [in] Vector to write.
{
    os << "[" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() <<  "]";
    return os;
}

/** '>>' operator, Read input from stream.
@todo math::v4 Handle commas and brackets when reading
@see https://codereview.stackexchange.com/questions/216343/overloading-istream-to-read-comma-separated-input
@return Reference to (modified) istream.
*/
template <typename T>
inline std::istream&		operator >>(std::istream& is,		///< [in]  Input stream to read from.	
										v4<T>& v)				///< [out] Vector recieve the data from the stream.
{
    is >> v.x() >> v.y() >> v.z() >> v.w();
    return is;
}

using v4f = v4<float>;
using v4d = v4<double>;
using v4i32 = v4<std::int32_t>;
using v4i64 = v4<std::int64_t>;

}



