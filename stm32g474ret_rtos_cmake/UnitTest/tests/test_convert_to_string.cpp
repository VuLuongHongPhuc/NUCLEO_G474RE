#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "test_convert_to_string.h"
#include "convert_to_string.h"


TEST(ConvertToStringTest, FromUInt32)
{
    char str[11];
    u32toa(4294967295, str); // max uint32 = 10 char -> str == "4294967295" 
    std::string out(str);

    EXPECT_EQ(out, "4294967295");
}

