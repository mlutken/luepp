#ifndef NESTLE_DEFAULT_CONFIG_H
#define NESTLE_DEFAULT_CONFIG_H

#ifndef CXX_STANDARD
#   define CXX_STANDARD 17
// Alternatives 98, 11, 14, (20 todo)
#endif

#ifndef CXX_STANDARD_LIBRARY
#   define CXX_STANDARD_LIBRARY 17
// Alternatives 98, 11, 14, (20 todo)
#endif

#ifndef ESTL_USE_EXCEPTIONS
#   define ESTL_USE_EXCEPTIONS 1
// Alternatives 0 : Don't usse execptions
#endif

// FIXMENM: Testing only!!!
//#undef CXX_STANDARD
//#define CXX_STANDARD 98


// Some definitions
#if (CXX_STANDARD == 98)
#   define NESTLE_NOEXEPT
#else
#   define NESTLE_NOEXEPT noexcept
#endif

#if ( (CXX_STANDARD == 98) || (CXX_STANDARD_LIBRARY == 98))
#   define NESTLE_MOVE(x) x
typedef long  signed_size_t;
#else
#   include <cstdint>
#   define NESTLE_MOVE(x) std::move(x)
typedef std::int64_t signed_size_t;
#endif

#endif //NESTLE_DEFAULT_CONFIG_H
