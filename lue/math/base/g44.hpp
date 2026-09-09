#pragma once
// ****************************************************************************
// CREATED_ON	: 17-06-03 17:00:19
// CREATED_BY	: Martin Lutken
// ---------------------------------------------------------------------------- 
// COPYRIGHT	: 
// ****************************************************************************

#include <stdio.h>
#include <iostream.h>

#include "../lue_math.h"
#include "../utils.hpp"
#include "../stdmath.hpp"

#include "v3.hpp"
#include "m33.hpp"
#include "v4.hpp"
#include "m44.hpp"


namespace lue::math {



/**	
Specialized 4x4 matrix with bottom row allways 0,0,0,1.
Used in computer graphics XX more XX.
The g44 matrix is internally represented as a m33 matrix and a v3 vector.
As the bottom row of the 4x4 matrix is allways 0 0 0 1
Matrix layout:
\pre
	column, row

	m0,0	m1,0	m2,0	m3,0	=	mx,x	my,x	mz,x	mw,x
	m0,1	m1,1	m2,1	m3,1	=	mx,y	my,y	mz,y	mw,y
	m0,2	m1,2	m2,2	m3,2	=	mx,z	my,z	mz,z	mw,z
	 0		 0		 0		 1		=	 0		 0		 0		 1
\endpre
*/


template <typename T>
class g44
{

public:	

	// ----------------
	// --- Typedefs ---
	// ----------------
	typedef g44<T>					value_type;
	typedef value_type&				reference;
	typedef const value_type&		const_reference;
	typedef value_type*				pointer;
	typedef const value_type*		const_pointer;
	typedef unsigned_int_t			index_type;

	// *** vector typedefs ***
	typedef v3<T>					vec_value_type;
	typedef vec_value_type&			vec_reference;
	typedef const vec_value_type&	vec_const_reference;

	// *** m44 typedefs ***
	typedef m44<T>					m44_value_type;
	typedef m44_value_type&			m44_reference;
	typedef const m44_value_type&	m44_const_reference;

	// *** v4 typedefs ***
	typedef v4<T>					v4_value_type;
	typedef v4_value_type&			v4_reference;
	typedef const v4_value_type&	v4_const_reference;

	// *** m33 typedefs ***
    typedef gmath::m33<T>			m33_value_type;
	typedef m33_value_type&			m33_reference;
	typedef const m33_value_type&	m33_const_reference;

	// *** v3 typedefs ***
    typedef gmath::v3<T>			v3_value_type;
	typedef v3_value_type&			v3_reference;
	typedef const v3_value_type&	v3_const_reference;

	
	// ----------------------------
	// --- Const Static Members ---
	// ----------------------------

	/** Constant zero matrix.*/
	static const value_type M_ZERO;

	/** Constant identity matrix.*/
	static const value_type M_I;

	
public:

	// --------------------------------------------
	// --- Constructors / Set values functions. ---
	// --------------------------------------------

	/** Default constructor with no initialization. */
	g44() {}

	/** Copy constructor. Initializing with a general g44<>.*/
	template <typename T1>
	g44(const g44<T1>& m)				///< [in] General g44<TYPE> matrix
		:	m_m33(static_cast<m33_value_type>(m.m33_ref())), 
			m_v3(static_cast<v3_value_type>(m.v3_ref())) {}


	/** Constructor with m33, v3 initialization. */
	g44(m33_const_reference m,			///< [in] m33 matrix
		v3_const_reference v)			///< [in] v3 vector
		:	m_m33(m), m_v3(v) {}	


	/** Constructor with m44 initialization. */
	g44(m44_const_reference m)			///< [in] m44 matrix
	{
		m_m33.
	}	

	// ----------------------------
	// --- Set values functions ---
	// ----------------------------




	// -----------------------
	// --- Access elements ---
	// -----------------------

	/** [] Operator. Acces individual columns of the matrix.*/
	vec_reference			operator[](index_type i)	///< [in] Index value. Should be in range 0..3 ~ X, Y, Z, W.
	{	return m_Cols[i]; }
	vec_const_reference		operator[](index_type i) const
	{	return m_Cols[i]; }

