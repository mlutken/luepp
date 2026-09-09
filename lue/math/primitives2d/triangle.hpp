#pragma once
// ****************************************************************************
// CREATED_ON	: 26-09-05 
// CREATED_BY	: Martin Lutken
// ---------------------------------------------------------------------------- 
// COPYRIGHT	: 
// ****************************************************************************


#include "../../_tmath_internal_utils.hpp"

#include "../v2.hpp"
#include "../g33.hpp"


namespace lue::math::p2d {

/// DOXYS_OFF

/** Triangle store by value policy. 
\todo Make a 6 values initialisation.*/
template <typename T>
class PTriangleStoreByValue
{
public:		

	typedef v2<T>					v2_storage_type;

	// --- vector typedefs ---
 	typedef v2<T>					v2_value_type;
 	typedef v2_value_type&			v2_reference;
 	typedef const v2_value_type&	v2_const_reference;

	/// Default constructor with no initialization.
	PTriangleStoreByValue() {}


	/// Copy constructor. 
	PTriangleStoreByValue(const PTriangleStoreByValue& t)	
		: 	m_vV0(t.m_vV0), m_vV1(t.m_vV1), m_vV2(t.m_vV2) {}


	/// Constructor with 3 points initialization. 
	PTriangleStoreByValue(	v2_storage_type vV0,		///< [in] Triangle point 0
							v2_storage_type vV1,		///< [in] Triangle point 1
							v2_storage_type vV2)		///< [in] Triangle point 2
		: 	m_vV0(vV0), m_vV1(vV1),	m_vV2(vV2)  {}

	// --- Access elements ---

	/// Access point 0 of the triangle. \return Reference to point 0 of the triangle. 
	v2_reference		v0()			{ return m_vV0; }
	v2_const_reference	v0() const		{ return m_vV0; }
	/// Access point 1 of the triangle. \return Reference to point 1 of the triangle. 
	v2_reference		v1()			{ return m_vV1; }
	v2_const_reference	v1() const		{ return m_vV1; }
	/// Access point 2 of the triangle. \return Reference to point 2 of the triangle. 
	v2_reference		v2()			{ return m_vV2; }
	v2_const_reference	v2() const		{ return m_vV2; }

private:	
	v2_storage_type			m_vV0;		///< Triangle point 0
	v2_storage_type			m_vV1;		///< Triangle point 1
	v2_storage_type			m_vV2;		///< Triangle point 2
};


/** Triangle store by pointer policy. */
template <typename T>
class PTriangleStoreByPointer
{
public:		

	typedef v2<T>*					v2_storage_type;

	// --- vector typedefs ---
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;

	/// Default constructor with no initialization. \important NOT supported !!!
	PTriangleStoreByPointer() {
		static_assert(0, class_triangle_using_PTriangleStoreByPointer_policy_does_NOT_support_default_constructor);
	}

	/// Copy constructor. \important NOT supported !!!
	PTriangleStoreByPointer(const PTriangleStoreByPointer& t) {
		static_assert(0, class_triangle_using_PTriangleStoreByPointer_policy_does_NOT_support_copy_constructor);
	}

	/// Constructor with 3 points initialization. 
	PTriangleStoreByPointer(v2_storage_type pvV0,		///< [in] Pointer to triangle point 0
							v2_storage_type pvV1,		///< [in] Pointer to triangle point 1
							v2_storage_type pvV2)		///< [in] Pointer to triangle point 2
		: 	m_pvV0(pvV0), m_pvV1(pvV1),	m_pvV2(pvV2)  {}

	// --- Access elements ---

