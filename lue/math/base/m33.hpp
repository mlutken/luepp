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
//#include <math/base/lue_math_internal_utils.hpp>

#include "v3.hpp"
#include "m22.hpp"


namespace lue::math {



/** 
3D matrix class. XXX
Matrix layout:
	column, row

	m0,0	m1,0	m2,0	=	mx,x	my,x	mz,x
	m0,1	m1,1	m2,1	=	mx,y	my,y	mz,y
	m0,2	m1,2	m2,2	=	mx,z	my,z	mz,z
*/
template <typename T>
class m33
{
public:	

	// ----------------
	// --- Typedefs ---
	// ----------------
	typedef m33<T>					value_type;
	typedef value_type&				reference;
	typedef const value_type&		const_reference;
	typedef value_type*				pointer;
	typedef const value_type*		const_pointer;
	typedef unsigned_int_t			index_type;

	// --- vector typedefs ---
	typedef v3<T>					v3_value_type;
	typedef v3_value_type&			v3_reference;
	typedef const v3_value_type&	v3_const_reference;

	// --- m22 typedefs ---
	typedef m22<T>					m22_value_type;
	typedef m22_value_type&			m22_reference;
	typedef const m22_value_type&	m22_const_reference;
	
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
	m33() {}

	/** Copy constructor. Initializing with a general m33<T>.*/
	template <typename T1>
	m33(const m33<T1>& m)	
//		: 	m_Cols[X])(static_cast<v3_value_type>(m.xcol())), 
//			m_Cols[Y](static_cast<v3_value_type>(m.ycol())),
//			m_Cols[Z](static_cast<v3_value_type>(m.zcol())) {}
	{
		m_Cols[X]	= m.xcol(); m_Cols[Y]	= m.ycol(); m_Cols[Z]	= m.zcol();
	}

	/** Constructor with single value initialization. */
	explicit m33(const T fVal) 			///< [in] All elements are set to this value.
//		: m_Cols[X](fVal), m_Cols[Y](fVal), m_Cols[Z](fVal) {}	
	{
		m_Cols[X].set(fVal); m_Cols[Y].set(fVal); m_Cols[Z].set(fVal);
	}


	/** Constructor with 3 vector initialization. */
	m33(v3_const_reference colX,		///< [in] X Column of matrix
		v3_const_reference colY,		///< [in] Y Column of matrix
		v3_const_reference colZ)		///< [in] Z Column of matrix
//		:	m_Cols[X](colX), m_Cols[Y](colY), m_Cols[Z](colZ) {}	
	{
		m_Cols[X] = colX; m_Cols[Y]	= colY; m_Cols[Z] = colZ;
	}

 
	/** Constructor with 9 values initialization. */
	m33	(	const T fxx,			///< [in] Column-X, X component 
			const T fxy,			///< [in] Column-X, Y component
			const T fxz,			///< [in] Column-X, Z component
			const T fyx,			///< [in] Column-Y, X component
			const T fyy,			///< [in] Column-Y, Y component 
			const T fyz,			///< [in] Column-Y, Z component
			const T fzx,			///< [in] Column-Z, X component
			const T fzy,			///< [in] Column-Z, Y component 
			const T fzz)			///< [in] Column-Z, Z component
//		:	m_Cols[X](fxx, fxy, fxz), 
//			m_Cols[Y](fyx, fyy, fyz), 
//			m_Cols[Z](fzx, fzy, fzz)	{}	
	{
		m_Cols[X].set(fxx, fxy, fxz); 
		m_Cols[Y].set(fyx, fyy, fyz); 
		m_Cols[Z].set(fzx, fzy, fzz);
	}

	// ----------------------------
	// --- Set values functions ---
	// ----------------------------

	/** Set all fields to same value. */
	void				set(const T fVal)				///< [in] All elements are set to this value.
	{
		xcol().set(fVal);
		ycol().set(fVal);
		zcol().set(fVal);
	}

	/** Set values with 3 vectors. */
	void				set(v3_const_reference colX,	///< [in] Column-X of matrix
							v3_const_reference colY,	///< [in] Column-Y of matrix
							v3_const_reference colZ)	///< [in] Column-Z of matrix
	{
		xcol() = colX;
		ycol() = colY;
		zcol() = colZ;
	}

