
/********************************* Includes ***************************************/

#include <stdint.h>

/********************************* Constants definition ***************************/

static const char digit_pairs[] =
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899";

/********************************* Macros definition ******************************/
/********************************* Types definition *******************************/
/********************************* Local variable *********************************/
/********************************* API functions prototype ************************/

char *u32toa(uint32_t value, char *buf)
{
    char temp[10];
    char *p = temp + sizeof(temp);

    while (value >= 100) {
        uint32_t q = value / 100;
        uint32_t r = value - q * 100;

        p -= 2;
        p[0] = digit_pairs[r * 2];
        p[1] = digit_pairs[r * 2 + 1];

        value = q;
    }

    if (value < 10) {
        *--p = (char)('0' + value);
    } else {
        p -= 2;
        p[0] = digit_pairs[value * 2];
        p[1] = digit_pairs[value * 2 + 1];
    }

    char *out = buf;
    while (p < temp + sizeof(temp))
        *out++ = *p++;

    *out = '\0';
    return buf;
}


char *fixed3_to_str(int32_t value, char *buf)
{
    char *p = buf;

    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

    uint32_t integer = value / 1000;
    uint32_t fraction = value % 1000;

    // Integer part
    char tmp[10];
    int n = 0;

    if (integer == 0) {
        tmp[n++] = '0';
    } else {
        while (integer) {
            tmp[n++] = '0' + (integer % 10);
            integer /= 10;
        }
    }

    while (n--)
        *p++ = tmp[n];

    // Decimal point
    *p++ = '.';

    // Fractional part (always 3 digits)
    *p++ = '0' + (fraction / 100);
    fraction %= 100;

    *p++ = '0' + (fraction / 10);
    fraction %= 10;

    *p++ = '0' + fraction;

    *p = '\0';

    return buf;
}

/*EOF*/
