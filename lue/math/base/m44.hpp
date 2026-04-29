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

#include "lue_math.h"
#include "stdmath.hpp"
#include "lue_math_internal_utils.hpp"

#include "m33.hpp"
#include "v4.hpp"


namespace lue::math {



/** 
4D matrix class. XXX
Matrix layout:
	column, row

	m0,0	m1,0	m2,0	m3,0	=	mx,x	my,x	mz,x	mw,x
	m0,1	m1,1	m2,1	m3,1	=	mx,y	my,y	mz,y	mw,y
	m0,2	m1,2	m2,2	m3,2	=	mx,z	my,z	mz,z	mw,z
	m0,3	m1,3	m2,3	m3,3	=	mx,w	my,w	mz,w	mw,w
*/
template <typename T>
class m44
{
public:	

	// ----------------
	// --- Typedefs ---
	// ----------------
	typedef m44<T>					value_type;
	typedef value_type&				reference;
	typedef const value_type&		const_reference;
	typedef value_type*				pointer;
	typedef const value_type*		const_pointer;
	typedef unsigned_int_t			index_type;

	// *** vector typedefs ***
	typedef v4<T>					vec_value_type;
	typedef vec_value_type&			vec_reference;
	typedef const vec_value_type&	vec_const_reference;
	
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
	m44() {}

	/** Copy constructor. Initializing with a general m44<T>.*/
	template <typename T1>
	m44(const m44<T1>& m)	
		: 	m_ColX(static_cast<vec_value_type>(m.xcol())), 
			m_ColY(static_cast<vec_value_type>(m.ycol())),
			m_ColZ(static_cast<vec_value_type>(m.zcol())),
			m_ColW(static_cast<vec_value_type>(m.wcol())) {}


	/** Constructor with single value initialization. */
	explicit m44(const T fVal) 			///< [in] All elements are set to this value.
		: m_ColX(fVal), m_ColY(fVal), m_ColZ(fVal), m_ColW(fVal) {}	


	/** Constructor with 4 vector initialization. */
	m44(vec_const_reference colX,		///< [in] X Column of matrix
		vec_const_reference colY,		///< [in] Y Column of matrix
		vec_const_reference colZ,		///< [in] Z Column of matrix
		vec_const_reference colW)		///< [in] W Column of matrix
		:	m_ColX(colX), m_ColY(colY), m_ColZ(colZ), m_ColW(colW) {}	


	/** Constructor with 16 values initialization. */
	m44	(	const T fxx,			///< [in] Column-X, X component 
			const T fxy,			///< [in] Column-X, Y component
			const T fxz,			///< [in] Column-X, Z component
			const T fxw,			///< [in] Column-X, W component

			const T fyx,			///< [in] Column-Y, X component
			const T fyy,			///< [in] Column-Y, Y component 
			const T fyz,			///< [in] Column-Y, Z component
			const T fyw,			///< [in] Column-Y, W component

			const T fzx,			///< [in] Column-Z, X component
			const T fzy,			///< [in] Column-Z, Y component 
			const T fzz,			///< [in] Column-Z, Z component
			const T fzw,			///< [in] Column-Z, W component

			const T fwx,			///< [in] Column-W, X component
			const T fwy,			///< [in] Column-W, Y component 
			const T fwz,			///< [in] Column-W, Z component
			const T fww)			///< [in] Column-W, W component

		:	m_ColX(fxx, fxy, fxz, fxw), 
			m_ColY(fyx, fyy, fyz, fyw), 
			m_ColZ(fzx, fzy, fzz, fzw),	
			m_ColW(fwx, fwy, fwz, fww)	{}	

	// ----------------------------
	// --- Set values functions ---
	// ----------------------------

	/** Set all fields to same value. */
	void				set(const T fVal)				///< [in] All elements are set to this value.
	{
		xcol().set(fVal);
		ycol().set(fVal);
		zcol().set(fVal);
		wcol().set(fVal);
	}

	/** Set values with 4 vectors. */
	void				set(vec_const_reference colX,	///< [in] Column-X of matrix
							vec_const_reference colY,	///< [in] Column-Y of matrix
							vec_const_reference colZ,	///< [in] Column-Z of matrix
							vec_const_reference colW)	///< [in] Column-W of matrix
	{
		xcol() = colX;
		ycol() = colY;
		zcol() = colZ;
		wcol() = colW;
	}

