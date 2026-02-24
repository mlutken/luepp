#pragma once
#include <luepp_default_config.h>
#include <cstdint>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <algorithm>


namespace lue {
/**
 *

*/

class parser
{
public:


private:
    // --- Member data --
    // --- Implementation remark: ---
    // We do not use the simpler 'value_type  data_[CAPACITY];'
    // data memeber since that would default initialize the entire array, which
    // we don't want to spend time doing as the container conceptually is empty
    // after construction, unless we are using one of the  initializing constructors.
    // So: We simply want to reserve the memory at first.
};


} // END namespace lue

