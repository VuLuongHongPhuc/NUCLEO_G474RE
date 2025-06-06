#include <iostream>
#include <fstream>
#include <array>
#include "SRecord.h"
#include "ConvertString.h"

#include <windows.h> // Sleep

/*
* - SRecordStruct enregistre les data successif
* - new SRecordStruct si adresse discontinue
*/




int atoi(char c)
{
    if (c < 'A')
    {
        return (c - '0');
    }
    else
    {
        return ((c < 'a') ? (c - 'A') : (c - 'a'));
    }
}


int SRecord::ExtractLine(const std::string& line)
{
    if (line[0] != 'S')
    {
        // Error - Type not present
        return -1;
    }

    if (line.length() % 2)
    {
        std::cout << "Error - Number of char odd\n";
        return -1;
    }

    struct SRecordStruct SRecord;

    SRecord.Type = line[1] - '0';
    if (SRecord.Type > 9)
    {
        std::cout << "Error - Type S\n";
        return -1;
    }

    int ret = 0;

    switch (SRecord.Type)
    {
    case 0:// Header record
        break;

    case 3:// Data record with 32-bit address.
    case 7:// Termination records S3
    {
        std::string strCountHex = line.substr(2, 2);
        uint32_t count = ConvertString::ToUInt32(strCountHex);
        SRecord.CRC = static_cast<uint8_t>(count);

        std::string remainText = line.substr(4, line.length() - 4);

        ExtractAddress(remainText, SRecord);

        // Extract Data & CRC check
        ExtractData(remainText, SRecord);

        ret = (SRecord.Count == count) ? 0 : -1;

        m_SequenceList.emplace_back(SRecord);
    }break;

    default:
        std::cout << "Error - Type S not for STM32G474\n";
        ret = -1;
        break;
    }

    return ret;
}


int SRecord::ExtractAddress(std::string& line, struct SRecordStruct& SRecord)
{
    int ret = 0;

    switch (SRecord.Type)
    {
    case 0:   // Header record (often contains metadata like file name).
        ret = ExtractAddressExt(line, SRecord, 4);
        break;

    case 1:   // Data record with 16-bit address.
        ret = ExtractAddressExt(line, SRecord, 4);
        break;

    case 2:   // Data record with 24-bit address.
        ret = ExtractAddressExt(line, SRecord, 6);
        break;

    case 3:   // Data record with 32-bit address.
        ret = ExtractAddressExt(line, SRecord, 8);
        break;

    case 5:   // Record count (optional, number of data records). 16-bits count
        ret = ExtractAddressExt(line, SRecord, 4);
        break;

    case 6:   // Record count (optional, number of data records). 24-bits count
        ret = ExtractAddressExt(line, SRecord, 6);
        break;

    case 7:   // Termination records S3 (entry point for execution or end of data).
        ret = ExtractAddressExt(line, SRecord, 8);
        break;

    case 8:   // Termination records S2 (entry point for execution or end of data).
        ret = ExtractAddressExt(line, SRecord, 6);
        break;

    case 9:   // Termination records S1 (entry point for execution or end of data).
        ret = ExtractAddressExt(line, SRecord, 4);
        break;

    default:
        break;
    }

    return ret;
}

int SRecord::ExtractAddressExt(std::string& line, struct SRecordStruct& SRecord, int count)
{
    if (line.length() < count)
        return -1;

    // get address string
    std::string address = line.substr(0, count);
    SRecord.Address = ConvertString::ToUInt32(address);

    // remove address from line
    line = line.substr(count, line.length() - count);

    // CRC
    int n = count / 2;

    for (int i = 0; i < n; i++)
    {
        SRecord.CRC += static_cast<uint8_t>(SRecord.Address >> 8);
    }

    SRecord.Count += n;

    return 0;
}

