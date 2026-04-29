#pragma once

#include "../../_tmath_internal_utils.hpp"

#include "../v2.hpp"
#include "../g33.hpp"


namespace lue::math::p2d {


// No need for storage policy classes here! We have only a scalar (radius).




/** 
2D circle class with center in origo. Circle defined just by a radius as the 
center is assumed to be in origo (0,0).  */
template
<
    typename T
>
class circle_origo
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	
	typedef circle_origo<T>			value_type;
	typedef value_type&				reference;
	typedef const value_type&		const_reference;
	typedef value_type*				pointer;
	typedef const value_type*		const_pointer;

	// --- vector typedefs ---
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;


	// --------------------------------------------
	// --- Constructors / Set values functions. ---
	// --------------------------------------------

	/// Default constructor with no initialization. 
	circle_origo() {}


	/// Copy constructor. Initializing with another circle.
	circle_origo(const_reference c) 			///< Circle to copy from.
		: m_Radius(c.m_Radius) {} 

	/** Constructor with radius initialization. */
	explicit circle_origo( T Radius )			///< [in] Circle radius
		: 	m_Radius(Radius) {}

	
	/// Cast to circle with different number-type 'T'.
	template <typename T1>
	operator circle_origo<T1>() const 
	{
		typedef circle_origo<T1> circle_value_type;
		return circle_value_type( radius() );
	}

	
	// ------------------------
	// --- Access functions ---
	// ------------------------
	

	/// Access radius of circle. \return Reference to radius of circle. 
 	T&					radius()			{ return m_Radius; }
 	T					radius()	const	{ return m_Radius; }
 	
 	/// Set radius of circle.
 	void				radius(T r) 		{ m_Radius = r; }

	

	// ----------------------------
	// --- Assignment operators ---
	// ----------------------------

	/** '=' operator, Assignment from another circle.
		\note Here we allways copy values to destination. In case this circle does not
			  use the PCircleStoreByValue - then it is still values that are copied - not 
			  pointers or references, that are assigned. */
	reference			operator =(reference c)		///< [in] Right hand side operand.
	{
		radius() = c.radius();
		return *this;
	}


	// ----------------------
	// --- Util functions ---
	// ----------------------

	
	/** Get area of circle.
		\return Area of circle, which is allways positive or zero. */
	T				area() const { return C<T>::PI()*radius()*radius(); }	

	/** Get circumference of circle.
		\return Circumference of circle. */
	T				circumference() const { return T(2)*C<T>::PI()*radius(); }	

	/** Get diameter of circle.
		\return Diameter of circle. */
	T				diameter() const { return T(2)*radius(); }	

	/** Point inside. 
	\return True if point given is inside the circle. */
	bool_t			point_inside(v2_const_reference p0	///< [in] Point to test
								) const
	{
		return ( p0.len2() <= radius()*radius() );
	}	

	// -------------------------
	// --- Compare operators ---
	// -------------------------

	/** '==' operator. Tests two circles for equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if all corresponding elements of the two circles is no further,
		than EPSILON<T>() apart (absolute value), false otherwise. */
	bool_t	operator ==(const_reference c			///< [in] Right operand.    
					   ) const
	{
		return	( Abs(radius() - c.radius()) < constants<T>::epsilon() ) ;
	}

	/** '!=' operator. Tests two circles for in-equality. 
		EPSILON<T>() is used as threshold to avoid numeric instabilities. Use 
		the 'equal_exact' member function if exact comparision is required. 
		\return True if just one corresponding pair of elements of the two 
		circles is further than EPSILON<T>() apart (absolute value), 
		false otherwise. */
	bool_t	operator !=(const_reference c			///< [in] Right operand.    
					   ) const
	{
		return	( Abs(radius() - c.radius()) > constants<T>::epsilon() ) ;
	}


	// -------------------------
	// --- Compare functions ---
	// -------------------------
	

	/** Equal with exact comparision. Tests two circles for equality.
		No epsilon value is used. Use with care when comparing floating point 
		type values with this function. 
		\return True if all corresponding elements of the two circles are exactly
		equal, false otherwise. */
	bool_t		equal_exact(const_reference c)		///< [in] Circle to compare with
	{
		return	( radius() == c.radius() ) ;
				
	}


	/** Equal with custom epsilon comparision. Tests two vectors for equality.
		Supplied epsilon value is used. 
		\return True if all corresponding elements of the two vectors is no further,
		than epsilon apart (absolute value), false otherwise. */
	bool_t		equal_epsilon(const_reference c,	///< [in] Circle to compare with
							  T epsilon				///< [in] Epsilon value to use when comparing
							 ) const
	{
		return	( Abs(radius() - c.radius()) < epsilon ) ;
	}


	// -----------------------
	// --- Debug Functions ---
	// -----------------------

	/// Convert circle to a string for debug print.
	std::string			str(int_t iDecimals= -1	///< [in] Number of decimals
						   ) const
	{
		std::string s = "Radius: " + _UTILS::str( radius(), iDecimals );
		return s;
	}

private:
	// ------------
	// --- Data ---	
	// ------------
	T					m_Radius;		///< Circle radius.
};


// -----------------
// --- Operators ---
// -----------------




// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write circle to stream. 
	\return Reference to (modified) ostream. */
template < typename T >
inline std::ostream&		
	operator <<(std::ostream& os,		///< [out] Output stream to write to.
				const circle<T>& c)		///< [in] Circle to write.
{
	os<<c.radius();
	return os;
}

/** '>>' operator. Read circle from input stream. */
template < typename T >
inline std::istream&		
	operator >>(std::istream& is,		///< [in]  Input stream to read from.	
				circle<T>& c)			///< [out] Circle recieve the data from the stream.
{
	is>>c.radius();
	return is;
}


}

