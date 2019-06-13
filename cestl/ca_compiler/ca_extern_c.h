/**@file
 * Extern C macro.
 */
#ifndef CA_COMPILER_CA_EXTERN_C_H
#define CA_COMPILER_CA_EXTERN_C_H

#ifndef extern_C
#   ifdef __cplusplus
#       define extern_C extern "C"
#   else
#       define extern_C extern
#   endif
#endif

#endif // CA_COMPILER_CA_EXTERN_C_H
