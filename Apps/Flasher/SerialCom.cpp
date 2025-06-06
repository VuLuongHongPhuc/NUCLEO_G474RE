#include <iostream>


#include "SerialCom.h"

//#define TTL_COM_PORT L"COM9"
#define TTL_COM_PORT L"\\\\.\\COM16"    // when greater than 10, you need to prefix with "\\\\.\\"
#define DEF_RX_BUFFER_SIZE 80

//static HANDLE m_hUart = INVALID_HANDLE_VALUE;


SerialCom::SerialCom(std::string name) 
    : m_Name(name),
    m_hUart(INVALID_HANDLE_VALUE),
    m_IsOpen(false)
{
    
}


int SerialCom::Open()
{
    // open com port
    m_hUart = CreateFile(TTL_COM_PORT, // drive name to open
        GENERIC_READ | GENERIC_WRITE,       // must be opened with exclusive access(No Sharing)
        0,                                  // no access to the drive
        NULL,                               // default security attributes
        OPEN_EXISTING,                      // disposition i.e. if file already exist
        FILE_ATTRIBUTE_NORMAL,              // file attributes
        NULL);                              // do not copy file attributes

    if (INVALID_HANDLE_VALUE == m_hUart)
    {
        //DWORD error = GetLastError();
        // error
        return -1;
    }
    else
    {
        SetConfig();

        // vide le buffer
        Read();


        m_IsOpen = true;
    }

    return 0;
}

void SerialCom::SetConfig()
{
    DCB dcbSerialParams;
    // set com port settings
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(m_hUart, &dcbSerialParams))
    {
        std::cout << "Could not get COM port settings!\n";
    }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(m_hUart, &dcbSerialParams))
    {
        std::cout << "Could not set COM port settings!\n";
    }

    COMMTIMEOUTS timeouts;
    // set com port timeouts
    timeouts.ReadIntervalTimeout = MAXDWORD; // MAXDWORD -> return immediately with any bytes available
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    // Total read timeout = (number of bytes to read * ReadTotalTimeoutMultiplier ) + ReadTotalTimeoutConstant

    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    // Total write timeout = (number of bytes to write * WriteTotalTimeoutMultiplier) + WriteTotalTimeoutConstant

    if (!SetCommTimeouts(m_hUart, &timeouts))
    {
        std::cout << "Could not set COM port timeouts!\n";
    }
}

void SerialCom::Write(const uint8_t& c)
{
    uint8_t buf[1] = { 0x55 };
    DWORD bytes_write = 0;

    buf[0] = c;

    if (!WriteFile(m_hUart, buf, 1, &bytes_write, NULL))
    {
        DWORD error = GetLastError();

        std::cout << "Error write !\n";
    }
    else
    {
        std::cout << "Write 0x" << std::hex << (int)buf[0] << '\n';
    }
}



char SerialCom::Read()
{
    char buf[DEF_RX_BUFFER_SIZE + 1] = { 0 };
    DWORD bytes_read = 0;


    if (!ReadFile(m_hUart, buf, DEF_RX_BUFFER_SIZE, &bytes_read, NULL))
    {
        DWORD error = GetLastError();

        std::cout << "Error read !\n";
    }
    else
    {
        if (bytes_read)
        {
            std::cout << "Read 0x" << std::hex << (int)buf[0] << '\n';
        }
        else
        {
            std::cout << "Rx empty\n";
        }
    }

    return buf[0];
}

bool SerialCom::CheckReceiveValue(const uint8_t value)
{
    uint8_t buf[8] = { 0 };
    DWORD bytes_read = 0;
    bool ret = false;

    if (!ReadFile(m_hUart, buf, 1, &bytes_read, NULL))
    {
        DWORD error = GetLastError();

        std::cout << "[Error] Serial Read code : " << std::hex << error << '\n';
    }
    else
    {
        if (bytes_read == 1)
        {
            std::cout << "Read 0x" << std::hex << (int)buf[0] << '\n';

            if (value == buf[0])
                ret = true;
        }
    }

    return ret;
}

/*EOF*/