	/** Set values with 9 numbers. */
	void				set(const T fxx,				///< [in] Column-X, X component 
							const T fxy,				///< [in] Column-X, Y component
							const T fxz,				///< [in] Column-X, Z component
							const T fyx,				///< [in] Column-Y, X component
							const T fyy,				///< [in] Column-Y, Y component 
							const T fyz,				///< [in] Column-Y, Z component
							const T fzx,				///< [in] Column-Z, X component
							const T fzy,				///< [in] Column-Z, Y component 
							const T fzz)				///< [in] Column-Z, Z component
	{
		xcol().set(fxx, fxy, fxz);
		ycol().set(fyx, fyy, fyz);
		zcol().set(fzx, fzy, fzz);	
	}

	// -----------------------
	// --- Access elements ---
	// -----------------------

	/** [] Operator. Acces individual columns of the matrix.*/
	v3_reference			operator[](index_type i)	///< [in] Index value. Should be in range 0..2 ~ X, Y, Z.
	{	return m_Cols[i]; }
	v3_const_reference		operator[](index_type i) const
	{	return m_Cols[i]; }

	/** Access column X. Use this function to get acces to the X-column of the matrix. 
		\return Reference to coloumn X as a vector. */
	v3_reference		xcol()			{ return m_Cols[X]; }
	v3_const_reference	xcol()	const	{ return m_Cols[X]; }

	/** Access column Y. Use this function to get acces to the Y-column of the matrix. 
		\return Reference to coloumn Y as a vector. */
	v3_reference		ycol()			{ return m_Cols[Y]; }
	v3_const_reference	ycol()	const	{ return m_Cols[Y]; }

	/** Access column Z. Use this function to get acces to the Z-column of the matrix. 
		\return Reference to coloumn Z as a vector. */
	v3_reference		zcol()			{ return m_Cols[Z]; }
	v3_const_reference	zcol()	const	{ return m_Cols[Z]; }

	// ------------------------------------
	// --- Simple Set And Get Functions ---
	// ------------------------------------

	/** Get copy of X-row. Use this function to get a copy of the X-row of the matrix. 
		\return Copy of row X as a vector. */
	v3_value_type		get_xrow()	const	
	{ 	
		return v3_value_type(xcol().x(), ycol().x(), zcol().x()); 
	}
	
	/** Get copy of Y-row. Use this function to get a copy of the Y-row of the matrix. 
		\return Copy of row Y as a vector. */
	v3_value_type		get_yrow()	const	
	{ 	
		return v3_value_type(xcol().y(), ycol().y(), zcol().y()); 
	}

	/** Get copy of Z-row. Use this function to get a copy of the Z-row of the matrix. 
		\return Copy of row Z as a vector. */
	v3_value_type		get_zrow()	const	
	{ 	
		return v3_value_type(xcol().z(), ycol().z(), zcol().z()); 
	}

	/// Set X-row to vector. Assigns input vector to X-row of matrix.
	void				set_xrow(v3_const_reference v)		///< [in] Vector to assign to X-row	
	{ 
		xcol().x() = v.x(); 
		ycol().x() = v.y(); 
		zcol().x() = v.z(); 
	}

	/// Set Y-row to vector. Assigns input vector to Y-row of matrix.
	void				set_yrow(v3_const_reference v)		///< [in] Vector to assign to Y-row	
	{ 
		xcol().y() = v.x(); 
		ycol().y() = v.y(); 
		zcol().y() = v.z(); 
	}

	/// Set Z-row to vector. Assigns input vector to Z-row of matrix.
	void				set_zrow(v3_const_reference v)		///< [in] Vector to assign to Z-row	
	{ 
		xcol().z() = v.x(); 
		ycol().z() = v.y(); 
		zcol().z() = v.z(); 
	}

	// ----------------------------
	// --- Assignment operators ---
	// ----------------------------

	/** '=' operator, Assignment from another m33.*/
	reference			operator =(const_reference m)		///< [in] Right hand side operand.
	{
		xcol() = m.xcol(); 
		ycol() = m.ycol(); 
		zcol() = m.zcol(); 
		return *this;
	}

	// ----------------------------
	// --- Misc. math operators ---
	// ----------------------------

	/** Unary '-' operator, Negate matrix.
		\return Matrix holding the result of the negation. */
	value_type		operator -() const
	{
		return value_type(-xcol(), -ycol(), -zcol());
	}

