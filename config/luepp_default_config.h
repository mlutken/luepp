#ifndef LUEPP_DEFAULT_CONFIG_H
#define LUEPP_DEFAULT_CONFIG_H
#if 0

#if (ESTL_USE_EXCEPTIONS == 1)
    #define ESTL_THROW(exception_type, message) throw exception_type(message)
#else
    #define ESTL_THROW(exception_type, message)
#endif

#else

// IMPORTANT: Must match the definitions set in tools/cmake/default_versions.cmake
//            The reason for the redundancy is to make it easier for users that do not want
//            to compile this using cmake!
#define LUEPP_PLATFORM_TYPE_DESKTOP    1
#define LUEPP_PLATFORM_TYPE_EMBEDDED   2

#define LUEPP_SYSTEM_NAME_LINUX        1
#define LUEPP_SYSTEM_NAME_WINDOWS      2
#define LUEPP_SYSTEM_NAME_OSX          3
#define LUEPP_SYSTEM_NAME_FREERTOS     4


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



// Some definitions
#if (CXX_STANDARD == 98)
    #define LUEPP_NOEXEPT throw()
    #define constexpr
    #define nullptr NULL
    #define LUEPP_ALIGNAS(x) x
#else
    #define LUEPP_NOEXEPT noexcept
    #define LUEPP_ALIGNAS(x) alignas(x)
#endif


#ifndef LUEPP_PLATFORM_TYPE
    #define LUEPP_PLATFORM_TYPE LUEPP_PLATFORM_TYPE_DESKTOP
#endif

#ifndef LUEPP_SYSTEM_NAME
    #define LUEPP_SYSTEM_NAME LUEPP_SYSTEM_NAME_LINUX
    // TODO: Alternatives windows, osx
#endif

#if (ESTL_USE_EXCEPTIONS == 1)
    #define ESTL_THROW(exception_type, message) throw exception_type(message)
#else
    #define ESTL_THROW(exception_type, message)
#endif


// Some helper macros
// Macro for adding quotes
#define LUEPP_STRINGIFY(X) LUEPP_STRINGIFY2(X)
#define LUEPP_STRINGIFY2(X) #X

// Macros for concatenating tokens
#define LUEPP_CAT(X,Y) LUEPP_CAT2(X,Y)
#define LUEPP_CAT2(X,Y) X##Y
#define LUEPP_CAT_2 LUEPP_CAT
#define LUEPP_CAT_3(X,Y,Z) LUEPP_CAT(X,LUEPP_CAT(Y,Z))
#define LUEPP_CAT_4(A,X,Y,Z) LUEPP_CAT(A,LUEPP_CAT_3(X,Y,Z))
#define LUEPP_CAT_5(B,A,X,Y,Z) LUEPP_CAT(B,LUEPP_CAT_4(A,X,Y,Z))

#endif

#endif //LUEPP_DEFAULT_CONFIG_H