	/** Set values with 16 numbers. */
	void				set(const T fxx,				///< [in] Column-X, X component 
							const T fxy,				///< [in] Column-X, Y component
							const T fxz,				///< [in] Column-X, Z component
							const T fxw,				///< [in] Column-X, W component

							const T fyx,				///< [in] Column-Y, X component
							const T fyy,				///< [in] Column-Y, Y component 
							const T fyz,				///< [in] Column-Y, Z component
							const T fyw,				///< [in] Column-Y, W component

							const T fzx,				///< [in] Column-Z, X component
							const T fzy,				///< [in] Column-Z, Y component 
							const T fzz,				///< [in] Column-Z, Z component
							const T fzw,				///< [in] Column-Z, W component

							const T fwx,				///< [in] Column-W, X component
							const T fwy,				///< [in] Column-W, Y component 
							const T fwz,				///< [in] Column-W, Z component
							const T fww)				///< [in] Column-W, W component
	{
		xcol().set(fxx, fxy, fxz, fxw);
		ycol().set(fyx, fyy, fyz, fyw);
		zcol().set(fzx, fzy, fzz, fzw);	
		wcol().set(fwx, fwy, fwz, fww);	
	}

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

	// ------------------------------------
	// --- Simple Set And Get Functions ---
	// ------------------------------------

	/** Get copy of X-row. Use this function to get a copy of the X-row of the matrix. 
		\return Copy of row X as a vector. */
	vec_value_type		get_xrow()	const	
	{ 	
		return vec_value_type(xcol().x(), ycol().x(), zcol().x(), wcol().x()); 
	}
	
	/** Get copy of Y-row. Use this function to get a copy of the Y-row of the matrix. 
		\return Copy of row Y as a vector. */
	vec_value_type		get_yrow()	const	
	{ 	
		return vec_value_type(xcol().y(), ycol().y(), zcol().y(), wcol().y()); 
	}

	/** Get copy of Z-row. Use this function to get a copy of the Z-row of the matrix. 
		\return Copy of row Z as a vector. */
	vec_value_type		get_zrow()	const	
	{ 	
		return vec_value_type(xcol().z(), ycol().z(), zcol().z(), wcol().z()); 
	}

	/** Get copy of W-row. Use this function to get a copy of the W-row of the matrix. 
		\return Copy of row W as a vector. */
	vec_value_type		get_wrow()	const	
	{ 	
		return vec_value_type(xcol().w(), ycol().w(), zcol().w(), wcol().w()); 
	}

	/// Set X-row to vector. Assigns input vector to X-row of matrix.
	void				set_xrow(vec_const_reference v)		///< [in] Vector to assign to X-row	
	{ 
		xcol().x() = v.x(); 
		ycol().x() = v.y(); 
		zcol().x() = v.z(); 
		wcol().x() = v.w(); 
	}

	/// Set Y-row to vector. Assigns input vector to Y-row of matrix.
	void				set_yrow(vec_const_reference v)		///< [in] Vector to assign to Y-row	
	{ 
		xcol().y() = v.x(); 
		ycol().y() = v.y(); 
		zcol().y() = v.z(); 
		wcol().y() = v.w(); 
	}

	/// Set Z-row to vector. Assigns input vector to Z-row of matrix.
	void				set_zrow(vec_const_reference v)		///< [in] Vector to assign to Z-row	
	{ 
		xcol().z() = v.x(); 
		ycol().z() = v.y(); 
		zcol().z() = v.z(); 
		wcol().z() = v.w(); 
	}

	/// Set W-row to vector. Assigns input vector to W-row of matrix.
	void				set_wrow(vec_const_reference v)		///< [in] Vector to assign to W-row	
	{ 
		xcol().w() = v.x(); 
		ycol().w() = v.y(); 
		zcol().w() = v.z(); 
		wcol().w() = v.w(); 
	}

	// ----------------------------
	// --- Assignment operators ---
	// ----------------------------

	/** '=' operator, Assignment from another m44.*/
	reference			operator =(const_reference m)		///< [in] Right hand side operand.
	{
		xcol() = m.xcol(); 
		ycol() = m.ycol(); 
		zcol() = m.zcol(); 
		wcol() = m.wcol(); 
		return *this;
	}