	/** Access column X. Use this function to get acces to the X-column of the matrix. 
		\return Reference to coloumn X as a vector. */
	vec_reference		xcol()			{ return m_ColX; }
	vec_const_reference	xcol()	const	{ return m_ColX; }

	/** Access column Y. Use this function to get acces to the Y-column of the matrix. 
		\return Reference to coloumn Y as a vector. */
	vec_reference		ycol()			{ return m_ColY; }
	vec_const_reference	ycol()	const	{ return m_ColY; }

	/** Access column Z. Use this function to get acces to the Z-column of the matrix. 
		\return Reference to coloumn Z as a vector. */
	vec_reference		zcol()			{ return m_ColZ; }
	vec_const_reference	zcol()	const	{ return m_ColZ; }

	/** Access column W. Use this function to get acces to the W-column of the matrix. 
		\return Reference to coloumn W as a vector. */
	vec_reference		wcol()			{ return m_ColW; }
	vec_const_reference	wcol()	const	{ return m_ColW; }

private:
	/// Return reference m33 matrix.
	m33_reference			m33_ref()			{ return m_m33; }
	m33_const_reference		m33_ref() const		{ return m_m33; }

	/// Return reference v3 vector.
	v3_reference			v3_ref()			{ return m_v3; }
	v3_const_reference		v3_ref() const		{ return m_v3; }


public:

	// ------------------------------------
	// --- Simple Set And Get Functions ---
	// ------------------------------------

	// **************************
	// *** Operator overloads ***
	// **************************

	// *** Assignment operators ***

	/** '=' operator. Assignment from a general m44<T>.*/
	template <typename T1>
	reference			operator =(const m44<T1>& m)		///< [in] Right hand side operand.
	{
		xcol_ref() = static_cast<v4_value_type>(m.xcol_ref()); 
		ycol_ref() = static_cast<v4_value_type>(m.ycol_ref()); 
		zcol_ref() = static_cast<v4_value_type>(m.zcol_ref()); 
		wcol_ref() = static_cast<v4_value_type>(m.wcol_ref()); 
		return *this;
	}

	/** '+=' operator. Add matrix with another. */
	reference			operator +=(const_reference m)		///< [in] Right hand side operand.
	{
		xcol_ref() += m.xcol_ref(); 
		ycol_ref() += m.ycol_ref(); 
		zcol_ref() += m.zcol_ref(); 
		wcol_ref() += m.wcol_ref(); 
		return *this;
	}

	/** '-=' operator. Substract a matrix from this matrix.*/
	reference			operator -=(const_reference m)		///< [in] Right hand side operand.
	{
		xcol_ref() -= m.xcol_ref(); 
		ycol_ref() -= m.ycol_ref(); 
		zcol_ref() -= m.zcol_ref(); 
		wcol_ref() -= m.wcol_ref(); 
		return *this;
	}

	/** '*=' operator. Scales the matrix with a scalar. (I.e. multiplying the elements with a scalar.) */
	reference			operator *=(const T fVal)			///< [in] Right hand side operand. Scalar used to scale the vector.
	{
		xcol_ref() *= fVal; 
		ycol_ref() *= fVal; 
		zcol_ref() *= fVal; 
		wcol_ref() *= fVal; 
		return *this;
	}

	// *********************************
	// *** Friend Operator overloads ***
	// *********************************



	/** '+' operator. Matrix - Matrix addition. m44 + m44. */
	friend value_type		operator +(	const_reference m1,			///< [in] Left hand side operand (m44).
										const_reference m2)			///< [in] Right hand side operand (m44).
	{
		return value_type(	
			m1.xcol_ref() + m2.xcol_ref(),
			m1.ycol_ref() + m2.ycol_ref(),
			m1.zcol_ref() + m2.zcol_ref(),
			m1.wcol_ref() + m2.wcol_ref()
			);
	}

