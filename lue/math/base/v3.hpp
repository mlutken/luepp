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

#include "lue_math.h"
#include "stdmath.hpp"
#include "lue_math_internal_utils.hpp"

namespace lue::math {


template <typename T> class m33;

/** 
3D vector class. Works together with matrix (m33).  The parameter type 'T' is 
used as the precision type (typically float or double would be used).

The functions and operator overloads that returns a v3 vector as a value 
are written so that they construct and return the v3 in a single line statement 
to ease the compiler's possibilities to do "return value optimization". */
template <typename T>
class v3
{
friend class m33<T>;

public:	
	// ----------------
	// --- Typedefs ---
	// ----------------
	typedef v3<T>				value_type;
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
	static const value_type V_Z;		///< Constant unit vector in the Z-direction.

	
public:

	// --------------------
	// --- Constructors ---
	// --------------------

	/** Default constructor with no initialization. */
	v3() {}

	/** Copy constructor. Initializtion with general (v3<>) vector.
		Also work as casting operator from general v3<>.*/
	template <typename T1>
	v3(const v3<T1>& v)	
//	: m_d[X](static_cast<T>(v.x())), m_d[Y](static_cast<T>(v.y())), m_d[Z](static_cast<T>(v.z())) {}
	{
		m_d[X] = static_cast<T>(v.x()); 
		m_d[Y] = static_cast<T>(v.y());
		m_d[Z] = static_cast<T>(v.z());
	}
	/** Constructor with single value initialization. */
	explicit v3(const T fVal)						///< [in] Both x, y, z are set to this value
//		: m_d[X](fVal), m_d[Y](fVal), m_d[Z](fVal) {}	
	{
		m_d[X] = fVal; m_d[Y] = fVal; m_d[Z] = fVal;
	}

	/** Constructor with 3 value initialization. */
	v3(	const T fx,									///< [in] x value.
		const T fy, 								///< [in] y value.
		const T fz) 								///< [in] z value.
//		: m_d[X](fx), m_d[Y](fy), m_d[Z](fz){}	
	{
		m_d[X] = fx; m_d[Y] = fy; m_d[Z] = fz;
	}


	// -----------------------
	// --- Access elements ---
	// -----------------------

	/** Reference to x-coordinate.
		\return Reference to x-coordinate of vector. */
	T&			x()			{return m_d[X];}
	const T&	x() const	{return m_d[X];}

	/** Reference to y-coordinate.
		\return Reference to y-coordinate of vector. */
	T&			y()			{return m_d[Y];}
	const T&	y() const	{return m_d[Y];}

	/** Reference to z-coordinate.
		\return Reference to z-coordinate of vector. */
	T&			z()			{return m_d[Z];}
	const T&	z() const	{return m_d[Z];}

	/** Set x-coordinate. */
	void		x(T x)		{m_d[X] = x;}
	/** Set y-coordinate. */
	void		y(T y)		{m_d[Y] = y;}
	/** Set z-coordinate. */
	void		z(T z)		{m_d[Z] = z;}


	/** [] Operator, Acces individual elements of a vector.
		\return Reference to indexed element (coordinate) of vector. */
	T&					operator[](const index_type i)				///< [in] Index value. Should be in range 0..2 ~ X,Y,Z.
	{ return m_d[i]; }
	const T&			operator[](const index_type i) const
	{ return m_d[i]; }


	// ------------------------------------
	// --- Simple Set And Get Functions ---
	// ------------------------------------

	/// Set all elements to same value.
	void		set(const T fVal)		///< [in] Both x, y, z are set to this value
	{
		m_d[X] = fVal; m_d[Y] = fVal; m_d[Z] = fVal;
	}

	/// Sets the 3 values of a vector.
	void		set(const T fx,			///< [in] New x value.
					const T fy,			///< [in] New y value.
					const T fz)			///< [in] New z value.
	{
		m_d[X] = fx; m_d[Y] = fy; m_d[Z] = fz;
	}


