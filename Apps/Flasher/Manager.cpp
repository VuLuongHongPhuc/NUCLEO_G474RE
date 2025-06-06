#include <thread> // sleep_for
#include <chrono>

#include "Manager.h"
#include "SRecord.h"



void Manager::ExecuteCommand(const char& c)
{
    switch (c)
    {
    case 'f':
        FlashFirmware();
        break;

    case 'r':
        break;

    case 'c':
        break;

    case 'l':
        if (m_Serial.IsOpen())
        {
            m_Serial.Write(c);
            Sleep(10);// in milliseconds
            m_Serial.Read();
        }
        break;

    default:
        break;
    }


}


void Manager::ConsoleClear()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD consoleSize, charsWritten;

    // Get the size of the console screen buffer
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the screen buffer with spaces
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, { 0, 0 }, &charsWritten);
    // Reset the cursor position
    SetConsoleCursorPosition(hConsole, { 0, 0 });
}

void Manager::ConsoleMenuFlash()
{
    std::cout << "f - Flash\n";
    std::cout << "r - Read program checksum\n";
    std::cout << "t - Toggle light\n";
    std::cout << "c - Clear console\n";
    std::cout << "q - Exit program\n";
    std::cout << "\nEnter command : ";
}

void Manager::FlashFirmware()
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
}