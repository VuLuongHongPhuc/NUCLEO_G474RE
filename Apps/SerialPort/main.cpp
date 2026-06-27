#include <iostream>
#include <stdint.h>
//#include <conio.h>  // _getch()
#include <Windows.h> // fileapi.h : ReadFile, WriteFile
#include <string>
#include <tchar.h>
#include <thread>
#include <chrono>
#include <sstream> // ostringstream, stringstream
#include <iomanip> // setw
#include <map>


#define TTL_COM_PORT "COM9"
//#define TTL_COM_PORT "\\\\.\\COMx"


bool OpenCom(HANDLE& handle);
void Write(HANDLE handle, const uint8_t& c);
void Read(HANDLE handle);


int main()
{
    HANDLE hSerial;
    bool stop = false;

    if (!OpenCom(hSerial))
    {
        return EXIT_SUCCESS;
    }

    std::thread t([&] {

        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            Read(hSerial);

        }while(!stop);

        std::cout << "I am out.\n";
    });

    char c;
    std::cin >> c;

    stop = true;
    t.join(); // Wait for the serial thread to finish
    std::cout << "Threads finished.\n";
    
    std::cout << "Exiting main thread.\n";
    
    return EXIT_SUCCESS;
}

bool OpenCom(HANDLE& handle)
{
    // open com port
    handle = CreateFile(TTL_COM_PORT, // drive name to open
        GENERIC_READ | GENERIC_WRITE, // must be opened with exclusive access(No Sharing)
        0,                            // no access to the drive
        NULL,                         // default security attributes
        OPEN_EXISTING,                // disposition i.e. if file already exist
        FILE_ATTRIBUTE_NORMAL,        // file attributes
        NULL);                        // do not copy file attributes

    if (INVALID_HANDLE_VALUE == handle)
    {
        DWORD error = GetLastError();
        std::cout << "Error: " << error << '\n';
        return false;
    }
    
    DCB dcbSerialParams;
    // set com port settings
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(handle, &dcbSerialParams))
    {
        std::cout << "Could not get COM port settings!\n";
        return false;
    }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;
    if (!SetCommState(handle, &dcbSerialParams))
    {
        std::cout << "Could not set COM port settings!\n";
        return false;
    }

    COMMTIMEOUTS timeouts;
    // set com port timeouts
    timeouts.ReadIntervalTimeout = MAXDWORD; // MAXDWORD -> return immediately with any bytes available
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    // Total read timeout = (number of bytes to read * ReadTotalTimeoutMultiplier ) + ReadTotalTimeoutConstant

    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    // Total write timeout = (number of bytes to write * WriteTotalTimeoutMultiplier) + WriteTotalTimeoutConstant

    if (!SetCommTimeouts(handle, &timeouts))
    {
        std::cout << "Could not set COM port timeouts!\n";
        return false;
    }

    return true;
}

void Write(HANDLE handle, const uint8_t& c)
{
    uint8_t buf[2];
    DWORD numberOfBytesWriten = 0;

    buf[0] = c;

    if (!WriteFile(handle, buf, 1, &numberOfBytesWriten, NULL))
    {
        DWORD error = GetLastError();
        std::cout << "ERROR WRITE - Code error : 0x" << std::hex << error << "\n";
        std::cout << __LINE__ << " - " << __FUNCTION__ << " - " << __FILE__ << "\n";
    }

    std::ostringstream oss;

    oss << std::setw(2) << std::hex << std::setfill('0');
    oss << "W:";

    for (int i = 0; i < numberOfBytesWriten; i++)
    {
        oss << " 0x" << static_cast<int>(buf[i]);
    }

    std::cout << oss.str() << '\n';
}

void Read(HANDLE handle)
{
    #define RX_BUF_SIZE   256U
    uint8_t buf[RX_BUF_SIZE+1] = { 0 };
    DWORD numberOfBytesRead = 0;

    if (!ReadFile(handle, buf, RX_BUF_SIZE, &numberOfBytesRead, NULL))
    {
        DWORD error = GetLastError();

        // ERROR_INVALID_HANDLE = 6
        std::cout << "ERROR READ - Code: " << error << "\n";
        std::cout << __LINE__ << " - " << __FUNCTION__ << " - " << __FILE__ << "\n";
        return;
    }

    if (numberOfBytesRead == 0)
        return;
    
    std::ostringstream oss;

    //oss << std::setw(2) << std::hex << std::setfill('0');
    oss << "R: ";

    for (int i = 0; i < numberOfBytesRead; i++)
    {
        if (buf[i] == '\0')
            break;
            
        oss << static_cast<char>(buf[i]);
    }

    std::cout << oss.str() << '\n';
}

/*EOF*/