	// ----------------------------
	// --- Assignment operators ---
	// ----------------------------
	/** '=' operator, Assignment from other vector.
		\return Reference to "this".*/
	reference			operator =(const_reference v)	///< [in] Right hand side operand.
	{
		m_d[X] = v.x(); 
		m_d[Y] = v.y(); 
		m_d[Z] = v.z(); 
		return *this;
	}

	// ----------------------
	// --- Math operators ---
	// ----------------------

	/** Unary '-' operator, Negate vector.
		\return Vector holding the result of the negation. */
	value_type		operator -() const
	{
		return value_type(-x(), -y(), -z());
	}

	/** '+=' operator, Add vector to this.
		\return Reference to "this".*/
	reference			operator +=(const_reference v)	///< [in] Right hand side operand.
	{
		m_d[X] += v.x(); 
		m_d[Y] += v.y(); 
		m_d[Z] += v.z(); 
		return *this;
	}

	/** '-=' operator, Substract vector from this.
		\return Reference to "this".*/
	reference			operator -=(const_reference v)	///< [in] Right hand side operand.
	{
		m_d[X] -= v.x();
		m_d[Y] -= v.y();
		m_d[Z] -= v.z();
		return *this;
	}

	/** '*=' operator, Scales the vector with a scalar. (I.e. multiplying the 
		elements with a scalar.) 
		\return Reference to "this".*/
	reference			operator *=(const T fVal)		///< [in] Right hand side operand. Scalar used to scale the vector.
	{
		m_d[X] *= fVal;
		m_d[Y] *= fVal;
		m_d[Z] *= fVal;
		return *this;
	}

	/** '/=' operator, Divides the elements of the vector with a scalar. 
		\return Reference to "this".*/
	reference			operator /=(const T fVal)		///< [in] Right hand side operand. Dividend used to scale the vector.
	{
		T fV = T(1) / fVal;
		m_d[X] *= fV;
		m_d[Y] *= fV;
		m_d[Z] *= fV;
		return *this;
	}

	/** '-' operator, Substract two vectors 
		\return Vector holding the result of the substraction. */
	value_type		operator -(const_reference v		///< [in] Right hand side operand.
							  ) const
	{
		return value_type(x() - v.x(), y() - v.y(), z() - v.z());
	}

	/** '+' operator, Add two vectors 
		\return Vector holding the result of the addition. */
	value_type		operator +(	const_reference v		///< [in] Right hand side operand.
							  ) const
	{
		return value_type(x() + v.x(), y() + v.y(), z() + v.z());
	}

    /** '*' operator, Element wise multiplication of two vectors.
     *  @sa dot() Dot product
        \return Vector which is the two vectors multiplied element wise. */
    template <typename T1>
    value_type	operator *(v3<T1> v		///< [in] Right hand side operand.
                         ) const
    {
        const auto x1 = x()*static_cast<T>(v.x());
        const auto y1 = y()*static_cast<T>(v.y());
        const auto z1 = y()*static_cast<T>(v.y());
        return value_type(x1, y1, z1);
    }

    /** '/' operator, Element wise division of two vectors.
        \return Vector which is the two vectors divided element wise. */
    template <typename T1>
    value_type	operator /(v3<T1> v		///< [in] Right hand side operand.
                         ) const
    {
        const auto x1 = x()/static_cast<T>(v.x());
        const auto y1 = y()/static_cast<T>(v.y());
        const auto z1 = y()/static_cast<T>(v.y());
        return value_type(x1, y1, z1);
    }

    /** '*' operator, Scales the vector with a scalar
		\return Vector holding the result of the scaling. */
	value_type		operator *(const T fVal			///< [in] Right Hand Side operand (scalar) to scale with.
							  ) const
	{
		return value_type( x()*fVal, y()*fVal, z()*fVal );
	}

    /** '*' operator, Dot product of two vectors.
        \return Scalar (number) holding the result of the dot product. */
    T	dot(const_reference v		///< [in] Right hand side operand.
          ) const
    {
        return ( x()*v.x() + y()*v.y() + z()*v.z());
    }