int SRecord::ExtractData(std::string& line, struct SRecordStruct& SRecord)
{
    if (line.length() < 2)
        return -1;

    // n*Data - CRC
    size_t n = (line.length() / 2) - 1;

    if (n < 0)
        return -1;

    for (int i = 0; i < n; i++)
    {
        std::string hexText = line.substr(0, 2);

        uint32_t value = ConvertString::ToUInt32(hexText);

        SRecord.Data.emplace_back(static_cast<uint8_t>(value));
        SRecord.CRC += static_cast<uint8_t>(value);

        SRecord.Count++;

        // remaining
        line = line.substr(2, line.length() - 2);
    }

    SRecord.Count++;
    
    uint8_t checksum = CalculChecksum(SRecord);
    SRecord.CRC = checksum;
    
    uint32_t check = ConvertString::ToUInt32(line);

    if (checksum == static_cast<uint8_t>(check))
    {
        std::cout << line << '\n';
    }

    return 0;
}

uint8_t SRecord::CalculChecksum(const struct SRecordStruct& SRecord)
{
    //SRecord.CRC %= 256; // Modulo 256 to get the lower 8 bits    
    //uint8_t checksum = ~static_cast<uint8_t>(SRecord.CRC);// Invert all bits (one's complement)

    //return (0xFF - (SRecord.CRC & 0xFF));
    //return ((SRecord.CRC ^ 0xFF) & 0xFF);
    return (0xFF ^ (SRecord.CRC & 0xFF));
}


int SRecord::LoadFile(const std::string& filepath)
{
    // Create an input file stream
    std::ifstream inputFile(filepath);

    // Check if the file is open
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    // count the number of each type
    std::array<int, 10> type;
    memset(&type, 0, 10 * sizeof(int));


    // Read the file line by line
    std::string line;
    int count = 0;

    while (std::getline(inputFile, line))
    {
        //std::cout << line << std::endl;        

        if (line[0] != 'S')
        {
            count++;
        }
        else
        {
            ExtractLine(line);

            int n = line[1] - '0';
            if (n > 9)
            {
                std::cout << "Type S out of range\n";
            }
            else
            {
                type[n]++;
            }
        }
    }

    std::cout << "First character not S count: " << count << '\n';

    int index = 0;
    for (auto& e : type)
    {
        std::cout << 'S' << index++ << " count:" << e << '\n';
    }

    count = 0;
    int count4n = 0;
    int count8n = 0;
    int count16n = 0;
    for (const auto& s : GetList())
    {
        switch (s.Data.size())
        {
        case 4:
            count4n++;
            break;

        case 8:
            count8n++;
            break;

        case 16:
            count16n++;
            break;

        default:
            count++;
            std::cout << "Type " << std::dec << s.Type;
            std::cout << "  Address 0x" << std::hex << s.Address;
            std::cout << "  Data size: " << std::dec << s.Data.size() << '\n';
            break;
        }
    }

    std::cout << " count  :" << count << '\n';
    std::cout << " count4 :" << count4n << '\n';
    std::cout << " count8 :" << count8n << '\n';
    std::cout << " count16:" << count16n << '\n';

    // Close the file
    inputFile.close();

    return 0;
}

void SRecord::FlashWrite(SerialCom& serial)
{
    int countMillisecond = 0;

    // command erase page
    serial.Write(0x45);
    while (!serial.CheckReceiveValue(0x4B))
    {
        Sleep(1);// in milliseconds
        countMillisecond++;
    }

    std::cout << "Erase page time : " << std::dec << countMillisecond << '\n';
    

    // command debug flash
    serial.Write(0xF5);
    while (!serial.CheckReceiveValue(0x4B))
    {
        Sleep(1);// in milliseconds
    }

    // pause before begin flash
    Sleep(2);// in milliseconds

    for (const auto& frame : GetList())
    {
        // type
        serial.Write(static_cast<uint8_t>(frame.Type));

        // size
        serial.Write(static_cast<uint8_t>(frame.Data.size() + 4));

        // address
        serial.Write(static_cast<uint8_t>(frame.Address));
        serial.Write(static_cast<uint8_t>(frame.Address>>8));
        serial.Write(static_cast<uint8_t>(frame.Address>>16));
        serial.Write(static_cast<uint8_t>(frame.Address>>24));

        // data
        for(const auto& d : frame.Data)
        {
            serial.Write(d);
        }

        // response
        while (!serial.CheckReceiveValue(0x4B))
        {
            Sleep(1);// in milliseconds
        }
    }
}

/*EOF*/