	// ----------------------------
	// --- Misc. math operators ---
	// ----------------------------

	/** Unary '-' operator, Negate matrix.
		\return Matrix holding the result of the negation. */
	value_type		operator -() const
	{
		return value_type(-xcol(), -ycol(), -zcol(), -wcol());
	}

	/** '/=' operator, Divides the elements of the vector with a scalar. 
		\return Reference to "this".*/
	reference			operator /=(const T fVal)		///< [in] Right hand side operand. Dividend used to scale the vector.
	{
		T fV = T(1) / fVal;
		xcol() *= fV;
		ycol() *= fV;
		zcol() *= fV;
		wcol() *= fV;
		return *this;
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
		zcol() += m.zcol(); 
		wcol() += m.wcol(); 
		return *this;
	}


	/** '-=' operator, Substract a matrix from this matrix.
		\return Reference to "this".*/
	reference			operator -=(const_reference m)		///< [in] Right hand side operand.
	{
		xcol() -= m.xcol(); 
		ycol() -= m.ycol(); 
		zcol() -= m.zcol(); 
		wcol() -= m.wcol(); 
		return *this;
	}

	/** '+' operator, Matrix - Matrix addition. m33 + m33. 
		\return Matrix holding the result of the addition. */
	value_type		operator +(	const_reference m		///< [in] Right hand side operand (m33).
							  )const
	{
		return value_type(
			xcol() + m.xcol(),
			ycol() + m.ycol(),
			zcol() + m.zcol(),
			wcol() + m.wcol() );
	}

	/** '-' operator, Matrix - Matrix substraction. m33 - m33. 
		\return Matrix holding the result of the substraction. */
	value_type		operator -(	const_reference m		///< [in] Right hand side operand (m33).
							  )const
	{
		return value_type(
			xcol() - m.xcol(),
			ycol() - m.ycol(),
			zcol() - m.zcol(),
			wcol() - m.wcol() );
	}


	// -------------------------------------
	// --- Multiplication math operators ---
	// -------------------------------------

	/** '*=' operator, Scales the matrix with a scalar. (I.e. multiplying the 
		elements with a scalar.) 
		\return Reference to "this".*/
	reference			operator *=(const T fVal)		///< [in] Right hand side operand. Scalar used to scale the vector.
	{
		xcol() *= fVal; 
		ycol() *= fVal; 
		zcol() *= fVal; 
		wcol() *= fVal; 
		return *this;
	}

	/** '*' operator, Matrix - Scalar multiplication. 
		Scales matrix with a scalar, like this: matrix * number.
		\return Matrix holding the result of the scaling. */
	value_type		operator *(const T fVal			///< [in] Right hand side operand scalar (T).
							  )const
	{
		return value_type( xcol()*fVal, ycol()*fVal , zcol()*fVal, wcol()*fVal );
	}


	/** '*' operator. Matrix - Vector multiplication. Performs m44 * v4. */
	vec_value_type	operator *(	vec_const_reference v)	///< [in] Right hand side operand (v4).
	{
		return vec_value_type(
			xcol().x()*v.x() + ycol().x()*v.y() + zcol().x()*v.z() + wcol().x()*v.w(),
			xcol().y()*v.x() + ycol().y()*v.y() + zcol().y()*v.z() + wcol().y()*v.w(),
			xcol().z()*v.x() + ycol().z()*v.y() + zcol().z()*v.z() + wcol().z()*v.w(),
			xcol().w()*v.x() + ycol().w()*v.y() + zcol().w()*v.z() + wcol().w()*v.w());
	}


