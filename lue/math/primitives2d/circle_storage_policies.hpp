#pragma once

#include "math/base/lue_math_internal_utils.hpp"

#include "math/base/v2.hpp"
#include "math/base/g33.hpp"


namespace lue::math::p2d {


/** Circle store by value policy. */
template <typename T>
class PCircleStoreByValue
{
public:		

	typedef v2<T>					v2_storage_type;

	// --- vector typedefs ---
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;

	/// Default constructor with no initialization.
	PCircleStoreByValue() {}


	/// Copy constructor. 
	PCircleStoreByValue(const PCircleStoreByValue& t)	
		: 	m_vCenter(t.m_vCenter) {}


	/// Constructor with center initialization. 
	PCircleStoreByValue(	v2_storage_type vCenter)	///< [in] Center of circle
		: 	m_vCenter(vCenter)  {}

	// --- Access elements ---

	/// Access center of circle. \return Reference to center of rectangle.
	v2_reference		center()			{ return m_vCenter; }
	v2_const_reference	center()	const	{ return m_vCenter; }

private:	
	v2_storage_type			m_vCenter;		///< Center of circle.
};



/** Circle store by pointer policy. */
template <typename T>
class PCircleStoreByPointer
{
public:		

	typedef v2<T>*					v2_storage_type;

	// --- vector typedefs ---
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;

	/// Default constructor with no initialization. \important NOT supported !!!
	PCircleStoreByPointer() {
		STATIC_CHECK(0, class_circle_using_PCircleStoreByPointer_policy_does_NOT_support_default_constructor);
	}

	/// Copy constructor. \important NOT supported !!!
	PCircleStoreByPointer(const PCircleStoreByPointer& t) {
		STATIC_CHECK(0, class_circle_using_PCircleStoreByPointer_policy_does_NOT_support_copy_constructor);
	}

	/// Constructor with center initialization. 
	PCircleStoreByPointer(v2_storage_type pvCenter)		///< [in] Pointer to circle center
		: 	m_pvCenter(pvCenter) {}

	// --- Access elements ---

	/// Access center of circle. \return Reference to center of rectangle.
	v2_reference		center()			{ return *m_pvCenter; }
	v2_const_reference	center()	const	{ return *m_pvCenter; }

private:	
	v2_storage_type			m_pvCenter;		///< Pointer to circle center
};



/** Circle store by reference policy. */
template <typename T>
class PCircleStoreByReference
{
public:		

	typedef v2<T>&					v2_storage_type;

	// --- vector typedefs ---
	typedef v2<T>					v2_value_type;
	typedef v2_value_type&			v2_reference;
	typedef const v2_value_type&	v2_const_reference;

	/// Default constructor with no initialization. \important NOT supported !!!
	PCircleStoreByReference() {
		STATIC_CHECK(0, class_circle_using_PCircleStoreByReference_policy_does_NOT_support_default_constructor);
	}

	/// Copy constructor. \important NOT supported !!!
	PCircleStoreByReference(const PCircleStoreByReference& t) {
		STATIC_CHECK(0, class_circle_using_PCircleStoreByReference_policy_does_NOT_support_copy_constructor);
	}

	/// Constructor with center initialization. 
	PCircleStoreByReference(v2_storage_type rvCenter)	///< [in] Reference to circle center
		: 	m_rvCenter(rvCenter)  {}

	// --- Access elements ---

	/// Access center of circle. \return Reference to center of rectangle.
	v2_reference		center()			{ return m_rvCenter; }
	v2_const_reference	center()	const	{ return m_rvCenter; }

private:	
	v2_storage_type			m_rvCenter;		///< Reference to circle center
};




}

