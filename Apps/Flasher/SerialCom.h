#pragma once

#include <Windows.h>

class SerialCom
{
public:
	
	SerialCom(std::string name);
		

	int Open();
	void Write(const uint8_t& c);
	
	char Read();
	bool CheckReceiveValue(const uint8_t value);

	bool IsOpen() { return m_IsOpen; };

private:
	void SetConfig();

private:
	std::string m_Name;
	HANDLE m_hUart;
	bool m_IsOpen;

};

