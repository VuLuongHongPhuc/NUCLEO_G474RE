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




extern char *u32toa(uint32_t value, char *buf);


extern char *fixed3_to_str(int32_t value, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __CONVERT_TO_STRING_H_ */