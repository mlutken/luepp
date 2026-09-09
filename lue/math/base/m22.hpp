#pragma once
// ****************************************************************************
// CREATED_ON	: 17-06-03 17:00:19
// CREATED_BY	: Martin Lutken
// ---------------------------------------------------------------------------- 
// COPYRIGHT	: 
// ****************************************************************************

#include <stdio.h>
#include <iostream>
#include <math.h>

#include <math/base/lue_math.h>
#include "utils.hpp"
#include "stdmath.hpp"
#include <math/base/lue_math_internal_utils.hpp>

#include "v2.hpp"


namespace lue::math {



/** 
2D matrix class. XXX
Matrix layout:
	column, row

	m0,0	m1,0	=	mx,x	my,x
	m0,1	m1,1	=	mx,y	my,y
*/
template <typename T>
class m22
{
public:	

	// ----------------
	// --- Typedefs ---
	// ----------------
	typedef m22<T>					value_type;
	typedef value_type&				reference;
	typedef const value_type&		const_reference;
	typedef value_type*				pointer;
	typedef const value_type*		const_pointer;
    typedef size_t                  index_type;

	// *** vector typedefs ***
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;
	
	// ----------------------------
	// --- Const Static Members ---
	// ----------------------------
	static const value_type M_ZERO;		///< Constant zero matrix.
	static const value_type M_I;		///< Identity matrix.



public:

	// --------------------------------------------
	// --- Constructors / Set values functions. ---
	// --------------------------------------------

	/** Default constructor with no initialization. */
	m22() {}

	/** Copy constructor. Initializing with a general m22<T>.*/
	template <typename T1>
	m22(const m22<T1>& m)	
//		: 	m_Cols[X](static_cast<v2_value_type>(m.xcol())), 
//			m_Cols[Y](static_cast<v2_value_type>(m.ycol())) {}
	{
		m_Cols[X] = m.xcol(); m_Cols[Y] = m.ycol();
	}


	/** Constructor with single value initialization. */
	explicit m22(const T Val) 			///< [in] All elements are set to this value.
//		: m_Cols[X](Val), m_Cols[Y](Val) {}	
	{
		m_Cols[X].set(Val); m_Cols[Y].set(Val);
	}


	/** Constructor with 2 vector initialization. */
	m22(v2_const_reference colX,		///< [in] X Column of matrix
		v2_const_reference colY)		///< [in] Y Column of matrix
//		:	m_Cols[X](colX), m_Cols[Y](colY) {}	
	{
		m_Cols[X] = colX; m_Cols[Y]	= colY;
	}


	/** Constructor with 4 values initialization. */
	m22	(	const T fxx,			///< [in] Column-X, X component 
			const T fxy,			///< [in] Column-X, Y component
			const T fyx,			///< [in] Column-Y, X component
			const T fyy)			///< [in] Column-Y, Y component 
//			: m_Cols[X](fxx, fxy), m_Cols[Y](fyx, fyy){}	
	{
		m_Cols[X].set(fxx, fxy); 
		m_Cols[Y].set(fyx, fyy); 
	}


	// ----------------------------
	// --- Set values functions ---
	// ----------------------------

	/** Set all fields to same value. */
	void				set(const T Val)				///< [in] All elements are set to this value.
	{
		xcol().set(Val);
		ycol().set(Val);
	}

	/** Set values with 2 vectors. */
	void				set(v2_const_reference colX,	///< [in] Column-X of matrix
							v2_const_reference colY)	///< [in] Column-Y of matrix
	{
		xcol() = colX;
		ycol() = colY;
	}

	/** Set values with 4 numbers. */
	void				set	(	const T fxx,			///< [in] Column-X, X component 
								const T fxy,			///< [in] Column-X, Y component
								const T fyx,			///< [in] Column-Y, X component
								const T fyy)			///< [in] Column-Y, Y component 
	{
		xcol().set(fxx, fxy);
		ycol().set(fyx, fyy);
	}

	// -----------------------
	// --- Access elements ---
	// -----------------------

	/** [] Operator. Acces individual columns of the matrix.*/
	v2_reference			operator[](index_type i)	///< [in] Index value. Should be in range 0..1 ~ X, Y.
	{	return m_Cols[i]; }
	v2_const_reference		operator[](index_type i) const
	{	return m_Cols[i]; }

	/** Access column X. Use this function to get acces to the X-column of the matrix. 
		\return Reference to coloumn X as a vector. */
	v2_reference		xcol()			{ return m_Cols[X]; }
	v2_const_reference	xcol()	const	{ return m_Cols[X]; }

	/** Access column Y. Use this function to get acces to the Y-column of the matrix. 
		\return Reference to coloumn Y as a vector. */
	v2_reference		ycol()			{ return m_Cols[Y]; }
	v2_const_reference	ycol()	const	{ return m_Cols[Y]; }

