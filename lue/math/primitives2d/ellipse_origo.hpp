#pragma once

#include "../../_tmath_internal_utils.hpp"


namespace lue::math::p2d {


// No need for storage policy classes here! We have only a scalars (radius'es).


/** 
Ellipse (2D) defined by 2 radius'es centered around origo.
\todo How to define an ellipse. See fx. http://mathworld.wolfram.com/Ellipse.html 
*/
template
<
    typename T
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