	/** '-' operator. Matrix - Matrix substraction. m44 - m44. */
	friend value_type		operator -(	const_reference m1,			///< [in] Left hand side operand (m44).
										const_reference m2)			///< [in] Right hand side operand (m44).
	{
		return value_type(	
			m1.xcol_ref() - m2.xcol_ref(),
			m1.ycol_ref() - m2.ycol_ref(),
			m1.zcol_ref() - m2.zcol_ref(),
			m1.wcol_ref() - m2.wcol_ref()
			);
	}


	// *** Multiplication ***

	/** '*' operator. Matrix - Scalar multiplication. m44 * T. Scales matrix with a scalar. */
	friend value_type		operator *(	const_reference m,		///< [in] Left hand side operand matrix (m44).
										const T fVal)			///< [in] Right hand side operand scalar (T).
	{
		return value_type(	m.xcol_ref()*fVal, m.ycol_ref() * fVal, m.zcol_ref() * fVal, m.wcol_ref() * fVal);
	}

	/** '*' operator. Scalar - Matrix  multiplication. T * m44. Scales matrix with a scalar. */
	friend value_type		operator *(	const T fVal,			///< [in] Left hand side operand scalar (T).
										const_reference m)		///< [in] Right hand side operand matrix (m44).
	{
		return value_type(	m.xcol_ref()*fVal, m.ycol_ref() * fVal, m.zcol_ref() * fVal, m.wcol_ref() * fVal);
	}

	/** '*' operator. Matrix - Vector multiplication. m44 * v4. */
	friend v4_value_type	operator *(	const_reference m,		///< [in] Left hand side operand (m44).
										v4_const_reference v)	///< [in] Right hand side operand (v4).
	{
		return v4_value_type(
			m.xcol_ref().x()*v.x() + m.ycol_ref().x()*v.y() + m.zcol_ref().x()*v.z() + m.wcol_ref().x()*v.w(),
			m.xcol_ref().y()*v.x() + m.ycol_ref().y()*v.y() + m.zcol_ref().y()*v.z() + m.wcol_ref().y()*v.w(),
			m.xcol_ref().z()*v.x() + m.ycol_ref().z()*v.y() + m.zcol_ref().z()*v.z() + m.wcol_ref().z()*v.w(),
			m.xcol_ref().w()*v.x() + m.ycol_ref().w()*v.y() + m.zcol_ref().w()*v.z() + m.wcol_ref().w()*v.w());
	}

	/** '*' operator. Vector - Matrix multiplication. v4 * m44. (Vector is 
		transposed before multiplication)*/
	friend v4_value_type	operator *(	v4_const_reference v,		///< [in] Left hand side operand (v4).
										const_reference m)			///< [in] Right hand side operand (m44).
	{
		return v4_value_type(	m.xcol_ref()*v, m.ycol_ref()*v, m.zcol_ref()*v, m.wcol_ref()*v);
	}

