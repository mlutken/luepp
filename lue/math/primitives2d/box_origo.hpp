#pragma once

#include "math/math_internal_utils.hpp"


namespace lue::math::p2d {

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Rectangle (2D) defined by width, height and a centered around origo. 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PBoxOrigoStoreByValue
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