	/** '*' operator. Matrix - Matrix multiplication. m44 * m44. */
	value_type		operator *(const_reference m 	///< [in] Right hand side operand (m44).
							  ) const
	{
		return value_type(	
			xcol().x() * m.xcol().x() + ycol().x() * m.xcol().y() + zcol().x() * m.xcol().z() + wcol().x() * m.xcol().w(), 
			xcol().y() * m.xcol().x() + ycol().y() * m.xcol().y() + zcol().y() * m.xcol().z() + wcol().y() * m.xcol().w(),
			xcol().z() * m.xcol().x() + ycol().z() * m.xcol().y() + zcol().z() * m.xcol().z() + wcol().z() * m.xcol().w(),
			xcol().w() * m.xcol().x() + ycol().w() * m.xcol().y() + zcol().w() * m.xcol().z() + wcol().w() * m.xcol().w(),

			xcol().x() * m.ycol().x() + ycol().x() * m.ycol().y() + zcol().x() * m.ycol().z() + wcol().x() * m.ycol().w(), 
			xcol().y() * m.ycol().x() + ycol().y() * m.ycol().y() + zcol().y() * m.ycol().z() + wcol().y() * m.ycol().w(),
			xcol().z() * m.ycol().x() + ycol().z() * m.ycol().y() + zcol().z() * m.ycol().z() + wcol().z() * m.ycol().w(),
			xcol().w() * m.ycol().x() + ycol().w() * m.ycol().y() + zcol().w() * m.ycol().z() + wcol().w() * m.ycol().w(),

			xcol().x() * m.zcol().x() + ycol().x() * m.zcol().y() + zcol().x() * m.zcol().z() + wcol().x() * m.zcol().w(), 
			xcol().y() * m.zcol().x() + ycol().y() * m.zcol().y() + zcol().y() * m.zcol().z() + wcol().y() * m.zcol().w(),
			xcol().z() * m.zcol().x() + ycol().z() * m.zcol().y() + zcol().z() * m.zcol().z() + wcol().z() * m.zcol().w(),
			xcol().w() * m.zcol().x() + ycol().w() * m.zcol().y() + zcol().w() * m.zcol().z() + wcol().w() * m.zcol().w(),

			xcol().x() * m.wcol().x() + ycol().x() * m.wcol().y() + zcol().x() * m.wcol().z() + wcol().x() * m.wcol().w(), 
			xcol().y() * m.wcol().x() + ycol().y() * m.wcol().y() + zcol().y() * m.wcol().z() + wcol().y() * m.wcol().w(),
			xcol().z() * m.wcol().x() + ycol().z() * m.wcol().y() + zcol().z() * m.wcol().z() + wcol().z() * m.wcol().w(),
			xcol().w() * m.wcol().x() + ycol().w() * m.wcol().y() + zcol().w() * m.wcol().z() + wcol().w() * m.wcol().w()
			);
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
									const T yy,		///< [in] yy component
									const T zz,		///< [in] zz component
									const T ww)		///< [in] ww component
	{
		set(xx,T(0),T(0),T(0), T(0),yy,T(0),T(0), T(0),T(0),zz,T(0), T(0),T(0),T(0),ww);
	}

	/** Sets the matrix to a diagonal matrix using the vector specified.
		The rest of the elements are set to zero. */	
	void			load_diagonal(	vec_const_reference v)	///< [in] Vector to assign to diagonal
	{
		set(v.x(),T(0),T(0),T(0), T(0),v.y(),T(0),T(0), T(0),T(0),v.z(),T(0), T(0),T(0),T(0),v.w());
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
		std::swap(m_ColX.m_y, m_ColY.m_x);
		std::swap(m_ColX.m_z, m_ColZ.m_x);
		std::swap(m_ColX.m_w, m_ColW.m_x);
		std::swap(m_ColY.m_z, m_ColZ.m_y);
		std::swap(m_ColY.m_w, m_ColW.m_y);
		std::swap(m_ColZ.m_w, m_ColW.m_z);
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
		return value_type(get_xrow(), get_yrow(), get_zrow(), get_wrow());
	}


	// -------------------------------------
	// --- Determinant, Adjoint, Inverse ---
	// -------------------------------------

