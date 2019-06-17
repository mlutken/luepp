/**@file
 * cesl_number_to_strin Functions to convert from numbers to strings
 * Integer, floating point and pointer conversions.
*/
#ifndef CESL_STRINGS_NUMBER_TO_STRING_H
#define CESL_STRINGS_NUMBER_TO_STRING_H

#include <cesl_compiler/cesl_extern_c.h>
#include <stdint.h>
#include <stddef.h>

// ----------------------------
// Signed integer functions ---
// ----------------------------

/**
 * @brief cesl_int64_to_str Convert 64 bit integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
If base is 10 and value is negative, the resulting string is preceded with a
minus sign (-). With any other base, value is always considered unsigned.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_int64_to_str(int64_t value, char* dst_str, uint8_t base);

/**
 * @brief cesl_int32_to_str Convert 32 bit integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
If base is 10 and value is negative, the resulting string is preceded with a
minus sign (-). With any other base, value is always considered unsigned.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_int32_to_str(int32_t value, char* dst_str, uint8_t base);


/**
 * @brief cesl_int16_to_str Convert 16 bit integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
If base is 10 and value is negative, the resulting string is preceded with a
minus sign (-). With any other base, value is always considered unsigned.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_int16_to_str(int16_t value, char* dst_str, uint8_t base);

/**
 * @brief cesl_int8_to_str Convert 8 bit integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
If base is 10 and value is negative, the resulting string is preceded with a
minus sign (-). With any other base, value is always considered unsigned.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_int8_to_str(int8_t value, char* dst_str, uint8_t base);


/**
 * @brief cesl_long_to_str Convert long integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
If base is 10 and value is negative, the resulting string is preceded with a
minus sign (-). With any other base, value is always considered unsigned.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_long_to_str(long value, char* dst_str, uint8_t base);

/**
 * @brief cesl_int_to_str Convert integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
If base is 10 and value is negative, the resulting string is preceded with a
minus sign (-). With any other base, value is always considered unsigned.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_int_to_str(int value, char* dst_str, uint8_t base);


// ------------------------------
// Unsigned integer functions ---
// ------------------------------

/**
 * @brief cesl_uint64_to_str Convert 64 bit unsigned integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_uint64_to_str(uint64_t value, char* dst_str, uint8_t base);

/**
 * @brief cesl_uint32_to_str Convert 32 bit unsigned integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_uint32_to_str(uint32_t value, char* dst_str, uint8_t base);

/**
 * @brief cesl_uint16_to_str Convert 16 bit unsigned integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_uint16_to_str(uint16_t value, char* dst_str, uint8_t base);

/**
 * @brief cesl_uint8_to_str Convert 8 bit unsigned integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_uint8_to_str(uint8_t value, char* dst_str, uint8_t base);

/**
 * @brief cesl_ulong_to_str Convert unsigned long integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_ulong_to_str(unsigned long value, char* dst_str, uint8_t base);

/**
 * @brief cesl_uint_to_str Convert unsigned integer to string (similar to itoa)
 * @param value Value to be converted to a string.
 * @param dst_str Array in memory where to store the resulting null-terminated string.
 * @param base Numerical base used to represent the value as a string, between 2 and 36,
 *        where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 * @return The lenght of the string written to dst_str.
 * @remark This function returns the lenght of the string written and
 *         not a pointer to the string as itoa does.

Converts an integer value to a null-terminated string using the specified base and
stores the result in the array given by str parameter.
str should be an array long enough to contain any possible value:
(sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
*/
extern_C size_t cesl_uint_to_str(unsigned int value, char* dst_str, uint8_t base);

#endif //CESL_STRINGS_NUMBER_TO_STRING_H
