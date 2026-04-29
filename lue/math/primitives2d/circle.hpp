
#include "math/base/lue_math_internal_utils.hpp"
#include <cmath>

#include "math/base/v2.hpp"
#include "math/base/g33.hpp"
#include "math/primitives2d/circle_storage_policies.hpp"

namespace lue::math::p2d {


/**
2D circle class. Circle defined by a center and a radius.  */
template
<
    typename T,
    template <class> class StoragePolicy = PCircleStoreByValue
>
class circle
{
public:
    // ----------------
    // --- Typedefs ---
    // ----------------
    typedef circle<T, StoragePolicy >	value_type;
    typedef value_type&					reference;
    typedef const value_type&			const_reference;
    typedef value_type*					pointer;
    typedef const value_type*			const_pointer;

    /// For operators and functions returning an object of type circle.
    /// Since we get temporary object created this way we need these temporarys
    /// to have it's own store for the whole circle.
    typedef circle<T, PCircleStoreByValue>	value_value_type;

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
    circle() : m_Center() {}


    /// Copy constructor. Initializing with another circle.
    circle(const_reference c) : m_Center(c.m_Center), m_Radius(c.m_Radius) {}

    /** Constructor with center and radius initialization. */
    circle(	v2_storage_type vCenter,	///< [in] Circle center
            T Radius)					///< [in] Circle radius
        : 	m_Center(vCenter), m_Radius(Radius) {}


    /// Cast to circle with different number-type 'T'.
    template <typename T1>
    operator circle<T1>() const
    {
        typedef circle<T1> circle_value_type;
        return circle_value_type( center(), radius() );
    }


    // ------------------------
    // --- Access functions ---
    // ------------------------

    /// Access center of circle. \return Reference to center of rectangle.
    v2_reference		center()			{ return m_Center.center(); }
    v2_const_reference	center()	const	{ return m_Center.center(); }

    /// Access radius of circle. \return Reference to radius of circle.
    T&					radius()			{ return m_Radius; }
    T					radius()	const	{ return m_Radius; }

    /// Set radius of circle.
    void				radius(T r) 		{ m_Radius = r; }

    /// Access x-coordinate of circle. This is the same as circle::center().x().
    /// \return Reference to x-coordinate of circle (center x-coordinate).
    T&					x()			{ return center().x(); }
    T					x()	const	{ return center().x(); }

    /// Access y-coordinate of circle. This is the same as circle::center().y().
    /// \return Reference to x-coordinate of circle (center y-coordinate).
    T&					y()			{ return center().y(); }
    T					y()	const	{ return center().y(); }

    // ----------------------------
    // --- Set values functions ---
    // ----------------------------

    /** Set circle with  points. */
    void		set(v2_const_reference vCenter,	///< [in] Circle center
                    T Radius)					///< [in] Circle radius
    {
        center() = vCenter;
        m_Radius = Radius;
    }


    /** Set circle with 3 values. */
    void		set(T CenterX,			///< [in] Circle center, X-coordinate.
                    T CenterY,			///< [in] Circle center, Y-coordinate.
                    T Radius)			///< [in] Circle radius.
    {
        center().set(CenterX, CenterY);
        m_Radius = Radius;
    }



    // ----------------------------
    // --- Assignment operators ---
    // ----------------------------

    /** '=' operator, Assignment from another circle.
        \note Here we allways copy values to destination. In case this circle does not
              use the PCircleStoreByValue - then it is still values that are copied - not
              pointers or references, that are assigned. */
    template < template <class> class StorPolicy >
    reference			operator =(const circle<T, StorPolicy>& c)		///< [in] Right hand side operand.
    {
        center() = c.center();
        radius() = c.radius();
        return *this;
    }

    // ------------------------------------
    // --- Add/Substract math operators ---
    // ------------------------------------

    /** '+=' operator, Add vector to circle. Translate circle to other position.
        \return Reference to "this".*/
    reference			operator +=(v2_value_type v)		///< [in] Vector to add to all points.
    {
        center() += v;
        return *this;
    }


    /** '-=' operator, Subtract vector from circle. Translate circle to other position.
        \return Reference to "this".*/
    reference			operator -=(v2_value_type v)		///< [in] Vector to subtract to all points.
    {
        center() -= v;
        return *this;
    }

    /** '+' operator, Add vector to circle. Translate circle to other position.
        \return circle translated to the new position. */
    value_value_type	operator +(	v2_const_reference v		///< [in] Vector to add to all points.
                              )const
    {
        return value_value_type( center() + v, radius());
    }

    /** '-' operator, Subtract vector from circle. Translate circle to other position.
        \return circle translated to the new position. */
    value_value_type	operator -(	v2_const_reference v		///< [in] Vector to subtract to all points.
                              )const
    {
        return value_value_type( center() - v, radius());
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
        v2_value_type p0T = p0 - center();	// Simulate that circle has center in origo.
        return ( p0T.len2() <= radius()*radius() );
    }

    /** Point inside.
    \return True if point given is inside the circle. */
    v2_value_type			angle_to_pos(const T& angle	///< [in] Angle to convert to position on circle
                                ) const
    {
        v2_value_type vec_angle (std::cos(angle), std::sin(angle));
        return center() + vec_angle*radius();
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
        return	( Abs(radius() - c.radius()) < constants<T>::epsilon() ) &&
                ( center() == c.center() );
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
        return	( Abs(radius() - c.radius()) > constants<T>::epsilon() ) ||
                ( center() != c.center() );
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
        return	( radius() == c.radius() ) &&
                center().equal_exact( c.center() );

    }


    /** Equal with custom epsilon comparision. Tests two vectors for equality.
        Supplied epsilon value is used.
        \return True if all corresponding elements of the two vectors is no further,
        than epsilon apart (absolute value), false otherwise. */
    bool_t		equal_epsilon(const_reference c,	///< [in] Circle to compare with
                              T epsilon				///< [in] Epsilon value to use when comparing
                             ) const
    {
        return	( Abs(radius() - c.radius()) < epsilon ) &&
                center().equal_epsilon( c.center(), epsilon );
    }


    // -----------------------
    // --- Debug Functions ---
    // -----------------------

    /// Convert circle to a string for debug print.
    std::string			str(int_t iDecimals= -1	///< [in] Number of decimals
                           ) const
    {
        std::string s = "Center: " + center().str(iDecimals);
                    s += "  Radius: " + _UTILS::str( radius(), iDecimals );
        return s;
    }

private:
    // ------------
    // --- Data ---
    // ------------
    StoragePolicy<T>	m_Center;		///< Center of circle.
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
template < typename T, template <class> class StoragePolicy >
inline std::ostream&
    operator <<(std::ostream& os,					///< [out] Output stream to write to.
                const circle<T, StoragePolicy>& c)	///< [in] Circle to write.
{
    os << "c:" << c.center() << " r:" << c.radius();
    return os;
}

/** '>>' operator. Read circle from input stream. */
template < typename T, template <class> class StoragePolicy >
inline std::istream&
    operator >>(std::istream& is,				///< [in]  Input stream to read from.
                circle<T, StoragePolicy>& c)	///< [out] Circle recieve the data from the stream.
{
    is>>c.center()>>c.radius();
    return is;
}

typedef circle<float, PCircleStoreByValue>	circle2f;


}

