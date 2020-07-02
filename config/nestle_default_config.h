#ifndef NESTLE_DEFAULT_CONFIG_H
#define NESTLE_DEFAULT_CONFIG_H

// IMPORTANT: Must match the definitions set in tools/cmake/default_versions.cmake
//            The reason for the redundancy is to make it easier for users that do not want
//            to compile this using cmake!
#define NESTLE_PLATFORM_TYPE_DESKTOP    1
#define NESTLE_PLATFORM_TYPE_EMBEDDED   2

#define NESTLE_SYSTEM_NAME_LINUX        1
#define NESTLE_SYSTEM_NAME_WINDOWS      2
#define NESTLE_SYSTEM_NAME_OSX          3
#define NESTLE_SYSTEM_NAME_FREERTOS     4


#ifndef CXX_STANDARD
    #define CXX_STANDARD 17
    // Alternatives 98, 11, 14, (20 todo)
#endif

#ifndef CXX_STANDARD_LIBRARY
    #define CXX_STANDARD_LIBRARY 17
// Alternatives 98, 11, 14, (20 todo)
#endif

#ifndef ESTL_USE_EXCEPTIONS
    #define ESTL_USE_EXCEPTIONS 1
    // Alternatives 0 : Don't use execptions
#endif

// FIXMENM: Testing only!!!
//#undef CXX_STANDARD
//#define CXX_STANDARD 98


// Some definitions
#if (CXX_STANDARD == 98)
    #define NESTLE_NOEXEPT throw()
    #define constexpr
    #define nullptr NULL
    #define NESTLE_ALIGNAS(x) x
#else
    #define NESTLE_NOEXEPT noexcept
    #define NESTLE_ALIGNAS(x) alignas(x)
#endif

#if ( (CXX_STANDARD == 98) || (CXX_STANDARD_LIBRARY == 98))
    #define NESTLE_MOVE(x) x
    #define NESTLE_FORWARD(x) x
typedef long  signed_size_t;
#else
    #include <cstdint>
    #define NESTLE_MOVE(x) std::move(x)
    #define NESTLE_FORWARD(x) std::forward(x)
    typedef std::int64_t signed_size_t;
#endif


#if (ESTL_USE_EXCEPTIONS == 1)
    #define ESTL_THROW(exception_type, message) throw exception_type(message)
#else
    #define ESTL_THROW(exception_type, message)
#endif

#ifndef NESTLE_PLATFORM_TYPE
    #define NESTLE_PLATFORM_TYPE NESTLE_PLATFORM_TYPE_DESKTOP
#endif

#ifndef NESTLE_SYSTEM_NAME
    #define NESTLE_SYSTEM_NAME NESTLE_SYSTEM_NAME_LINUX
    // TODO: Alternatives windows, osx
#endif


// Some helper macros
// Macro for adding quotes
#define NESTLE_STRINGIFY(X) NESTLE_STRINGIFY2(X)
#define NESTLE_STRINGIFY2(X) #X

// Macros for concatenating tokens
#define NESTLE_CAT(X,Y) NESTLE_CAT2(X,Y)
#define NESTLE_CAT2(X,Y) X##Y
#define NESTLE_CAT_2 NESTLE_CAT
#define NESTLE_CAT_3(X,Y,Z) NESTLE_CAT(X,NESTLE_CAT(Y,Z))
#define NESTLE_CAT_4(A,X,Y,Z) NESTLE_CAT(A,NESTLE_CAT_3(X,Y,Z))
#define NESTLE_CAT_5(B,A,X,Y,Z) NESTLE_CAT(B,NESTLE_CAT_4(A,X,Y,Z))


#endif //NESTLE_DEFAULT_CONFIG_H