	/** '*' operator. Matrix - Matrix multiplication. g44 * g44. */
	friend value_type		operator *(	const_reference m1,			///< [in] Left hand side operand (m33).
										const_reference m2)			///< [in] Right hand side operand (m33).
	{
		return value_type(	
			m1.xcol_ref().x() * m2.xcol_ref().x() + m1.ycol_ref().x() * m2.xcol_ref().y() + m1.zcol_ref().x() * m2.xcol_ref().z() + m1.wcol_ref().x() * m2.xcol_ref().w(), 
			m1.xcol_ref().y() * m2.xcol_ref().x() + m1.ycol_ref().y() * m2.xcol_ref().y() + m1.zcol_ref().y() * m2.xcol_ref().z() + m1.wcol_ref().y() * m2.xcol_ref().w(),
			m1.xcol_ref().z() * m2.xcol_ref().x() + m1.ycol_ref().z() * m2.xcol_ref().y() + m1.zcol_ref().z() * m2.xcol_ref().z() + m1.wcol_ref().z() * m2.xcol_ref().w(),
			m1.xcol_ref().w() * m2.xcol_ref().x() + m1.ycol_ref().w() * m2.xcol_ref().y() + m1.zcol_ref().w() * m2.xcol_ref().z() + m1.wcol_ref().w() * m2.xcol_ref().w(),

			m1.xcol_ref().x() * m2.ycol_ref().x() + m1.ycol_ref().x() * m2.ycol_ref().y() + m1.zcol_ref().x() * m2.ycol_ref().z() + m1.wcol_ref().x() * m2.ycol_ref().w(), 
			m1.xcol_ref().y() * m2.ycol_ref().x() + m1.ycol_ref().y() * m2.ycol_ref().y() + m1.zcol_ref().y() * m2.ycol_ref().z() + m1.wcol_ref().y() * m2.ycol_ref().w(),
			m1.xcol_ref().z() * m2.ycol_ref().x() + m1.ycol_ref().z() * m2.ycol_ref().y() + m1.zcol_ref().z() * m2.ycol_ref().z() + m1.wcol_ref().z() * m2.ycol_ref().w(),
			m1.xcol_ref().w() * m2.ycol_ref().x() + m1.ycol_ref().w() * m2.ycol_ref().y() + m1.zcol_ref().w() * m2.ycol_ref().z() + m1.wcol_ref().w() * m2.ycol_ref().w(),

			m1.xcol_ref().x() * m2.zcol_ref().x() + m1.ycol_ref().x() * m2.zcol_ref().y() + m1.zcol_ref().x() * m2.zcol_ref().z() + m1.wcol_ref().x() * m2.zcol_ref().w(), 
			m1.xcol_ref().y() * m2.zcol_ref().x() + m1.ycol_ref().y() * m2.zcol_ref().y() + m1.zcol_ref().y() * m2.zcol_ref().z() + m1.wcol_ref().y() * m2.zcol_ref().w(),
			m1.xcol_ref().z() * m2.zcol_ref().x() + m1.ycol_ref().z() * m2.zcol_ref().y() + m1.zcol_ref().z() * m2.zcol_ref().z() + m1.wcol_ref().z() * m2.zcol_ref().w(),
			m1.xcol_ref().w() * m2.zcol_ref().x() + m1.ycol_ref().w() * m2.zcol_ref().y() + m1.zcol_ref().w() * m2.zcol_ref().z() + m1.wcol_ref().w() * m2.zcol_ref().w(),

			m1.xcol_ref().x() * m2.wcol_ref().x() + m1.ycol_ref().x() * m2.wcol_ref().y() + m1.zcol_ref().x() * m2.wcol_ref().z() + m1.wcol_ref().x() * m2.wcol_ref().w(), 
			m1.xcol_ref().y() * m2.wcol_ref().x() + m1.ycol_ref().y() * m2.wcol_ref().y() + m1.zcol_ref().y() * m2.wcol_ref().z() + m1.wcol_ref().y() * m2.wcol_ref().w(),
			m1.xcol_ref().z() * m2.wcol_ref().x() + m1.ycol_ref().z() * m2.wcol_ref().y() + m1.zcol_ref().z() * m2.wcol_ref().z() + m1.wcol_ref().z() * m2.wcol_ref().w(),
			m1.xcol_ref().w() * m2.wcol_ref().x() + m1.ycol_ref().w() * m2.wcol_ref().y() + m1.zcol_ref().w() * m2.wcol_ref().z() + m1.wcol_ref().w() * m2.wcol_ref().w()
			);
	}



	/** [] Operator. Acces individual columns of the matrix.*/
	v4_reference			operator[](index_type i)				///< [in] Index value. Should be in range 0..2 ~ X, Y, Z.
	{
		return m_Cols[i];
	}

	/** [] Operator. Acces individual columns of the matrix.*/
	v4_const_reference		operator[](index_type i) const			///< [in] Index value. Should be in range 0..2 ~ X, Y, Z.
	{
		return m_Cols[i];
	}

	// ************************
	// *** Stream operators ***
	// ************************

