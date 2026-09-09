#pragma once
// ****************************************************************************
// CREATED_ON	: 09-07-03
// CREATED_BY	: Martin Lutken
// ****************************************************************************

#include <math/base/lue_math.h>
#include <cstdio>
#include <string>

/*
Template utility functions. The functions are only ment to be used within gmath
namepace as boost, loki and others provide the same kinds of templates. It's just
to keep the gmath library as independent of other libraries as possible.

Most of this is actually "stolen" from the loki-library!

*/

namespace lue::math::utils {



/** Class template select. (From loki library, but uses stl naming style here)
    Selects one of two types based upon a boolean constant
    Invocation: select<flag, T, U>::result
    where:
    flag is a compile-time boolean constant
    T and U are types
    Result evaluates to T if flag is true, and to U otherwise.*/
    template <bool flag, typename T, typename U>
    struct select
    {
        typedef T result;
    };
    template <typename T, typename U>
    struct select<false, T, U>
    {
        typedef U result;
    };

    /// Convert float number to string for debug print.
inline std::string		str(float Number,		///< [in] number to turn into string.
                            int   iDecimals= -1	///< [in] Number of decimals
                            )
{
    using namespace std;
    char sz[10];
    switch ( iDecimals )
    {
        case 0	: sprintf(sz, "%.0f", Number); break;
        case 1	: sprintf(sz, "%.1f", Number); break;
        case 2	: sprintf(sz, "%.2f", Number); break;
        case 3	: sprintf(sz, "%.3f", Number); break;
        case 4	: sprintf(sz, "%.4f", Number); break;
        case 5	: sprintf(sz, "%.5f", Number); break;
        case 6	: sprintf(sz, "%.6f", Number); break;
        default	: sprintf(sz, "%f", Number);
    }
    return std::string(sz);
}





} //lue::math::utils

