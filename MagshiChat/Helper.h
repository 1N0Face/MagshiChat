#pragma once

#include <vector>
#include <string>
#include <WinSock2.h>


enum MessageType : byte
{
	MT_CLIENT_LOG_IN = 200,
	MT_CLIENT_UPDATE = 204,
	MT_CLIENT_FINISH = 207,
	MT_CLIENT_EXIT = 208,
	MT_SERVER_UPDATE = 101,
};

// These function were given and written originally by Magshimim and we had to deal with it :)
class Helper
{
public:
	static int getMessageTypeCode(SOCKET sc);
	static int getIntPartFromSocket(SOCKET sc, int bytesNum);
	static std::string getStringPartFromSocket(SOCKET sc, int bytesNum);
	static void sendData(SOCKET sc, std::string message);
	static void send_update_message_to_client(SOCKET sc, const std::string& file_content, const std::string& second_username, const std::string& all_users);
	static std::string getPaddedNumber(int num, int digits);

private:
	static std::string getPartFromSocket(SOCKET sc, int bytesNum);
	static std::string getPartFromSocket(SOCKET sc, int bytesNum, int flags);

};


#ifdef _DEBUG // vs adds this define in debug mode
#include <stdio.h>
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
#else // we want nothing to be printed in release version
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
#define TRACE(msg, ...) // do nothing
#endif