	/// Access point 0 of the triangle. \return Reference to point 0 of the triangle. 
	v2_reference		v0()			{ return *m_pvV0; }
	v2_const_reference	v0() const		{ return *m_pvV0; }
	/// Access point 1 of the triangle. \return Reference to point 1 of the triangle. 
	v2_reference		v1()			{ return *m_pvV1; }
	v2_const_reference	v1() const		{ return *m_pvV1; }
	/// Access point 2 of the triangle. \return Reference to point 2 of the triangle. 
	v2_reference		v2()			{ return *m_pvV2; }
	v2_const_reference	v2() const		{ return *m_pvV2; }

private:	
	v2_storage_type			m_pvV0;		///< Pointer to triangle point 0
	v2_storage_type			m_pvV1;		///< Pointer to triangle point 1
	v2_storage_type			m_pvV2;		///< Pointer to triangle point 2
};



/** Triangle store by reference policy. */
template <typename T>
class PTriangleStoreByReference
{
public:		

	typedef  v2<T>&					v2_storage_type;

	// --- vector typedefs ---
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;

	/// Default constructor with no initialization. \important NOT supported !!!
	PTriangleStoreByReference() {
		static_assert(0, class_triangle_using_PTriangleStoreByReference_policy_does_NOT_support_default_constructor);
	}

	/// Copy constructor. \important NOT supported !!!
	PTriangleStoreByReference(const PTriangleStoreByReference& t)
	{
		static_assert(0, class_triangle_using_PTriangleStoreByReference_policy_does_NOT_support_copy_constructor);
	}

	/// Constructor with 3 points initialization. 
	PTriangleStoreByReference(v2_storage_type rvV0,		///< [in] Reference to triangle point 0
							  v2_storage_type rvV1,		///< [in] Reference to triangle point 1
							  v2_storage_type rvV2)		///< [in] Reference to triangle point 2
		: 	m_rvV0(rvV0), m_rvV1(rvV1),	m_rvV2(rvV2)  {}

	// --- Access elements ---

	/// Access point 0 of the triangle. \return Reference to point 0 of the triangle. 
	v2_reference		v0()			{ return m_rvV0; }
	v2_const_reference	v0() const		{ return m_rvV0; }
	/// Access point 1 of the triangle. \return Reference to point 1 of the triangle. 
	v2_reference		v1()			{ return m_rvV1; }
	v2_const_reference	v1() const		{ return m_rvV1; }
	/// Access point 2 of the triangle. \return Reference to point 2 of the triangle. 
	v2_reference		v2()			{ return m_rvV2; }
	v2_const_reference	v2() const		{ return m_rvV2; }

private:	
	v2_storage_type			m_rvV0;		///< Reference to triangle point 0
	v2_storage_type			m_rvV1;		///< Reference to triangle point 1
	v2_storage_type			m_rvV2;		///< Reference to triangle point 2
};

/// DOXYS_ON

/** 
Triangle (2D) defined by 3 points.
\image images/triangle.gif */
template
<
    typename T,
    template <class> class StoragePolicy = PTriangleStoreByValue
>
class triangle
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	
	typedef triangle<T, StoragePolicy >		value_type;
	typedef value_type&						reference;
	typedef const value_type&				const_reference;
	typedef value_type*						pointer;
	typedef const value_type*				const_pointer;

	/// For operators and functions returning an object of type triangle.
	/// Since we get temporary object created this way we need these temporarys 
	/// to have it's own store for the whole triangle.
	typedef triangle<T, PTriangleStoreByValue>	value_value_type;	
	
	// --- v2 storage type typedefs ---
	typedef typename StoragePolicy<T>::v2_storage_type  v2_storage_type;

	// --- vector typedefs ---
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;

	// --- m22 typedefs ---
	typedef m22<T>					m22_value_type;
	typedef m22_value_type&			m22_reference;
	typedef const m22_value_type&	m22_const_reference;

	// --- g33 typedefs ---
	typedef g33<T>					g33_value_type;
	typedef g33_value_type&			g33_reference;
	typedef const g33_value_type&	g33_const_reference;



	// --------------------------------------------
	// --- Constructors / Set values functions. ---
	// --------------------------------------------

	/// Default constructor with no initialization. 
	triangle() : m_Data() {}


	/// Copy constructor. Initializing with another triangle.
	triangle(const_reference t) : m_Data(t.m_Data) {} 