	/** '/=' operator, Divides the elements of the vector with a scalar. 
		\return Reference to "this".*/
	reference			operator /=(const T fVal)		///< [in] Right hand side operand. Dividend used to scale the vector.
	{
		T fV = T(1) / fVal;
		xcol() *= fV;
		ycol() *= fV;
		zcol() *= fV;
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
		return *this;
	}


	/** '-=' operator, Substract a matrix from this matrix.
		\return Reference to "this".*/
	reference			operator -=(const_reference m)		///< [in] Right hand side operand.
	{
		xcol() -= m.xcol(); 
		ycol() -= m.ycol(); 
		zcol() -= m.zcol(); 
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
			zcol() + m.zcol() );
	}

	/** '-' operator, Matrix - Matrix substraction. m33 - m33. 
		\return Matrix holding the result of the substraction. */
	value_type		operator -(	const_reference m		///< [in] Right hand side operand (m33).
							  )const
	{
		return value_type(
			xcol() - m.xcol(),
			ycol() - m.ycol(),
			zcol() - m.zcol() );
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
		return *this;
	}

	/** '*' operator, Matrix - Scalar multiplication. 
		Scales matrix with a scalar, like this: matrix * number.
		\return Matrix holding the result of the scaling. */
	value_type		operator *(const T fVal			///< [in] Right hand side operand scalar (T).
							  )const
	{
		return value_type( xcol()*fVal, ycol()*fVal , zcol()*fVal );
	}


	/** '*' operator. Matrix - Vector multiplication. Performs m33 * v3. */
	v3_value_type	operator *(	v3_const_reference v)	///< [in] Right hand side operand (v3).
	{
		return v3_value_type(	xcol().x()*v.x() + ycol().x()*v.y() + zcol().x()*v.z(),
								xcol().y()*v.x() + ycol().y()*v.y() + zcol().y()*v.z(),
								xcol().z()*v.x() + ycol().z()*v.y() + zcol().z()*v.z());
	// \ML: This is not fast enough :-(	return v3_value_type(	get_xrow()*v, get_yrow()*v, get_zrow()*v );
	}