	/** Calculates the cross product of two vectors returning the result as a 
		value (v3). */
	value_type	cross(const_reference v			///< [in] Right hand side operand.
					 ) const
	{
		return value_type(	y()*v.z() - v.y()*z(), 
							z()*v.x() - v.z()*x(), 
							x()*v.y() - v.x()*y());
	}
	

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
		point in 3D space represented by this vector to another. This version is 
		generally faster than dist() and is recommended whenever it's feasible.
		\return The distance from this point to the input point. */
	T			dist2(const_reference v		///< [in] Vector representing the other point in 3D space.
					 ) const
	{
		return (*this - v).len2();
	}

	/** Distance to point. Calculates the distance from the point in 3D space 
		represented by this vector to another. 
		\return The distance from this point to the input point. */
	T			dist(const_reference v		///< [in] Vector representing the other point in 3D space.
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
		m_d[X] = static_cast<T>(0); 
		m_d[Y] = static_cast<T>(0);
		m_d[Z] = static_cast<T>(0);
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
			m_d[X] *= fInvLen; 
			m_d[Y] *= fInvLen;
			m_d[Z] *= fInvLen;
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

	/** Pulls a vector towards a destination vector. But only a fixed angle 
		at a time. Returns true when the destination vector is reached 
		otherwise false. 
		\return True if vector has reached destination vector.*/
	bool_t				pull(const_reference vDest,		///< [in] Destination vector we are pulling towards.
							 const T fMaxMoveAng)	///< [in] Maximum angle to move the vector.
	{
		value_type v0(*this), v1(vDest);
		T fLenSrc = v0.normalize();
		T fLenDst = v1.normalize();
		T fDot= v0*v1;
		T fCurAng = Acos(fDot);
		if( fCurAng > fMaxMoveAng)
		{
			value_type vOrtho = v0*fDot;
			vOrtho -= v1;
			vOrtho.normalize();
			vOrtho *= (-Sin(fMaxMoveAng));
			*this = v0 * Cos(fMaxMoveAng);
			*this += vOrtho;
			T t = 0;
			if( fCurAng > constants<T>::epsilon() )
			{
				t = fMaxMoveAng / fCurAng;
			}

			T fCurLen = ((fLenDst - fLenSrc) * t) + fLenSrc;
			*this *= fCurLen;
			return false;
		}
		else
		{
			*this = vDest;
		}
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
	bool_t	operator ==(const_reference v		///< [in] Right operand.    
						) const
	{
		return	( Abs(x() - v.x()) < constants<T>::epsilon() ) && 
				( Abs(y() - v.y()) < constants<T>::epsilon() ) &&
				( Abs(z() - v.z()) < constants<T>::epsilon() );
	}

	/** '!=' operator, In-equality test. 
		constants<T>::epsilon() is used as threshold to avoid numeric instabilities. 
		Use	the 'equal_exact' member function if exact comparision is required. 
		\return True if just one corresponding pair of elements of the two 
		vectors is further than constants<T>::epsilon() apart (absolute value), 
		false otherwise. */
	bool_t	operator !=(const_reference v		///< [in] Right operand.    
						) const
	{

		return	( Abs(x() - v.x()) > constants<T>::epsilon() ) || 
				( Abs(y() - v.y()) > constants<T>::epsilon() ) ||
				( Abs(z() - v.z()) > constants<T>::epsilon() );
	}

	/** '<' operator, Length comparision. Returns true if the length of the left hand 
		side vector is less than the length of the right hand side vector. Uses the 
		square of the length of the two vectors for speed optimization reasons. No 
		epsilon value is used here, but see operator '<='.
		\return True if lenght of LHS is less than the length of RHS, false otherwise.*/
	bool_t	operator <(const_reference v		///< [in] Right operand.    
					  ) const
	{	
		return len2() < v.len2();
	}

	/** '>' operator, Length comparision. Returns true if the length of the left hand 
		side vector is greater than the length of the right hand side vector. Uses 
		the square of the length of the two vectors for speed optimization reasons. No 
		epsilon value is used here, but see operator '>='.
		\return True if lenght of LHS is greater than the length of RHS, false otherwise.*/
	bool_t	operator >(const_reference v		///< [in] Right operand.    
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
	bool_t	operator <=(const_reference v		///< [in] Right operand.    
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
	bool_t	operator >=(const_reference v		///< [in] Right operand.    
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
	bool_t		equal_exact(const_reference v		///< [in] Vector to compare with
						   )const
	{
		return (x() == v.x()) && (y() == v.y()) && (z() == v.z());
	}

	/** Equal with custom epsilon comparision. Tests two vectors for equality.
		Supplied epsilon value is used. 
		\return True if all corresponding elements of the two vectors is no further,
		than epsilon apart (absolute value), false otherwise.. */
	bool_t		equal_epsilon(const_reference v,	///< [in] Vector to compare with
							  const T epsilon		///< [in] Epsilon value to use when comparing
							 )const
	{
		return	( Abs(x() - v.x()) <= epsilon ) && 
				( Abs(y() - v.y()) <= epsilon ) &&
				( Abs(z() - v.z()) <= epsilon );
	}


	// -----------------------
	// --- Debug Functions ---
	// -----------------------

	/// Convert vector to a string for debug print.
	std::string				str(int_t iDecimals= -1	///< [in] Number of decimals
							   ) const
	{
		char sz[40];
		const float_t fx = (float_t)x();
		const float_t fy = (float_t)y();
		const float_t fz = (float_t)z();
		switch ( iDecimals )
		{
			case 0	: sprintf(sz, "[%.0f, %.0f, %.0f]", fx, fy, fz); break;
			case 1	: sprintf(sz, "[%.1f, %.1f, %.1f]", fx, fy, fz); break;
			case 2	: sprintf(sz, "[%.2f, %.2f, %.2f]", fx, fy, fz); break;
			case 3	: sprintf(sz, "[%.3f, %.3f, %.3f]", fx, fy, fz); break;
			case 4	: sprintf(sz, "[%.4f, %.4f, %.4f]", fx, fy, fz); break;
			case 5	: sprintf(sz, "[%.5f, %.5f, %.5f]", fx, fy, fz); break;
			case 6	: sprintf(sz, "[%.6f, %.6f, %.6f]", fx, fy, fz); break;
			default	: sprintf(sz, "[%f, %f, %f]", fx, fy, fz);
		}
		return std::string(sz);
	}
	

	// -------------------
	// --- Member Data ---
	// -------------------

private:
	T	m_d[3];

};


// ----------------------------------------------
// --- Initialization Of Static Const Members ---
// ----------------------------------------------

template <typename T>
const v3<T> v3<T>::V_ZERO	=  v3<T>(T(0), T(0), T(0));

template <typename T>
const v3<T> v3<T>::V_X		=  v3<T>(T(1), T(0), T(0));

template <typename T>
const v3<T> v3<T>::V_Y		=  v3<T>(T(0), T(1), T(0));

template <typename T>
const v3<T> v3<T>::V_Z		=  v3<T>(T(0), T(0), T(1));

// -----------------
// --- Operators ---
// -----------------

/** '*' operator, Scales the vector with a scalar. 
	\return */
template <typename T>
inline v3<T>	operator *(	const T fVal, 		///< [in] Left Hand Side operand (scalar).
							const v3<T>& v)		///< [in] Right hand side operand (v3). 
{
	return v*fVal;	// Implement using operator v3<T>*T
}


// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write vector to stream. 
	\return Reference to (modified) ostream. */
template <typename T>
inline std::ostream&	operator <<(std::ostream& os,		///< [out] Output stream to write to.
									const v3<T>& v)			///< [in] Vector to write.
{
    os << "[" << v.x() << ", " << v.y() << ", " << v.z() <<  "]";
    return os;
}

/** '>>' operator, Read input from stream.
@todo math::v3 Handle commas and brackets when reading
@see https://codereview.stackexchange.com/questions/216343/overloading-istream-to-read-comma-separated-input
@return Reference to (modified) istream.
*/
template <typename T>
inline std::istream&		operator >>(std::istream& is,		///< [in]  Input stream to read from.	
										v3<T>& v)				///< [out] Vector recieve the data from the stream.
{
	is >> v.x() >> v.y() >> v.z();
	return is;
}

using v3f = v3<float>;
using v3d = v3<double>;
using v3i32 = v3<std::int32_t>;
using v3i64 = v3<std::int64_t>;


}





