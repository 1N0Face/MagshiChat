#include "WSAInitializer.h"
#include <exception>

//This function is the constructor
WSAInitializer::WSAInitializer()
{
	WSADATA wsa_data = { };
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		throw std::exception("WSAStartup Failed");
}
//This function is the destructor
WSAInitializer::~WSAInitializer()
{
	try
	{
		WSACleanup();
	}
	catch (...) {}
}