	/** '*' operator. Matrix - Matrix multiplication. m33 * m33. */
	value_type		operator *(const_reference m 	///< [in] Right hand side operand (m33).
							  ) const
	{
		return value_type(	
			xcol().x() * m.xcol().x() + ycol().x() * m.xcol().y() + zcol().x() * m.xcol().z(),
			xcol().y() * m.xcol().x() + ycol().y() * m.xcol().y() + zcol().y() * m.xcol().z(),
			xcol().z() * m.xcol().x() + ycol().z() * m.xcol().y() + zcol().z() * m.xcol().z(),

			xcol().x() * m.ycol().x() + ycol().x() * m.ycol().y() + zcol().x() * m.ycol().z(),
			xcol().y() * m.ycol().x() + ycol().y() * m.ycol().y() + zcol().y() * m.ycol().z(),
			xcol().z() * m.ycol().x() + ycol().z() * m.ycol().y() + zcol().z() * m.ycol().z(),

			xcol().x() * m.zcol().x() + ycol().x() * m.zcol().y() + zcol().x() * m.zcol().z(),
			xcol().y() * m.zcol().x() + ycol().y() * m.zcol().y() + zcol().y() * m.zcol().z(),
			xcol().z() * m.zcol().x() + ycol().z() * m.zcol().y() + zcol().z() * m.zcol().z()
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
									const T zz)		///< [in] zz component
	{
		set(xx, T(0), T(0), T(0), yy, T(0), T(0), T(0), zz);
	}


	/** Sets the matrix to a diagonal matrix using the vector specified.
		The rest of the elements are set to zero. */	
	void			load_diagonal(v3_const_reference v)		///< [in] Vector to assign to diagonal
	{
		set(v.x(), T(0), T(0), T(0), v.y(), T(0), T(0), T(0), v.z());
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
		std::swap(m_Cols[X].m_d[Z], m_Cols[Z].m_d[X]);
		std::swap(m_Cols[Y].m_d[Z], m_Cols[Z].m_d[Y]);
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
		return value_type(get_xrow(), get_yrow(), get_zrow());
	}

	// -----------------------------
	// --- Load matrix functions ---
	// -----------------------------

	/** Look at direction. Creates an ortho-normal matrix using the input vector 
		as Z-axis.The Y-Axis unit vector (0,1,0) is assumed to be 'up' when doing the 
		cross-products to make the matrix. Same as OpenGL 'gluLookAt'. */
	void				create_rot(v3_const_reference vDir)		///< [in] Direction of Z-axis in new matrix
	{
		zcol() = vDir;
		if ( zcol().normalize() < constants<T>::epsilon() ) 
		{
			zcol() = v3_value_type::V_Z;
		}

		v3_value_type v1(v3_value_type::V_Y);
		if ( Abs( zcol()*v1 ) > (T(1)-constants<T>::epsilon()) ) 
		{
			v1 = v3_value_type::V_X;
		}

		xcol() = v1.cross(zcol());
		xcol().normalize();
		ycol() = zcol().cross(xcol());
	}

	/** Look at direction. Creates an ortho-normal matrix using the first input 
		vector as Z-axis. The second input vector is used as the 'up' vector when doing the 
		cross-products to make the matrix. Same as OpenGL 'gluLookAt'. */
	void				create_rot(	v3_const_reference vDir,	///< [in] Direction of Z-axis in new matrix
									v3_const_reference vUp)		///< [in] Direction to use as up vector
	{
		zcol() = vDir;
		if ( zcol().normalize() < constants<T>::epsilon() ) 
		{
			zcol() = v3_value_type::V_Z;
		}

		if ( Abs( zcol()*vUp ) > (T(1)-constants<T>::epsilon()) ) 
		{
			zcol() = v3_value_type::V_Y;
		}
		xcol() = vUp.cross(zcol());
		xcol().normalize();
		ycol() = zcol().cross(xcol());
	}

	/** Look at direction rotated. Creates an ortho-normal matrix using the input vector 
		as Z-axis. The angle given is used to rotate around the direction vector. */
	void			create_rot(	v3_const_reference v, 	///< [in] Direction
								const T fAngle )		///< [in] Angle (radians) to rotate around direction 
	{
		T C = (T) Cos(fAngle), S = (T) Sin(fAngle);
		T x2 = v.x()*v.x(), y2 = v.y()*v.y(), z2 = v.z()*v.z();
		T xs = v.x()*S, ys = v.y()*S, zs = v.z()*S;
		T xyOneMinusC = v.x()*v.y()*(T(1)-C);
		T zxOneMinusC = v.z()*v.x()*(T(1)-C);
		T yzOneMinusC = v.y()*v.z()*(T(1)-C);

		m_Cols[X][X] = x2 + C*(T(1)-x2);
		m_Cols[Y][X] = xyOneMinusC + zs;
		m_Cols[Z][X] = zxOneMinusC - ys;
		
		m_Cols[X][Y] = xyOneMinusC - zs;
		m_Cols[Y][Y] = y2 + C*(T(1)-y2);
		m_Cols[Z][Y] = yzOneMinusC + xs;
		
		m_Cols[X][Z] = zxOneMinusC + ys;
		m_Cols[Y][Z] = yzOneMinusC - xs;
		m_Cols[Z][Z] = z2 + C*(T(1)-z2);
	}


	// -------------------------------------
	// --- Determinant, Adjoint, Inverse ---
	// -------------------------------------

	/** Finds the determinant of the matrix.
		\return Determinant of matrix. */
	T					det() const
	{
		const T det_x = ycol().y()*zcol().z() - ycol().z()*zcol().y();
		const T det_y = xcol().y()*zcol().z() - xcol().z()*zcol().y();
		const T det_z = xcol().y()*ycol().z() - xcol().z()*ycol().y();
		return xcol().x()*det_x - ycol().x()*det_y + zcol().x()*det_z;
	}

	/** Calculates the adjoint af the matrix. Same as (adjugate matrix or conjugate transpose ).
		\return The adjoint matrix.*/
	value_type			adjoint() const
	{
		const T Axx	= ycol().y()*zcol().z() - ycol().z()*zcol().y();
		const T Axy	= zcol().y()*xcol().z() - zcol().z()*xcol().y();
		const T Axz	= xcol().y()*ycol().z() - xcol().z()*ycol().y();
		
		const T Ayx	= zcol().x()*ycol().z() - zcol().z()*ycol().x();
		const T Ayy	= xcol().x()*zcol().z() - xcol().z()*zcol().x();
		const T Ayz	= ycol().x()*xcol().z() - ycol().z()*xcol().x();

		const T Azx	= ycol().x()*zcol().y() - ycol().y()*zcol().x();
		const T Azy	= zcol().x()*xcol().y() - zcol().y()*xcol().x();
		const T Azz	= xcol().x()*ycol().y() - xcol().y()*ycol().x();
		return value_type(Axx, Axy, Axz,  Ayx, Ayy, Ayz,  Azx, Azy, Azz);
	}
	

	/** Finds the inverse of this matrix. Ie. finds the inverse if possible. 
		Ie. if the determinant is non-zero (det() != 0). 
		No execptions are trown in case det() == 0. If one's unsure whether det() != 0
		Should call first calculate Det= m.det() and only if Det != 0 call inverse 
		with Det as argument.
		\return The inverse matrix if determinant is NOT zero. Otherwise the 
				unchanged matrix is returned.*/
	value_type			get_inverse(T Det = -1		///< [in] Determinant of this matrix. If Det is -1 then Det is calculated (Det = det())
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
	bool	operator ==(const_reference m			///< [in] Right operand.    
					   ) const
	{
		return	(xcol() == m.xcol()) && 
				(ycol() == m.ycol()) &&
				(zcol() == m.zcol());
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
				(ycol() != m.ycol()) ||
				(zcol() != m.zcol());
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
				ycol().equal_exact(m.ycol()) &&
				zcol().equal_exact(m.zcol());
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
				ycol().equal_epsilon(m.ycol(), epsilon) &&
				zcol().equal_epsilon(m.zcol(), epsilon);
	}

	// ----------------------------------
	// --- 2D Homogeneous coordinates ---
	// ----------------------------------

	
	/** Get 2D rotation matrix when using homogeneous coordinates.
		When using a 3D matrix to represent 2D transformations this function can 
		be used to get the 2D rotation matrix (upper left 2x2 of the 3x3 matrix).
		\return  Upper left 2x2 matrix representing the 2D rotation.*/
	m22_value_type		m22_rot() const
	{
		return	m22_value_type(xcol().x(), xcol().y(), ycol().x(), ycol().y() );
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
			s += "\n" + get_zrow().str(iDecimals);
		return s;
	}



	// -------------------
	// --- Member Data ---
	// -------------------
private:
	v3_value_type m_Cols[3];

};

// ----------------------------------------------
// --- Initialization Of Static Const Members ---
// ----------------------------------------------

template <class T>
const m33<T> m33<T>::M_ZERO	=  m33<T>(T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0));

template <class T>
const m33<T> m33<T>::M_I	=  m33<T>(T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1));