	// ------------------------------------
	// --- Simple Set And Get Functions ---
	// ------------------------------------

	/** Get copy of X-row. Use this function to get a copy of the X-row of the matrix. 
		\return Copy of row X as a vector. */
	v2_value_type		get_xrow()	const	
	{ 	
		return v2_value_type(xcol().x(), ycol().x()); 
	}
	
	/** Get copy of Y-row. Use this function to get a copy of the Y-row of the matrix. 
		\return Copy of row Y as a vector. */
	v2_value_type		get_yrow()	const	
	{ 	
		return v2_value_type(xcol().y(), ycol().y()); 
	}

	/// Set X-row to vector. Assigns input vector to X-row of matrix.
	void				set_xrow(v2_const_reference v)		///< [in] Vector to assign to X-row	
	{ 
		xcol().x() = v.x(); 
		ycol().x() = v.y(); 
	}

	/// Set Y-row to vector. Assigns input vector to Y-row of matrix.
	void				set_yrow(v2_const_reference v)		///< [in] Vector to assign to Y-row	
	{ 
		xcol().y() = v.x(); 
		ycol().y() = v.y(); 
	}

	// ----------------------------
	// --- Assignment operators ---
	// ----------------------------

	/** '=' operator, Assignment from another m22.*/
	reference			operator =(const_reference m)		///< [in] Right hand side operand.
	{
		xcol() = m.xcol(); 
		ycol() = m.ycol(); 
		return *this;
	}

	// ----------------------------
	// --- Misc. math operators ---
	// ----------------------------

	/** Unary '-' operator, Negate matrix.
		\return Matrix holding the result of the negation. */
	value_type		operator -() const
	{
		return value_type(-xcol(), -ycol());
	}

	/** '/=' operator, Divides the elements of the matrix with a scalar. 
		\return Reference to "this".*/
	reference			operator /=(T Val)		///< [in] Right hand side operand. Dividend used to scale the vector.
	{
		T fV = T(1) / Val;
		xcol() *= fV;
		ycol() *= fV;
		return *this;
	}
	
	/** '/' operator, Matrix - Scalar division. 
		Divides all elements in matrix with a scalar, like this: matrix / number.
		\return Matrix with all elements divided by Val. */
	value_type		operator /( T Val			///< [in] Right hand side operand scalar (T).
							  )const
	{
		T fV = T(1) / Val;
		return value_type( xcol()*fV, ycol()*fV );
	}

	// ------------------------------------
	// --- Add/Substract math operators ---
	// ------------------------------------

	/** '+=' operator, Add matrix with another. 
		\return Reference to "this".*/
	reference			operator +=(const_reference m)		///< [in] Right hand side operand.
	{
		xcol() += m.xcol(); 
		ycol() += m.ycol(); 
		return *this;
	}


	/** '-=' operator, Substract a matrix from this matrix.
		\return Reference to "this".*/
	reference			operator -=(const_reference m)		///< [in] Right hand side operand.
	{
		xcol() -= m.xcol(); 
		ycol() -= m.ycol(); 
		return *this;
	}

	/** '+' operator, Matrix - Matrix addition. m22 + m22. 
		\return Matrix holding the result of the addition. */
	value_type		operator +(	const_reference m		///< [in] Right hand side operand (m22).
							  )const
	{
		return value_type(
			xcol() + m.xcol(),
			ycol() + m.ycol() );
	}

	/** '-' operator, Matrix - Matrix substraction. m22 - m22. 
		\return Matrix holding the result of the substraction. */
	value_type		operator -(	const_reference m		///< [in] Right hand side operand (m22).
							  )const
	{
		return value_type(
			xcol() - m.xcol(),
			ycol() - m.ycol() );
	}


	// -------------------------------------
	// --- Multiplication math operators ---
	// -------------------------------------

	/** '*=' operator, Scales this matrix with a scalar. Operator multiplies the 
		elements with a scalar. 
		\return Reference to "this".*/
	reference			operator *=( T Val)		///< [in] Right hand side operand. Scalar used to scale the vector.
	{
		xcol() *= Val; 
		ycol() *= Val; 
		return *this;
	}

	/** '*' operator, Get copy of matrix scaled with scalar. Operator multiplies the 
		elements with a scalar.
		\return New matrix scaled with the number. */
	value_type		operator *( T Val 		///< [in] Right hand side operand. Scalar used to scale the vector.
							   ) const
	{
		return value_type( xcol()*Val, ycol()*Val ); 
	}
	