	/** Finds the determinant of the matrix.
		\return Determinant of matrix. */
	T					det() const
	{
		T determinant;
		determinant =
		m_Cols[W][X] * m_Cols[Z][Y] * m_Cols[Y][Z] * m_Cols[X][W]-m_Cols[Z][X] * m_Cols[W][Y] * m_Cols[Y][Z] * m_Cols[X][W]-m_Cols[W][X] * m_Cols[Y][Y] * m_Cols[Z][Z] * m_Cols[X][W]+m_Cols[Y][X] * m_Cols[W][Y] * m_Cols[Z][Z] * m_Cols[X][W]+
		m_Cols[Z][X] * m_Cols[Y][Y] * m_Cols[W][Z] * m_Cols[X][W]-m_Cols[Y][X] * m_Cols[Z][Y] * m_Cols[W][Z] * m_Cols[X][W]-m_Cols[W][X] * m_Cols[Z][Y] * m_Cols[X][Z] * m_Cols[Y][W]+m_Cols[Z][X] * m_Cols[W][Y] * m_Cols[X][Z] * m_Cols[Y][W]+
		m_Cols[W][X] * m_Cols[X][Y] * m_Cols[Z][Z] * m_Cols[Y][W]-m_Cols[X][X] * m_Cols[W][Y] * m_Cols[Z][Z] * m_Cols[Y][W]-m_Cols[Z][X] * m_Cols[X][Y] * m_Cols[W][Z] * m_Cols[Y][W]+m_Cols[X][X] * m_Cols[Z][Y] * m_Cols[W][Z] * m_Cols[Y][W]+
		m_Cols[W][X] * m_Cols[Y][Y] * m_Cols[X][Z] * m_Cols[Z][W]-m_Cols[Y][X] * m_Cols[W][Y] * m_Cols[X][Z] * m_Cols[Z][W]-m_Cols[W][X] * m_Cols[X][Y] * m_Cols[Y][Z] * m_Cols[Z][W]+m_Cols[X][X] * m_Cols[W][Y] * m_Cols[Y][Z] * m_Cols[Z][W]+
		m_Cols[Y][X] * m_Cols[X][Y] * m_Cols[W][Z] * m_Cols[Z][W]-m_Cols[X][X] * m_Cols[Y][Y] * m_Cols[W][Z] * m_Cols[Z][W]-m_Cols[Z][X] * m_Cols[Y][Y] * m_Cols[X][Z] * m_Cols[W][W]+m_Cols[Y][X] * m_Cols[Z][Y] * m_Cols[X][Z] * m_Cols[W][W]+
		m_Cols[Z][X] * m_Cols[X][Y] * m_Cols[Y][Z] * m_Cols[W][W]-m_Cols[X][X] * m_Cols[Z][Y] * m_Cols[Y][Z] * m_Cols[W][W]-m_Cols[Y][X] * m_Cols[X][Y] * m_Cols[Z][Z] * m_Cols[W][W]+m_Cols[X][X] * m_Cols[Y][Y] * m_Cols[Z][Z] * m_Cols[W][W];
		return determinant;
	}

