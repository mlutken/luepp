#pragma once

#include "../../_tmath_internal_utils.hpp"


namespace lue::math::p2d {

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Rectangle (2D) defined by width, height and a center. 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PBoxStoreByValue
>
class box
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

}