	/** '<<' operator. Write output to stream. */
	friend ostream&		operator <<(ostream &os,			///< [out] Output stream to write to.
									const_reference m)		///< [in] Matrix to write.
	{
		os<<m.xcol_ref()<<m.ycol_ref()<<m.zcol_ref()<<m.wcol_ref();
		return os;
	}

	/** '>>' operator. Read input from stream. */
	friend istream&		operator >>(istream &is,			///< [in]  Input stream to read from.	
									reference m)			///< [out] Matrix recieve the data from the stream.
	{
		is>>m.xcol_ref()>>m.ycol_ref()>>m.zcol_ref()>>m.wcol_ref();
		return is;
	}
	

	// *************************
	// *** Compare functions ***
	// *************************
	

	/** Equal with exact comparision. Tests two matrices for equality.
		No epsilon value is used. Use with care when comparing floating point 
		type values with this function. 
		\return True if all corresponding elements of the two matrices are exactly
		equal, false otherwise. */
	bool		equal_exact(const_reference m)		///< [in] Matrix to compare with
	{
		return	xcol_ref().equal_exact(m.xcol_ref()) && 
				ycol_ref().equal_exact(m.ycol_ref()) && 
				zcol_ref().equal_exact(m.zcol_ref()) && 
				wcol_ref().equal_exact(m.wcol_ref());
	}

	/** Not equal with exact comparision. Tests two matrices for in-equality.
		No epsilon value is used. Use with care when comparing floating point 
		type values with this function.
		\return True if just one pair of corresponding elements of the two matrices 
		are different, false otherwise. */
	bool		not_equal_exact(const_reference m)		///< [in] Matrix to compare with
	{
		return	xcol_ref().not_equal_exact(m.xcol_ref()) || 
				ycol_ref().not_equal_exact(m.ycol_ref()) || 
				zcol_ref().not_equal_exact(m.zcol_ref()) || 
				wcol_ref().not_equal_exact(m.wcol_ref());
	}

	// *************************
	// *** Compare operators ***
	// *************************

	/** '==' operator. Tests two matrices for equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if all corresponding elements of the two matrices is no further,
		than EPSILON<T>() apart (absolute value), false otherwise. */
	friend bool	operator ==(const_reference m0,			///< [in] Left operand.
								const_reference m1)			///< [in] Right operand.    
	{
		return	(m0.xcol_ref() == m1.xcol_ref()) && 
				(m0.ycol_ref() == m1.ycol_ref()) && 
				(m0.zcol_ref() == m1.zcol_ref()) && 
				(m0.wcol_ref() == m1.wcol_ref());
	}

	/** '!=' operator. Tests two matrices for in-equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'not_equal_exact' member function if exact comparision is required. 
		\return True if just one corresponding pair of elements of the two 
		matrices is further than EPSILON<T>() apart (absolute value), 
		false otherwise. */
	friend bool	operator !=(const_reference m0,			///< [in] Left operand.
								const_reference m1)			///< [in] Right operand.    
	{
		return	(m0.xcol_ref() != m1.xcol_ref()) || 
				(m0.ycol_ref() != m1.ycol_ref()) || 
				(m0.zcol_ref() != m1.zcol_ref()) || 
				(m0.wcol_ref() != m1.wcol_ref());
	}


	// **********************
	// *** Util functions ***
	// **********************

	/// Sets all elements to zero.
	void				reset()
	{
		*this = M_ZERO;
	}
	

	/// Sets the matrix to the identity matrix.
	void				load_identity()
	{
		*this = M_I;
	}

	/** Sets the matrix to a diagonal matrix using the elements specified.
		The rest of the elements are set to zero. */	
	void			load_diagonal(	const T xx,		///< [in] xx component
									const T yy,		///< [in] yy component
									const T zz,		///< [in] zz component
									const T ww)		///< [in] ww component
	{
		set(xx,T(0),T(0),T(0), T(0),yy,T(0),T(0), T(0),T(0),zz,T(0), T(0),T(0),T(0),ww);
	}