	/** '*' operator. Matrix - Vector multiplication. Performs m22 * v2. */
	v2_value_type	operator *(	v2_const_reference v)	///< [in] Right hand side operand (v2).
	{
		return v2_value_type(	xcol().x()*v.x() + ycol().x()*v.y(),
								xcol().y()*v.x() + ycol().y()*v.y() );
	// \ML: This is not fast enough :-(	return v2_value_type(	xrow()*v, yrow()*v );
	}


	/** '*' operator. Matrix - Matrix multiplication. m22 * m22. */
	value_type		operator *(const_reference m 	///< [in] Right hand side operand (m22).
							  ) const
	{
		return value_type(	
			xcol().x() * m.xcol().x() + ycol().x() * m.xcol().y(),
			xcol().y() * m.xcol().x() + ycol().y() * m.xcol().y(),

			xcol().x() * m.ycol().x() + ycol().x() * m.ycol().y(),
			xcol().y() * m.ycol().x() + ycol().y() * m.ycol().y() );
	}


	// ----------------------
	// --- Util functions ---
	// ----------------------

	/// Sets all elements to zero. 
	void			reset()
	{
		*this = M_ZERO;
	}
	

	/// Sets the matrix to the identity matrix.
	void			load_identity()
	{
		*this = M_I;
	}

	/** Sets the matrix to a diagonal matrix using the elements specified.
		The rest of the elements are set to zero. */	
	void			load_diagonal(	const T xx,		///< [in] xx component
									const T yy)		///< [in] yy component
	{
		set(xx, T(0), T(0), yy);
	}

	/** Sets the matrix to a diagonal matrix using the vector specified.
		The rest of the elements are set to zero. */	
	void			load_diagonal(v2_const_reference v)		///< [in] Vector to assign to diagonal
	{
		set(v.x(), T(0), T(0), v.y());
	}

	/** Transposes the matrix. Matrix is transposed. Remember that for an 
		orthonormal matrix (ie. a matrix where all coloumns have lenght 1 and
		perpendicular to each other) the transposed matrix is idetical to the
		inverse. This is a much faster way to calculate the inverse and the
		preferred wat in computer graphics where matrices are often pure rotation/
		orientation matrices which are orthonormal.
		\see get_transpose. */
	void				transpose()
	{
		std::swap(m_Cols[X].m_d[Y], m_Cols[Y].m_d[X]);
	}

	/** Returns the transpose of this matrix. Remember that for an 
		orthonormal matrix (ie. a matrix where all coloumns have lenght 1 and
		perpendicular to each other) the transposed matrix is idetical to the
		inverse. This is a much faster way to calculate the inverse and the
		preferred wat in computer graphics where matrices are often pure rotation/
		orientation matrices which are orthonormal.
		\return The transposed of this matrix*/
	value_type			get_transpose() const
	{
		return value_type(get_xrow(), get_yrow());
	}
	
	// -----------------------------
	// --- Load matrix functions ---
	// -----------------------------
	
	/** Look at direction. Creates an ortho-normal matrix using the input angle
		as direction. */
	void				create_rot(T fAngle)		///< [in] Angle in degrees to rotate (Counter clockwise is positive).
	{
		const T cosAng = Cos(fAngle);
		const T sinAng = Sin(fAngle);
		set(cosAng, sinAng, -sinAng, cosAng);
	}


	/** Look at direction. Creates an ortho-normal matrix using the input vector 
		as direction. */
	void				create_rot(v2_value_type vDir)		///< [in] XX
	{
		vDir.normalize();
		set(vDir.x(), vDir.y(), -vDir.y(), vDir.x());
	}


	// -------------------------------------
	// --- Determinant, Adjoint, Inverse ---
	// -------------------------------------

	/** Finds the determinant of the matrix.
		\return Determinant of matrix. */
	T					det() const
	{
		return xcol().x()*ycol().y() - ycol().x()*xcol().y();
	}

	/** Calculates the adjoint af the matrix. Same as (adjugate matrix or conjugate transpose ).
		\return The adjoint matrix.*/
	value_type			adjoint() const
	{
		return value_type(ycol().y(), -xcol().y(),  -ycol().x(), xcol().x());
	}

	/** Finds the inverse of this matrix. Ie. finds the inverse if possible. 
		Ie. if the determinant is non-zero (det() != 0). 
		No execptions are trown in case det() == 0. If one's unsure whether det() != 0
		then first caluculate Det= m.det() and only if Det != 0 call inverse 
		with result of det() as argument. See also operator '~'.
'		\return The inverse matrix if determinant is NOT zero. Otherwise the 
				unchanged matrix is returned.*/
	value_type			get_inverse(T Det = -1		///< [in] Determinant of this matrix. If Det is -1 then Det is calculated (Det = det())
						   	 	   ) const
	{
		if ( Det == -1) Det = det();
		if ( Det != 0 )
		{	
			Det = T(1)/Det;
			return value_type(	ycol().y()*Det, -xcol().y()*Det,
								-ycol().x()*Det, xcol().x()*Det);
		}
		else return *this;
	}

