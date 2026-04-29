#pragma once
// ****************************************************************************
// CREATED_ON	: 31-08-03 09:13
// CREATED_BY	: Martin Lutken
// ----------------------------------------------------------------------------
// COPYRIGHT	:
// ****************************************************************************

// --- Include files ---
#include "lue_math.h"

namespace lue::math {

/**
Mathematical real number constants. Generic version uses float. Specialization
exist for double precision.
*/
template <typename T>
class constants
{
public:
    //---------------------
    //--- Trigonometric ---
    //---------------------

    /** PI. Definition of PI.*/
    static const T			pi()
    { return static_cast<T>(3.141592653589793f);}

    /** Convert radians to degrees. Multiplication constant used to convert
    from radians to degrees.
    Example: angle_in_radians = angle_in_degrees * C<float>::TODEG().*/
    static const T			todeg()
    { return static_cast<T>(180.f) / pi();}

    /** Convert degrees to radians. Multiplication constant used to convert
    from degrees to radians.
    Example: angle_in_radians = angle_in_degrees * C<float>::TODEG().*/
    static const T			torad()
    { return static_cast<T>( pi() / static_cast<T>(180.f) );}


    //------------
    //--- Misc ---
    //------------

    /** Epsilon value. More usable epsilon for practical numerical calculations
        than the one found in numeric_limits.*/
    static const T			epsilon()
    { return static_cast<T>(1.f / 8192.f);}

};

//----------------------------------------
//--- Double precision specialization. ---
//----------------------------------------
template <>
class constants<double_t>
{
public:
    //---------------------
    //--- Trigonometric ---
    //---------------------
    static constexpr double_t	pi()
    { return static_cast<double_t>(3.14159265358979323846264338327950288);}

    static constexpr double_t	todeg()
    { return static_cast<double_t>(180.0) / pi();}

    static constexpr double_t	torad()
    { return static_cast<double_t>( pi() / static_cast<double_t>(180.0));}

    //------------
    //--- Misc ---
    //------------
    static constexpr double_t	epsilon()
    { return static_cast<double_t>(1.f / 8192.f);} // NOT_IMPL: Better (smaller) value needed XX

};

using constants_f = constants<float>;
using constants_d = constants<double>;



}

