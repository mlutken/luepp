/**@file
 * Extern C macro.
 */
#ifndef CESL_COMPILER_EXTERN_C_H
#define CESL_COMPILER_EXTERN_C_H

#ifndef extern_C
#   ifdef __cplusplus
#       define extern_C extern "C"
#   else
#       define extern_C extern
#   endif
#endif

#endif // CESL_COMPILER_EXTERN_C_H