	/** Sets the matrix to a diagonal matrix using the vector specified.
		The rest of the elements are set to zero. */	
	void			load_diagonal(	v4_const_reference v)	///< [in] Vector to assign to diagonal
	{
		set(v.x(),T(0),T(0),T(0), T(0),v.y(),T(0),T(0), T(0),T(0),v.z(),T(0), T(0),T(0),T(0),v.w());
	}

	/// Transposes the matrix.
	void				transpose()
	{
		LUE_MATH::swap(m_ColX.m_y, m_ColY.m_x);
		LUE_MATH::swap(m_ColX.m_z, m_ColZ.m_x);
		LUE_MATH::swap(m_ColX.m_w, m_ColW.m_x);
		LUE_MATH::swap(m_ColY.m_z, m_ColZ.m_y);
		LUE_MATH::swap(m_ColY.m_w, m_ColW.m_y);
		LUE_MATH::swap(m_ColZ.m_w, m_ColW.m_z);
	}

	/** Returns the transpose of this matrix. 
		\return The transposed of of this matrix*/
	value_type			get_transpose() const
	{
		return value_type(xrow(), yrow(), zrow(), wrow());
	}


	// ************************
	// *** Friend Functions ***
	// ************************


	// ***********************
	// *** Debug Functions ***
	// ***********************

	/// Convert matrix to a string for debug print.
	std::string			str(const int iDecimals= -1) const	///< [in] Number of decimals
	{
		std::string s = xrow().str(iDecimals);
			s += "\n" + yrow().str(iDecimals);
			s += "\n" + zrow().str(iDecimals);
			s += "\n" + wrow().str(iDecimals);
		return s;
	}



	// ************************************
	// ************************************
	// *** Simple Set And Get Functions ***
	// ************************************
	// ************************************


	/** Set all fields to same value. */
	void				set(const T fVal)					///< [in] All elements are set to this value.
	{
		xcol_ref().set(fVal);
		ycol_ref().set(fVal);
		zcol_ref().set(fVal);	
		wcol_ref().set(fVal);	
	}

	/** Set values with 4 vectors. */
	void				set(v4_const_reference colX,		///< [in] X Column of matrix
							v4_const_reference colY,		///< [in] Y Column of matrix
							v4_const_reference colZ,		///< [in] Z Column of matrix.
							v4_const_reference colW)		///< [in] W Column of matrix.
	{
		xcol_ref()	= colX;
		ycol_ref()	= colY;
		zcol_ref()	= colZ;
		wcol_ref()	= colW;
	}

	/** Set values with 16 numbers. */
	void				set	(	const T fxx, const T fxy, const T fxz, const T fxw,		// ColX
								const T fyx, const T fyy, const T fyz, const T fyw,		// ColY
								const T fzx, const T fzy, const T fzz, const T fzw, 	// ColZ
								const T fwx, const T fwy, const T fwz, const T fww ) 	// ColW
	{
		xcol_ref().set(fxx, fxy, fxz, fxw);
		ycol_ref().set(fyx, fyy, fyz, fyw);
		zcol_ref().set(fzx, fzy, fzz, fzw);	
		wcol_ref().set(fwx, fwy, fwz, fww);	
	}


	// *************************************************
	// *** Acces Row and Colums as Vectors functions ***
	// *************************************************
public:

	////////////////////////////
	// Column acces functions //
	////////////////////////////
	
	/// Return copy of column X.
	v4_value_type			xcol()	const	{ return m_ColX; }
	/// Return copy of column Y.
	v4_value_type			ycol()	const	{ return m_ColY; }
	/// Return copy of column Z.
	v4_value_type			zcol()	const	{ return m_ColZ; }
	/// Return copy of column W.
	v4_value_type			wcol()	const	{ return m_ColW; }

