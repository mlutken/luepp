#pragma once
// ****************************************************************************
// CREATED_ON	: 20-03-03 19:00
// CREATED_BY	: Martin Lutken
// ---------------------------------------------------------------------------- 
// COPYRIGHT	: 
// ****************************************************************************


#include <math.h>
#include <cmath>
#include <cpaf_libs/math/base/lue_math.h>
#include <cpaf_libs/math/base/constants.hpp>

/** \unit
All math functions from the standard C math.h file. Default implementation 
uses float, but there's a spezialization for double.

*/


namespace lue::math {


// ***********************************************
// *** Abs, Ceil, Floor, Square Root, Modulus  ***
// ***********************************************

/** Absolute value. 
	\return Absolute value of x. */
template<typename TYPE>
inline TYPE Abs(const TYPE Val					///< [in] Value to find absolute value of. 
                )
{ 
	return static_cast<TYPE>( Val < static_cast<TYPE>(0) ? -Val:Val );
}


/** Get smallest not less than. 
	\return The smallest whole number not less than the specified real number. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Ceil(const TYPE x				///< [in] Argument. 
                 )
{ 
	return static_cast<TYPE>(ceilf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Ceil<double_t>(const double_t x)
{
    return static_cast<double_t>(std::ceil( x ));
}


/** Get largest not greater than.
	\return The largest integer not greater than the specified real number. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Floor(const TYPE x				///< [in] Argument. 
                  )
{ 
	return static_cast<TYPE>(floorf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Floor<double_t>(const double_t x)
{ 
    return static_cast<double_t>(std::floor( x ));
}


/** Square root of x. 
	\return The square root of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Sqrt(const TYPE x				///< [in] Value to find square root of. 
                 )
{ 
    return static_cast<TYPE>(std::sqrt( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Sqrt<double_t>(const double_t x)
{ 
    return static_cast<double_t>(std::sqrt( x ));
}


/** Mod(y,z), Floating-point modulus. The Mod function calculates 
	the floating-point remainder of y/z. If the % (modulus) operation were 
	defined for floating-point numbers, the expression would produce the 
	following: x = y % z; 
	\return The value y if the value z is 0. Otherwise, it returns a value that 
	has the same sign as y, is less than z, and satisfies the following 
	relationship: y = (i * z) + x. The argument i is an integer. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Mod(const TYPE y,				///< [in] Numerator.
				const TYPE z				///< [in] Denominator. 
				)
{ 
	return static_cast<TYPE>(fmodf( y, z ));
}
// Double floating point precision math specialization.
template<>
inline double_t Mod<double_t>(	const double_t y,
								const double_t z)
{ 
	return static_cast<double_t>(fmod( y, z ));
}
// Integer math specialization.
template<>
inline int_t Mod<int_t>(const int_t y,
						const int_t z)
{ 
	return static_cast<int_t>( y % z );
}


// *******************************
// *** Trigonometric functions ***
// *******************************

/** Cosine. 
	\return The cosine of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Cos(const TYPE x				///< [in] Argument in radians. 
                )
{ 
	return static_cast<TYPE>(cosf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Cos<double_t>(const double_t x)
{ 
	return static_cast<double_t>(cos( x ));
}


/** Sine. 
	\return The sine of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Sin(const TYPE x				///< [in] Argument in radians. 
                )
{ 
	return static_cast<TYPE>(sinf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Sin<double_t>(const double_t x)
{ 
	return static_cast<double_t>(sin( x ));
}


/** Tangent. 
	\return The tangent of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Tan(const TYPE x				///< [in] Argument in radians. 
                )
{ 
	return static_cast<TYPE>(tanf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Tan<double_t>(const double_t x)
{ 
	return static_cast<double_t>(tan( x ));
}


// ***************************************
// *** Inverse trigonometric functions ***
// ***************************************

/** Acos safe. Safe version of the standard arcus cosine funtion. The input 
	value is clamped to [-1;1].
 */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Acos(const TYPE x					///< [in] Argument in radians. 
                 )
{ 
	if( x >  1.0f ) return 0;
    if( x < -1.0f ) return constants<TYPE>::pi();
	return static_cast<TYPE>(acosf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Acos<double_t>(const double_t x) 
{ 
	if( x >  1.0f ) return 0;
    if( x < -1.0f ) return constants<double_t>::pi();
	return static_cast<double_t>(acos( x ));
}


/** Asin safe. Safe version of the standard arcus sine funtion. The input value 
	is clamped to [-1;1].
 */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Asin(const TYPE x					///< [in] Argument in radians. 
                 )
{ 
    if( x >  1.0f ) return  constants<TYPE>::pi()/2.0f;
    if( x < -1.0f ) return -constants<TYPE>::pi()/2.0f;
	return static_cast<TYPE>(asinf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Asin<double_t>(const double_t x) 
{ 
    if( x >  1.0f ) return  constants<double_t>::pi()/2.0f;
    if( x < -1.0f ) return -constants<double_t>::pi()/2.0f;
	return static_cast<double_t>(asin( x ));
}


/** Arctangent. 
	\return The arctangent of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Atan(const TYPE x				///< [in] Argument in radians. 
                 )
{ 
	return static_cast<TYPE>(atanf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Atan<double_t>(const double_t x)
{ 
	return static_cast<double_t>(atan( x ));
}

/** Atan(x,y), Arcus tangens of a quotient x/y.
	This function computes the arctangent of x/y and returns an angular 
	value expressed in radians. The result is constrained as -pi to pi. You can 
	express the large tangent value as a quotient of two more reasonable numbers. 
	Since the tangent becomes very large for angles close to pi/2, the this 
	function is often used to avoid computations with large numbers that might 
	easily overflow.
  \return Arctangent of x/y.
*/
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Atan(const TYPE x,						///< [in] x (Numerator)
				 const TYPE y						///< [in] y (Denominator). 
				 )
{ 
	return static_cast<TYPE>(atan2f( x, y ));
}
// Double floating point precision math specialization.
template<>
inline double_t Atan<double_t>(const double_t x,	///< [in] x (Numerator)
							   const double_t y)	///< [in] y (Denominator). 
{ 
	return static_cast<double_t>(atan2( x, y ));
}

// ****************************
// *** Hyperbolic functions ***
// ****************************

/** Hyperbolic cosine. 
	\return The hyperbolic cosine of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Cosh(const TYPE x						///< [in] Argument in radians. 
				 )
{ 
	return static_cast<TYPE>(coshf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Cosh<double_t>(const double_t x)
{ 
	return static_cast<double_t>(cosh( x ));
}


/** Hyperbolic sine. 
	\return The hyperbolic sine of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Sinh(const TYPE x				///< [in] Argument in radians. 
				 )
{ 
	return static_cast<TYPE>(sinhf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Sinh<double_t>(const double_t x)
{ 
	return static_cast<double_t>(sinh( x ));
}


/** Hyperbolic tangent. 
	\return The hyperbolic tangent of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Tanh(const TYPE x				///< [in] Argument in radians. 
				 )
{ 
	return static_cast<TYPE>(tanhf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Tanh<double_t>(const double_t x)
{ 
	return static_cast<double_t>(tanh( x ));
}

// *********************************************
// *** Exponential and Logarithmic functions ***
// *********************************************

/** Natural exponential function. The exp function raises the natural logarithm 
	base 'e' to the x'th power. 
	\return The exp(x) = e^x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Exp(const TYPE x				///< [in] Argument. 
				)
{ 
	return static_cast<TYPE>(expf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Exp<double_t>(const double_t x)
{ 
	return static_cast<double_t>(exp( x ));
}


/** Natural logarithmic function. The log function calculates the base e (natural) 
	logarithm. This function requires a positive argument. 
	\return The natural logarithm of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Log(const TYPE x				///< [in] Argument. Must be positive. 
				)
{ 
	return static_cast<TYPE>(logf( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Log<double_t>(const double_t x)
{ 
	return static_cast<double_t>(log( x ));
}

/** Base 10 logarithmic function. The log function calculates the base 10 logarithm. This 
	function requires a positive argument. 
	\return The base 10 logarithm of x. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Log10(const TYPE x				///< [in] Argument. Must be positive. 
				  )
{ 
	return static_cast<TYPE>(log10f( x ));
}
// Double floating point precision math specialization.
template<>
inline double_t Log10<double_t>(const double_t x)
{ 
	return static_cast<double_t>(log10( x ));
}


// ***********************
// *** Power Functions ***
// ***********************
/** Pow(x,y). The pow function raises the argument x to the y power. 
	\return x^y. */
// Default implementation. Uses single floating point precision math.
template<typename TYPE>
inline TYPE Pow(const TYPE x,				///< [in] Argument.
				const TYPE y				///< [in] Exponent. 
				)
{ 
	return static_cast<TYPE>(powf( x, y ));
}
// Double floating point precision math specialization.
template<>
inline double_t Pow<double_t>(const double_t x,			///< [in] Argument.
							  const double_t y			///< [in] Exponent. 
							  )
{ 
	return static_cast<double_t>(pow( x, y ));
}