	/** Calculates the adjoint af the matrix. Same as (adjugate matrix or conjugate transpose ).
		
		\return The adjoint matrix.*/
	value_type			adjoint() const
	{
		typedef m33<T>	m33_value_type;
		value_type		adj_mat;

		adj_mat[0][0]  =   m33_value_type( m_Cols[1][1], m_Cols[2][1], m_Cols[3][1], m_Cols[1][2], m_Cols[2][2], m_Cols[3][2], m_Cols[1][3], m_Cols[2][3], m_Cols[3][3]).det();
		adj_mat[1][0]  = - m33_value_type( m_Cols[1][0], m_Cols[2][0], m_Cols[3][0], m_Cols[1][2], m_Cols[2][2], m_Cols[3][2], m_Cols[1][3], m_Cols[2][3], m_Cols[3][3]).det();
		adj_mat[2][0]  =   m33_value_type( m_Cols[1][0], m_Cols[2][0], m_Cols[3][0], m_Cols[1][1], m_Cols[2][1], m_Cols[3][1], m_Cols[1][3], m_Cols[2][3], m_Cols[3][3]).det();
		adj_mat[3][0]  = - m33_value_type( m_Cols[1][0], m_Cols[2][0], m_Cols[3][0], m_Cols[1][1], m_Cols[2][1], m_Cols[3][1], m_Cols[1][2], m_Cols[2][2], m_Cols[3][2]).det();
		    
		adj_mat[0][1]  = - m33_value_type( m_Cols[0][1], m_Cols[2][1], m_Cols[3][1], m_Cols[0][2], m_Cols[2][2], m_Cols[3][2], m_Cols[0][3], m_Cols[2][3], m_Cols[3][3]).det();
		adj_mat[1][1]  =   m33_value_type( m_Cols[0][0], m_Cols[2][0], m_Cols[3][0], m_Cols[0][2], m_Cols[2][2], m_Cols[3][2], m_Cols[0][3], m_Cols[2][3], m_Cols[3][3]).det();
		adj_mat[2][1]  = - m33_value_type( m_Cols[0][0], m_Cols[2][0], m_Cols[3][0], m_Cols[0][1], m_Cols[2][1], m_Cols[3][1], m_Cols[0][3], m_Cols[2][3], m_Cols[3][3]).det();
		adj_mat[3][1]  =   m33_value_type( m_Cols[0][0], m_Cols[2][0], m_Cols[3][0], m_Cols[0][1], m_Cols[2][1], m_Cols[3][1], m_Cols[0][2], m_Cols[2][2], m_Cols[3][2]).det();
		    
		adj_mat[0][2]  =   m33_value_type( m_Cols[0][1], m_Cols[1][1], m_Cols[3][1], m_Cols[0][2], m_Cols[1][2], m_Cols[3][2], m_Cols[0][3], m_Cols[1][3], m_Cols[3][3]).det();
		adj_mat[1][2]  = - m33_value_type( m_Cols[0][0], m_Cols[1][0], m_Cols[3][0], m_Cols[0][2], m_Cols[1][2], m_Cols[3][2], m_Cols[0][3], m_Cols[1][3], m_Cols[3][3]).det();
		adj_mat[2][2]  =   m33_value_type( m_Cols[0][0], m_Cols[1][0], m_Cols[3][0], m_Cols[0][1], m_Cols[1][1], m_Cols[3][1], m_Cols[0][3], m_Cols[1][3], m_Cols[3][3]).det();
		adj_mat[3][2]  = - m33_value_type( m_Cols[0][0], m_Cols[1][0], m_Cols[3][0], m_Cols[0][1], m_Cols[1][1], m_Cols[3][1], m_Cols[0][2], m_Cols[1][2], m_Cols[3][2]).det();
		    
		adj_mat[0][3]  = - m33_value_type( m_Cols[0][1], m_Cols[1][1], m_Cols[2][1], m_Cols[0][2], m_Cols[1][2], m_Cols[2][2], m_Cols[0][3], m_Cols[1][3], m_Cols[2][3]).det();
		adj_mat[1][3]  =   m33_value_type( m_Cols[0][0], m_Cols[1][0], m_Cols[2][0], m_Cols[0][2], m_Cols[1][2], m_Cols[2][2], m_Cols[0][3], m_Cols[1][3], m_Cols[2][3]).det();
		adj_mat[2][3]  = - m33_value_type( m_Cols[0][0], m_Cols[1][0], m_Cols[2][0], m_Cols[0][1], m_Cols[1][1], m_Cols[2][1], m_Cols[0][3], m_Cols[1][3], m_Cols[2][3]).det();
		adj_mat[3][3]  =   m33_value_type( m_Cols[0][0], m_Cols[1][0], m_Cols[2][0], m_Cols[0][1], m_Cols[1][1], m_Cols[2][1], m_Cols[0][2], m_Cols[1][2], m_Cols[2][2]).det();
		return adj_mat;
	}
	

