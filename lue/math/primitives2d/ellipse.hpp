#pragma once

#include "../../_tmath_internal_utils.hpp"


namespace lue::math::p2d {

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Ellipse (2D) defined by 2 points and 2 radius'es .
\todo How to define an ellipse. See fx. http://mathworld.wolfram.com/Ellipse.html 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PEllipseStoreByValue
>
class ellipse
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

}

