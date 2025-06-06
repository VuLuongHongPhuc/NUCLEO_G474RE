#pragma once
#include <iostream>
#include <mutex>
#include <Windows.h>
#include "SerialCom.h"

class Manager
{
private:
    // Private constructor to prevent direct instantiation
    Manager(SerialCom& serial) : m_Serial(serial)
    {
        std::cout << "Singleton instance created.\n";
    }
        
    ~Manager() {};

    // Delete copy constructor and assignment operator to prevent copies
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;


    

// Attributes
private:
    SerialCom& m_Serial;
    std::mutex m_MutexConnect;
    std::mutex m_MutexFlash;
    

public:
    // Static method to get the single instance
    static Manager& getInstance(SerialCom& serial) {
        static Manager instance(serial); // Guaranteed to be created only once (C++11 thread-safe)
        return instance;
    }

    void showMessage() 
    {
        std::cout << "I am unique!\n";
    }

    static void ConsoleClear();
    static void ConsoleMenuFlash();
    void ExecuteCommand(const char& c);

    void FlashFirmware();
    
};

