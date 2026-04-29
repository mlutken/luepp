#pragma once

#include "../../_tmath_internal_utils.hpp"


namespace lue::math::p2d {

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Line (2D) defined by a point and a (direction) vector. 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PLinePVStoreByValue
>
class line_pv
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

}


