#pragma once

#include "../../_tmath_internal_utils.hpp"


namespace lue::math::p2d {

/// DOXYS_OFF

// Storage policies here !!!

/// DOXYS_ON

/** 
Line (2D) defined by 2 points. 
*/
template
<
    typename T,
    template <class> class StoragePolicy = PLine2PStoreByValue
>
class line_2p
    : public StoragePolicy<T>
{
	// ----------------
	// --- Typedefs ---
	// ----------------
	
public:	

};

}

