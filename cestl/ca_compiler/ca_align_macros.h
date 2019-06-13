/**@file
 * ca_align_macros Macros for aligning memory
 */
#ifndef CA_COMPILER_CA_ALIGN_MACROS_H
#define CA_COMPILER_CA_ALIGN_MACROS_H

#if defined(_MSC_VER)
#define CA_ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define CA_ALIGNED(x) __attribute__ ((aligned(x)))
#endif
#endif

#define CA_ALIGNED_TYPE(t,x) typedef t CA_ALIGNED(x)

#endif // CA_COMPILER_CA_ALIGN_MACROS_H