	/// Set column X.
	void			xcol(v4_const_reference v)		///< [in] Vector to assign to X-column	
	{	
		xcol_ref() = v; 
	}
	/// Set column Y.
	void			ycol(v4_const_reference v)		///< [in] Vector to assign to Y-column	
	{ 
		ycol_ref() = v; 
	}
	/// Set column Z.
	void			zcol(v4_const_reference v)		///< [in] Vector to assign to Z-column	
	{ 
		zcol_ref() = v; 
	}
	/// Set column W.
	void			wcol(v4_const_reference v)		///< [in] Vector to assign to W-column	
	{ 
		wcol_ref() = v; 
	}


	/////////////////////////
	// Row acces functions //
	/////////////////////////

	/// Return copy of row X.
	v4_value_type			xrow()	const	
	{ 
		return v4_value_type(m_ColX.x(), m_ColY.x(), m_ColZ.x(), m_ColW.x()); 
	}
	/// Return copy of row Y.
	v4_value_type			yrow()	const	
	{ 
		return v4_value_type(m_ColX.y(), m_ColY.y(), m_ColZ.y(), m_ColW.y()); 
	}
	/// Return copy of row Z.
	v4_value_type			zrow()	const	
	{ 
		return v4_value_type(m_ColX.z(), m_ColY.z(), m_ColZ.z(), m_ColW.z()); 
	}
	/// Return copy of row W.
	v4_value_type			wrow()	const	
	{ 
		return v4_value_type(m_ColX.w(), m_ColY.w(), m_ColZ.w(), m_ColW.w()); 
	}

	/// Set row X.
	void			xrow(v4_const_reference v)		///< [in] Vector to assign to X-row	
	{ 
		xcol_ref().x(v.x()); 
		ycol_ref().x(v.y()); 
		zcol_ref().x(v.z()); 
		wcol_ref().x(v.w()); 
	}
	/// Set row Y.
	void			yrow(v4_const_reference v)		///< [in] Vector to assign to Y-row	
	{ 
		xcol_ref().y(v.x()); 
		ycol_ref().y(v.y()); 
		zcol_ref().y(v.z()); 
		wcol_ref().y(v.w()); 
	}
	/// Set row Z.
	void			zrow(v4_const_reference v)		///< [in] Vector to assign to Z-row	
	{ 
		xcol_ref().z(v.x()); 
		ycol_ref().z(v.y()); 
		zcol_ref().z(v.z()); 
		wcol_ref().z(v.w()); 
	}

	/// Set row W.
	void			wrow(v4_const_reference v)		///< [in] Vector to assign to W-row	
	{ 
		xcol_ref().w(v.x()); 
		ycol_ref().w(v.y()); 
		zcol_ref().w(v.z()); 
		wcol_ref().w(v.w()); 
	}


	// -------------------
	// --- Member Data ---
	// -------------------

private:
	union 
	{
		struct 
		{
			m33_value_type					m_m33;		///< 3x3 matrix (m33<T>), topleft part of the total 4x4 matrix
			v3_value_type					m_v3;		///< X,Y,Z components of rightmost coloumn of the total 4x4 matrix
		};
		struct 
		{
			vec_value_type m_ColX, m_ColY, m_ColZ, m_ColW;
		};
		struct 
		{
			vec_value_type m_Cols[4];
		};
	};
};

// **********************************************
// *** Initialization Of Static Const Members ***
// **********************************************

template <class T>
const int m44<T>::X		= 0;  
	
template <class T>
const int m44<T>::Y		= 1;  

template <class T>
const int m44<T>::Z		= 2;  

template <class T>
const int m44<T>::W		= 3;  


template <class T>
const m44<T> m44<T>::M_ZERO	=  m44<T>(T(0),T(0),T(0),T(0), T(0),T(0),T(0),T(0), T(0),T(0),T(0),T(0), T(0),T(0),T(0),T(0));

template <class T>
const m44<T> m44<T>::M_I	=  m44<T>(T(1),T(0),T(0),T(0), T(0),T(1),T(0),T(0), T(0),T(0),T(1),T(0), T(0),T(0),T(0),T(1));

typedef g44<float>	g44f;
typedef g44<int>    g44i;


}