	// -------------------------
	// --- Compare operators ---
	// -------------------------

	/** '==' operator. Tests two matrices for equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if all corresponding elements of the two matrices is no further,
		than EPSILON<T>() apart (absolute value), false otherwise. */
	bool	operator ==(const_reference m			///< [in] Right operand.    
					   ) const
	{
		return	(xcol() == m.xcol()) && 
				(ycol() == m.ycol());
	}

	/** '!=' operator. Tests two matrices for in-equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if just one corresponding pair of elements of the two 
		matrices is further than EPSILON<T>() apart (absolute value), 
		false otherwise. */
	bool	operator !=(const_reference m			///< [in] Right operand.    
					   ) const
	{
		return	(xcol() != m.xcol()) || 
				(ycol() != m.ycol());
	}


	// -------------------------
	// --- Compare functions ---
	// -------------------------
	

	/** Equal with exact comparision. Tests two matrices for equality.
		No epsilon value is used. Use with care when comparing floating point 
		type values with this function. 
		\return True if all corresponding elements of the two matrices are exactly
		equal, false otherwise. */
	bool		equal_exact(const_reference m)		///< [in] Matrix to compare with
	{
		return	xcol().equal_exact(m.xcol()) && 
				ycol().equal_exact(m.ycol());
	}


	/** Equal with custom epsilon comparision. Tests two vectors for equality.
		Supplied epsilon value is used. 
		\return True if all corresponding elements of the two vectors is no further,
		than epsilon apart (absolute value), false otherwise.. */
	bool		equal_epsilon(const_reference m,	///< [in] Matrix to compare with
							  const T epsilon		///< [in] Epsilon value to use when comparing
							 ) const
	{
		return	xcol().equal_epsilon(m.xcol(), epsilon) && 
				ycol().equal_epsilon(m.ycol(), epsilon);
	}



	// -----------------------
	// --- Debug Functions ---
	// -----------------------

	/// Convert matrix to a string for debug print.
	std::string			str(const int iDecimals= -1	///< [in] Number of decimals
						   ) const
	{
		std::string s = get_xrow().str(iDecimals);
			s += "\n" + get_yrow().str(iDecimals);
		return s;
	}



	// -------------------
	// --- Member Data ---
	// -------------------
private:
	v2_value_type m_Cols[2];
};

// ----------------------------------------------
// --- Initialization Of Static Const Members ---
// ----------------------------------------------

template <class T>
const m22<T> m22<T>::M_ZERO	=  m22<T>(T(0), T(0), T(0), T(0));

template <class T>
const m22<T> m22<T>::M_I	=  m22<T>(T(1), T(0), T(0), T(1));


// -----------------
// --- Operators ---
// -----------------

// '*' operator, Scalar - matrix multiplication. Scales the matrix with a scalar. 
template <typename T>
inline m22<T>	operator *(	T Val, 				// [in] Left Hand Side operand (scalar).
							const m22<T>& m)	// [in] Right hand side operand matrix. 
{
	return m*Val;	// Implement using operator m22<T> * T
}

/** '*' operator. Vector - Matrix multiplication. v2 * m22. (Vector is 
	transposed before multiplication)*/
template <typename T>
inline v2<T>	operator *(	const v2<T>& v,		///< [in] Left hand side operand (v2).
							const m22<T>& m)	///< [in] Right hand side operand (m22).
{
	return v2<T>( m.xcol()*v, m.ycol()*v );
}


// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write matrix to stream. 
	\return Reference to (modified) ostream. */
template <typename T>
inline std::ostream&		operator <<(std::ostream& os,	///< [out] Output stream to write to.
										const m22<T>& m)	///< [in] Matrix to write.
{
	os<<m.xcol()<<m.ycol();
	return os;
}

/** '>>' operator. Read input from stream. */
template <typename T>
inline std::istream&		operator >>(std::istream& is,	///< [in]  Input stream to read from.	
										m22<T>& m)			///< [out] Matrix recieve the data from the stream.
{
	is>>m.xcol()>>m.ycol();
	return is;
}

// --- Specializations of selected functions ---

/// The general version of this would not work with integers.
template<>
inline m22<int>&		m22<int>::operator/=(int Val)
{
	xcol() /= Val;
	ycol() /= Val;
	return *this;
}

/// The general version of this would not work with integers.
template<>
inline m22<int>		m22<int>::operator /( int Val ) const
{
	return m22<int>( xcol() / Val, ycol() / Val );
}


typedef m22<float>	m22f;
typedef m22<int>    m22i;

}


