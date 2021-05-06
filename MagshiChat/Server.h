#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <thread>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include "Helper.h"
#include <queue>
#include <mutex>
#include <algorithm>
#include "MagshMsg.h"
class Server
{
public:
	Server(); //constructor
	~Server(); //destructor
	void serve(); //This function serves the server

private:
	//Functions
	void accept(); //This function accepts the client and create a specific socket from server to this client
	void clientHandler(SOCKET clientSocket);//This function handles the conversation with the client
	std::string updateUsers() const; // This function return the output string from user list
	std::string makeFileName(const MagshMsg mm) const; //This function build the file name for the users conversations
	void saveChat(); //This function saves users conversation from the chat to the txt file
	std::string readChat() const; //This function reads from the chat and returns string with all the messages
	std::string getConfig(const std::string ipORport) const;//This function returns the ip or the port from the config file


	//Fiels
	std::mutex m;
	std::condition_variable cond;
	SOCKET _serverSocket;
	std::vector<std::string> _users;
	std::string _file;
	std::queue<MagshMsg> _messages;
};
bool compNames(std::string name1, std::string name2); //This function helps compare strings for the std::sort function