	/** Constructor with 3 points initialization. */
	triangle(	v2_storage_type vV0,		///< [in] Triangle point 0
				v2_storage_type vV1,		///< [in] Triangle point 1
				v2_storage_type vV2)		///< [in] Triangle point 2
		: 	m_Data(vV0, vV1, vV2) {}

	
	/// Cast to triangle with different number-type 'T'.
	template <typename T1>
	operator triangle<T1>() const 
	{
		typedef triangle<T1> triangle_value_type;
		return triangle_value_type( v0(), v1(), v2() );
	}

	// ------------------------
	// --- Access functions ---
	// ------------------------
	
	/// Access point 0 of the triangle. \return Reference to point 0 of the triangle. 
 	v2_reference		v0()			{ return m_Data.v0(); }
 	v2_const_reference	v0()	const	{ return m_Data.v0(); }

	/// Access point 1 of the triangle. \return Reference to point 1 of the triangle. 
 	v2_reference		v1()			{ return m_Data.v1(); }
 	v2_const_reference	v1()	const	{ return m_Data.v1(); }

	/// Access point 2 of the triangle. \return Reference to point 2 of the triangle. 
 	v2_reference		v2()			{ return m_Data.v2(); }
 	v2_const_reference	v2()	const	{ return m_Data.v2(); }

	
	// ----------------------------
	// --- Set values functions ---
	// ----------------------------

	/** Set triangle with 3 points. */
	void		set(v2_const_reference vV0,		///< [in] Triangle point 0
					v2_const_reference vV1,		///< [in] Triangle point 1
					v2_const_reference vV2)		///< [in] Triangle point 2
	{ 	
		v0() = vV0;
		v1() = vV1;
		v2() = vV2; 
	}


	/** Set triangle with 6 values. */
	void		set(T V0x,			///< [in] Triangle point 0, X-coordinate.
					T V0y,			///< [in] Triangle point 0, Y-coordinate.
					T V1x,			///< [in] Triangle point 1, X-coordinate.
					T V1y,			///< [in] Triangle point 1, Y-coordinate.
					T V2x,			///< [in] Triangle point 2, X-coordinate.
					T V2y)			///< [in] Triangle point 2, Y-coordinate.
	{
		v0().set(V0x, V0y);
		v1().set(V1x, V1y);
		v2().set(V2x, V2y);
	}

		

	// ----------------------------
	// --- Assignment operators ---
	// ----------------------------

	/** '=' operator, Assignment from another triangle.
		\note Here we allways copy values to destination. In case this triangle does not
			  use the PTriangleStoreByValue - then it is still values that are copied - not 
			  pointers or references, that are assigned. */
	template < template <class> class StorPolicy >
	reference			operator =(const triangle<T, StorPolicy>& t)		///< [in] Right hand side operand.
	{
		v0() = t.v0();
		v1() = t.v1();
		v2() = t.v2();
		return *this;
	}

	// ------------------------------------
	// --- Add/Substract math operators ---
	// ------------------------------------

	/** '+=' operator, Add vector to triangle. Translate triangle to other position. 
		\return Reference to "this".*/
	reference			operator +=(v2_value_type v)		///< [in] Vector to add to all points.
	{
		v0() += v;
		v1() += v;
		v2() += v;
		return *this;
	}


	/** '-=' operator, Subtract vector from triangle. Translate triangle to other position. 
		\return Reference to "this".*/
	reference			operator -=(v2_value_type v)		///< [in] Vector to subtract to all points.
	{
		v0() -= v;
		v1() -= v;
		v2() -= v;
		return *this;
	}

	/** '+' operator, Add vector to triangle. Translate triangle to other position.  
		\return triangle translated to the new position. */
	value_value_type	operator +(	v2_const_reference v		///< [in] Vector to add to all points.
							  )const
	{
		return value_value_type( v0() + v, v1() + v, v2() + v);
	}

	/** '-' operator, Subtract vector from triangle. Translate triangle to other position.  
		\return triangle translated to the new position. */
	value_value_type	operator -(	v2_const_reference v		///< [in] Vector to subtract to all points.
							  )const
	{
		return value_value_type( v0() - v, v1() - v, v2() - v);
	}


