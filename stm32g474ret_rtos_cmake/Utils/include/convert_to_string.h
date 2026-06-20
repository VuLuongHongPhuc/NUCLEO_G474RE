/**
 * @file convert_to_string.h
 * @brief Convert to string
 * 
 * @author Phuc VU
 * @date Jun 20, 2026
 */

#ifndef __CONVERT_TO_STRING_H_
#define __CONVERT_TO_STRING_H_

/********************************* Includes ***************************************/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Constants definition ***************************/
/********************************* Macros definition ******************************/
/********************************* Types definition *******************************/
/********************************* Global variable ********************************/
/********************************* API functions prototype ************************/



/**
 * @brief Convert uint32 to string
 * @note Use
 *     char str[11];
 *     u32toa(4294967295, str); // max uint32 = 10 char -> str == "4294967295" 
 * @param value [in] - Input value to convert
 * @param buf [out] - Output value string converted
 * @retval - buf pointer
 */
extern char *u32toa(uint32_t value, char *buf);

/**
 * @brief Convert float to string with fixed-point 3
 * @note Use
 *     int32_t value = (int32_t)(floatValue * 1000);
 *     char str[16];
 *     fixed3_to_str(12345, str);   // "12.345"
 *     fixed3_to_str(-9876, str);   // "-9.876"
 *     fixed3_to_str(42, str);      // "0.042"
 * @param value [in] - Input value to convert
 * @param buf [out] - Output value string converted
 * @retval - buf pointer
 */
extern char *fixed3_to_str(int32_t value, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __CONVERT_TO_STRING_H_ */