#pragma once

#include <iostream>
#include <string>
#include "stdint.h"

class ConvertString
{
public:
	static uint32_t ToUInt32(const std::string& hexText)
	{
        char* end;

        unsigned long value = std::strtoul(hexText.c_str(), &end, 16); // Convert to long with base 16
        if (*end == '\0')
        { // Check if entire string was converted
            //std::cout << "The decimal value of hex " << hexText << " is: " << value << std::endl;
            //std::cout << "The hex value is: 0x" << std::hex << value << std::endl;
        }
        else
        {
            std::cout << "Invalid hex string: " << hexText << std::endl;
        }

        return static_cast<uint32_t>(value);
	}
};