	/** Finds the inverse of this matrix. Ie. finds the inverse if possible. 
		Ie. if the determinant is non-zero (det() != 0). 
		No execptions are trown in case det() == 0. If one's unsure whether det() != 0
		Should call first caluculate Det= m.det() and only if Det != 0 call inverse 
		with Det as argument.
		\return The inverse matrix if determinant is NOT zero. Otherwise the 
				unchanged matrix is returned.*/
	value_type			inverse(T Det = -1		///< [in] Determinant of this matrix. If Det is -1 then Det is calculated (Det = det())
							   ) const
	{
		if ( Det == -1 ) Det = det();
		if ( Det != 0 )
		{	
			Det = T(1)/Det;
			return adjoint()*Det;
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
	bool_t	operator ==(const_reference m			///< [in] Right operand.    
					   ) const
	{
		return	(xcol() == m.xcol()) && 
				(ycol() == m.ycol()) &&
				(zcol() == m.zcol()) &&
				(wcol() == m.wcol());
	}

	/** '!=' operator. Tests two matrices for in-equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if just one corresponding pair of elements of the two 
		matrices is further than EPSILON<T>() apart (absolute value), 
		false otherwise. */
	bool_t	operator !=(const_reference m			///< [in] Right operand.    
					   ) const
	{
		return	(xcol() != m.xcol()) || 
				(ycol() != m.ycol()) ||
				(zcol() != m.zcol()) ||
				(wcol() != m.wcol());
	}


	// -------------------------
	// --- Compare functions ---
	// -------------------------
	

	/** Equal with exact comparision. Tests two matrices for equality.
		No epsilon value is used. Use with care when comparing floating point 
		type values with this function. 
		\return True if all corresponding elements of the two matrices are exactly
		equal, false otherwise. */
	bool_t		equal_exact(const_reference m)		///< [in] Matrix to compare with
	{
		return	xcol().equal_exact(m.xcol()) && 
				ycol().equal_exact(m.ycol()) &&
				zcol().equal_exact(m.zcol()) &&
				wcol().equal_exact(m.wcol());
	}


	/** Equal with custom epsilon comparision. Tests two vectors for equality.
		Supplied epsilon value is used. 
		\return True if all corresponding elements of the two vectors is no further,
		than epsilon apart (absolute value), false otherwise.. */
	bool_t		equal_epsilon(const_reference m,	///< [in] Matrix to compare with
							  const T epsilon		///< [in] Epsilon value to use when comparing
							 ) const
	{
		return	xcol().equal_epsilon(m.xcol(), epsilon) && 
				ycol().equal_epsilon(m.ycol(), epsilon) &&
				zcol().equal_epsilon(m.zcol(), epsilon) &&
				wcol().equal_epsilon(m.wcol(), epsilon);
	}



	// -----------------------
	// --- Debug Functions ---
	// -----------------------

	/// Convert matrix to a string for debug print.
	std::string			str(const int_t iDecimals= -1	///< [in] Number of decimals
						   ) const
	{
		std::string s = get_xrow().str(iDecimals);
			s += "\n" + get_yrow().str(iDecimals);
			s += "\n" + get_zrow().str(iDecimals);
			s += "\n" + get_wrow().str(iDecimals);
		return s;
	}


	// -------------------
	// --- Member Data ---
	// -------------------
private:
	union 
	{
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

// ----------------------------------------------
// --- Initialization Of Static Const Members ---
// ----------------------------------------------

template <class T>
const m44<T> m44<T>::M_ZERO	=  m44<T>(T(0),T(0),T(0),T(0), T(0),T(0),T(0),T(0), T(0),T(0),T(0),T(0), T(0),T(0),T(0),T(0));

template <class T>
const m44<T> m44<T>::M_I	=  m44<T>(T(1),T(0),T(0),T(0), T(0),T(1),T(0),T(0), T(0),T(0),T(1),T(0), T(0),T(0),T(0),T(1));


// -----------------
// --- Operators ---
// -----------------

// '*' operator, Scalar - matrix multiplication. Scales the matrix with a scalar. 
template <typename T>
inline m44<T>	operator *(	const T fVal, 		// [in] Left Hand Side operand (scalar).
							const m44<T>& m)	// [in] Right hand side operand matrix. 
{
	return m*fVal;	// Implement using operator m44<T> * T
}

/** '*' operator. Vector - Matrix multiplication. v4 * m44. (Vector is 
	transposed before multiplication)*/
template <typename T>
inline v4<T>	operator *(	const v4<T>& v,		///< [in] Left hand side operand (v4).
							const m44<T>& m)	///< [in] Right hand side operand (m44).
{
	return v4<T>( m.xcol()*v, m.ycol()*v, m.zcol()*v, m.wcol()*v);
}


// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write matrix to stream. 
	\return Reference to (modified) ostream. */
template <typename T>
inline std::ostream&		operator <<(std::ostream& os,	///< [out] Output stream to write to.
										const m44<T>& m)	///< [in] Matrix to write.
{
	os<<m.xcol()<<m.ycol()<<m.zcol()<<m.wcol();
	return os;
}

/** '>>' operator. Read input from stream. */
template <typename T>
inline std::istream&		operator >>(std::istream& is,	///< [in]  Input stream to read from.	
										m44<T>& m)			///< [out] Matrix recieve the data from the stream.
{
	is>>m.xcol()>>m.ycol()>>m.zcol()>>m.wcol();
	return is;
}

typedef m44<float>	m44f;
typedef m44<int>    m44i;


}