// -----------------
// --- Operators ---
// -----------------

// '*' operator, Scalar - matrix multiplication. Scales the matrix with a scalar. 
template <typename T>
inline m33<T>	operator *(	const T fVal, 		// [in] Left Hand Side operand (scalar).
							const m33<T>& m)	// [in] Right hand side operand matrix. 
{
	return m*fVal;	// Implement using operator m33<T> * T
}

/** '*' operator. Vector - Matrix multiplication. v3 * m33. (Vector is 
	transposed before multiplication)*/
template <typename T>
inline v3<T>	operator *(	const v3<T>& v,		///< [in] Left hand side operand (v3).
							const m33<T>& m)	///< [in] Right hand side operand (m33).
{
	return v3<T>( m.xcol()*v, m.ycol()*v, m.zcol()*v);
}


// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write matrix to stream. 
	\return Reference to (modified) ostream. */
template <typename T>
inline std::ostream&		operator <<(std::ostream& os,	///< [out] Output stream to write to.
										const m33<T>& m)	///< [in] Matrix to write.
{
	os<<m.xcol()<<m.ycol()<<m.zcol();
	return os;
}

/** '>>' operator. Read input from stream. */
template <typename T>
inline std::istream&		operator >>(std::istream& is,	///< [in]  Input stream to read from.	
										m33<T>& m)			///< [out] Matrix recieve the data from the stream.
{
	is>>m.xcol()>>m.ycol()>>m.zcol();
	return is;
}

typedef m33<float>	m33f;
typedef m33<int>    m33i;

}

