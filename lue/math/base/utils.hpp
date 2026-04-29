#pragma once

// standard headers
//#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//gmath headers
#include "lue_math.h"



/**
Utility math funtions from the standard C math.h file along with swap, clamp
and a few othter, Scalar math.
XX*/


namespace lue::math {

    // *********************************
    // *** Random numbers generation ***
    // *********************************

    /// Get integer random number between 0 and an upper limit.
    /// \return Random number in interval [0..iUpperLimit]
    inline int_t random(int_t iUpperLimit)	///< Upper limit. Ie. number returned is in [0..iUpperLimit]
{
    return rand() % (iUpperLimit+1);
}

/// Get integer random number in interval.
/// \return Random number in interval [iLowerLimit..iUpperLimit]
inline int_t random(int_t iLowerLimit,	///< lower limit. Ie. number returned is in [iLowerLimit..iUpperLimit]
                    int_t iUpperLimit)	///< Upper limit. Ie. number returned is in [iLowerLimit..iUpperLimit]
{
    return (rand() % (iUpperLimit+1)) + iLowerLimit;
}


// **********************************
// *** Functions Not from math.h  ***
// **********************************


/** Restricts the value of variable to lie within the boundaries of a
    minimum and a maximum value. The interval is not inclusive */
template<typename TYPE>
inline void clamp(	TYPE& v,				///< [inout] Variable to restrict value.
                  const TYPE& min,		///< [in]	 Minimum allowed value.
                  const TYPE& max)		///< [in]	 Maximum allowed value.
{
    if		(v < min)	v = min;
    else if	(v > max)	v = max;
}

/** Restricts the value of variable to lie within the boundaries of a
    minimum and a maximum value. The interval is not inclusive */
template<typename TYPE>
inline TYPE clamp_copy(const TYPE& v,          ///< [in] Variable to restrict value.
                       const TYPE& min,		///< [in]	 Minimum allowed value.
                       const TYPE& max)		///< [in]	 Maximum allowed value.
{
    if		(v < min)	return min;
    else if	(v > max)	return max;
    return v;
}

/** Test if the value of a variable lies within the boundaries of a
    minimum and a maximum value. The interval is "open" (is not inclusive the end values)
    @todo Perhaps use std::decay on arguments in comparisons. */
template<typename TYPE>
inline bool within_interval_open(const TYPE& v,         ///< [in] Variable to restrict value.
                                 const TYPE& min,		///< [in]	 Minimum allowed value.
                                 const TYPE& max)		///< [in]	 Maximum allowed value.
{
    return min < v && v < max;
}


/** A scalar version of vector pull. This function pulls a variable value towards a
    wanted value, in a given Delta value per time. */
template<typename T1, typename T2, typename T3>
bool scalar_pull(	T1 &Val,				///< [inout] Value to change
                 const T2 DestVal,          ///< [in]  Destination value we are "pulling" towards
                 const T3 Delta)			///< [in]  Delta value
{
    bool bReachedValue= false;
    if (Val > DestVal)
    {
        Val= Val - Delta;
        if (Val < DestVal)
        {
            Val= DestVal;
            bReachedValue= true;
        }
    }
    else if (Val < DestVal)
    {
        Val= Val + Delta;
        if (Val > DestVal)
        {
            Val= DestVal;
            bReachedValue= true;
        }
    }
    else
    {
        bReachedValue= true;
    }
    // ELSE
    return bReachedValue;
}

/** Linear Interpolation (Lerp)
@return The "new" value between min and max
*/
template<typename T>
T lerp (T min,                    ///< [in] Minimum/start value
        T max,                    ///< [in] Maximum/end value. Ie. the value we are "pulling" towards
        T interpolation_fraction) ///< [in] Fraction/inperpolation point. I.e. how "far" relative (0..1) between min and max
{
    return min + (max - min)*interpolation_fraction;
}


template <typename INT_TYPE>
constexpr INT_TYPE round_up(INT_TYPE number, INT_TYPE multiple)
{
    if (multiple == 0) return number;
    const INT_TYPE is_positive = static_cast<INT_TYPE>(number >= 0);
    return ((number + is_positive * (multiple - 1)) / multiple) * multiple;
}

template <typename INT_TYPE>
constexpr INT_TYPE round_down(INT_TYPE number, INT_TYPE multiple)
{
    if (multiple == 0) return number;
    const INT_TYPE is_negative = static_cast<INT_TYPE>(number < 0);
    return ((number - is_negative * (multiple - 1)) / multiple) * multiple;
}


}