	// -------------------------------------
	// --- Multiplication math operators ---
	// -------------------------------------

	/** '*=' operator, Scales the triangle with a scalar. (I.e. multiplying the 
		points with a scalar.) 
		\return Reference to "this".*/
	reference			operator *=(T Val)		///< [in] Right hand side operand. Scalar used to scale the vector.
	{
		v0() *= Val;
		v1() *= Val;
		v2() *= Val;
		return *this;
	}

	/** '*' operator, Triangle - Scalar multiplication. 
		Scales triangle with a scalar, like this: matrix * number.
		\return Triangle holding the result of the scaling. */
	value_value_type	operator *(T Val			///< [in] Right hand side operand scalar (T).
								  ) const
	{
		return value_value_type( v0()*Val, v1()*Val, v2()*Val );
	}

	/** '*' operator. Triangle - Matrix (m22) multiplication. triangle * m22. Typically 
			used to rotate the triangle. */
	value_value_type	operator *(m22_const_reference M22 	///< [in] Right hand side operand (m22).
							  ) const
	{
		return value_value_type( v0()*M22, v1()*M22, v2()*M22() );
	}

	/** '*' operator. Triangle - Matrix (g33) multiplication. triangle * g33. Typically 
			used to transform the triangle. */
	value_value_type	operator *(g33_const_reference G33 	///< [in] Right hand side operand (g33).
							  ) const
	{
		return value_value_type( v0()*G33, v1()*G33, v2()*G33() );
	}


	// ----------------------
	// --- Util functions ---
	// ----------------------

	/** Get vector from triangle point 0 to point 1 \return Vector from point 0 to point 1. */
	v2_value_type			vec01() const { return v1() - v0();}

	/** Get vector from triangle point 1 to point 0 \return Vector from point 1 to point 0. */
	v2_value_type			vec10() const { return v0() - v1();}

	/** Get vector from triangle point 0 to point 2 \return Vector from point 0 to point 2. */
	v2_value_type			vec02() const { return v2() - v0();}

	/** Get vector from triangle point 2 to point 0 \return Vector from point 2 to point 0. */
	v2_value_type			vec20() const { return v0() - v2();}

	/** Get vector from triangle point 1 to point 2 \return Vector from point 1 to point 2. */
	v2_value_type			vec12() const { return v2() - v1();}

	/** Get vector from triangle point 2 to point 1 \return Vector from point 2 to point 1. */
	v2_value_type			vec21() const { return v1() - v2();}


	/** Get area (signed) of triangle. The area of the triangle is returned with sign:
	
	#Note:# That the signed area will be positive if V0V0V1  are oriented counterclockwise 
	around the triangle, and will be negative if the triangle is oriented clockwise; and so, 
	this area computation can be used to test for a triangle's orientation.  
	The signed area can also be used to test whether the point V1 is to the left (positive) 
	or the right (negative) of the directed line segment V0V0  .  
	
	!Implementation details
	In 2 dimensional space (2D), a vector can be viewed as embedded in 3D by adding a third 
	component which is set = 0.  This lets one take the cross-product of 2D vectors, and use 
	it to compute area.  Given a triangle with vertices Vi=(xi,yi)=(xi,yi,0) for i=0,2, 
	we can compute that:
	
	\image images/triangle_area_eqn_5a.gif
	And the absolute value of the third z-component is twice the absolute area of the triangle.  
	However, it is useful to not take the absolute value here, and instead let the area be a 
	signed quantity.

	\image images/triangle_area_eqn_5b.gif
	This formula for area is a very efficient computation with no roots or trigonometric 
	functions involved - just 2 multiplications and 5 additions, and 1 division by 2.
	
	\see http://softsurfer.com/Archive/algorithm_0101/algorithm_0101.htm (Modern Triangles)
	\return Signed area of triangle. */
	T				area() const 
	{
		return ( ( v1().x() - v0().x() )*( v2().y() - v0().y() ) - ( v2().x() - v0().x() )*(v1().y() - v0().y()) ) / 2;
	}	

