#include <iostream>
#include <stdint.h>
//#include <conio.h>  // _getch()
#include <windows.h> // Sleep
#include <string>




//#version 100 core


#include "SerialCom.h"
#include "SRecord.h"



int main()
{
    SRecord file;
    file.LoadFile("power_manager.srec");

    
    SerialCom serial("ToDoLater");
    if (serial.Open() == -1)
    {
        std::cout << "Error Open COM\n";
    }
    else
    {
        file.FlashWrite(serial);
    }


    std::string input;
    char c = 0;
    do
    {
        std::cin >> input;

        if (input.size() != 1)
            continue;

        //const char * c = input.c_str();
        //std::cout << ch[0] << '\n';

        c = input.at(0);

        if (c != 'q')
        {
            serial.Write(c);
            Sleep(10);// in milliseconds
            serial.Read();
        }
        
    } while (c != 'q');

    return EXIT_SUCCESS;
}

