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
    
    
    
    void        reset_errors    ()          { all_ok_ = true;             }
    bool        all_ok          ()  const   { return all_ok_;             }
    bool        errors          ()  const   { return all_ok_ == false;    }
private:
    // --- Member data --
    
    bool        all_ok_     {true};
};


} // END namespace lue