	/** Point inside. 
	\todo Implement this function!!
	\return True if point given is inside the triangle. */
	bool				point_inside(v2_const_reference v	///< [in] Point to test
									) const
	{
		return false; 
	}	

	// -------------------------
	// --- Compare operators ---
	// -------------------------

	/** '==' operator. Tests two triangles for equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if all corresponding elements of the two triangles is no further,
		than EPSILON<T>() apart (absolute value), false otherwise. */
	bool	operator ==(const_reference t			///< [in] Right operand.    
					   ) const
	{
		return	(v0() == t.v0()) && 
				(v1() == t.v1()) &&
				(v2() == t.v2());
	}

	/** '!=' operator. Tests two triangles for in-equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if just one corresponding pair of elements of the two 
		triangles is further than EPSILON<T>() apart (absolute value), 
		false otherwise. */
	bool	operator !=(const_reference t			///< [in] Right operand.    
					   ) const
	{
		return	(v0() != t.v0()) ||
				(v1() != t.v1()) ||
				(v2() != t.v2());
	}


	// -------------------------
	// --- Compare functions ---
	// -------------------------
	

	/** Equal with exact comparision. Tests two triangles for equality.
		No epsilon value is used. Use with care when comparing floating point 
		type values with this function. 
		\return True if all corresponding elements of the two triangles are exactly
		equal, false otherwise. */
	bool		equal_exact(const_reference t)		///< [in] Triangle to compare with
	{
		return	v0().equal_exact(t.v0()) && 
				v1().equal_exact(t.v1()) &&
				v2().equal_exact(t.v2());
	}


	/** Equal with custom epsilon comparision. Tests two vectors for equality.
		Supplied epsilon value is used. 
		\return True if all corresponding elements of the two vectors is no further,
		than epsilon apart (absolute value), false otherwise.. */
	bool		equal_epsilon(const_reference t,	///< [in] Triangle to compare with
							  T epsilon				///< [in] Epsilon value to use when comparing
							 ) const
	{
		return	v0().equal_epsilon(t.v0(), epsilon) && 
				v1().equal_epsilon(t.v1(), epsilon) && 
				v2().equal_epsilon(t.v2(), epsilon);
	}


	// -----------------------
	// --- Debug Functions ---
	// -----------------------

	/// Convert triangle to a string for debug print.
	std::string			str( int iDecimals= -1	///< [in] Number of decimals
						   ) const
	{
		std::string s = "V0: " + v0().str(iDecimals);
					s += "\nV1: " + v1().str(iDecimals);
					s += "\nV2: " + v2().str(iDecimals);
		return s;
	}

private:
	// ------------
	// --- Data ---	
	// ------------
	StoragePolicy<T>			m_Data;
	
};


// -----------------
// --- Operators ---
// -----------------

// '*' operator, Scalar - triangle multiplication. Scales the triangle with a scalar. 
//template < typename T, template <class> class StoragePolicy >
template < typename T, template <class> class StoragePolicy >
inline triangle<T, PTriangleStoreByValue>	
	operator *(	T Val, 									// [in] Left Hand Side operand (scalar).
				const triangle<T, StoragePolicy>& t)	// [in] Right hand side operand triangle. 
{
	return t*Val;	// Implement using operator triangle * T
}

// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write triangle to stream. 
	\return Reference to (modified) ostream. */
template < typename T, template <class> class StoragePolicy >
inline std::ostream&		
	operator <<(std::ostream& os,						///< [out] Output stream to write to.
				const triangle<T, StoragePolicy>& t)	///< [in] Triangle to write.
{
	os<<t.v0()<<t.v1()<<t.v2();
	return os;
}

/** '>>' operator. Read triangle from input stream. */
template < typename T, template <class> class StoragePolicy >
inline std::istream&		
	operator >>(std::istream& is,				///< [in]  Input stream to read from.	
				triangle<T, StoragePolicy>& t)	///< [out] Triangle recieve the data from the stream.
{
	is>>t.v0()>>t.v1()>>t.v2();
	return is;
}


}

