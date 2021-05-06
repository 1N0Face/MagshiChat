#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
Server::Server()
{
	std::cout << "Starting..." << std::endl;
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP server
	if (_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}
Server::~Server()
{
	try
	{
		closesocket(_serverSocket); //free resources that was allocated in the constructor
	}
	catch (...) {}
}
//This function serves the server
void Server::serve()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(atoi(getConfig("port").c_str())); // port that server will listen for
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(getConfig("server_ip").c_str());
	if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) // Connects between the socket and the configuration
		throw std::exception(__FUNCTION__ " - bind");
	else
		std::cout << "binded" << std::endl;
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)// Start listening for incoming requests of clients
		throw std::exception(__FUNCTION__ " - listen");
	else
		std::cout << "listening..." << std::endl;
	std::thread(&Server::saveChat, this).detach(); // pop messages and saves them to txt file
	while (true)
		accept();
}
//This function accepts the client and create a specific socket from server to this client
void Server::accept()
{
	SOCKET client_socket = ::accept(_serverSocket, NULL, NULL);
	std::cout << "accepting client.." << std::endl;
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);
	else
		std::cout << "Client accepted !" << std::endl;
	std::thread(&Server::clientHandler, this, client_socket).detach();
}
// This function handles the conversation with the client
void Server::clientHandler(SOCKET clientSocket)
{
	Helper h;
	std::string userName, secUserName, message, onlineUsers;
	try
	{
		while (true)
		{
			int typeCode = h.getMessageTypeCode(clientSocket);
			if (typeCode == MT_CLIENT_LOG_IN)
			{
				userName = h.getStringPartFromSocket(clientSocket, h.getIntPartFromSocket(clientSocket, 2)); //grab the username from the client
				_users.push_back(userName);
				onlineUsers = updateUsers();
				std::cout << "ADDED new client: " << clientSocket << ", " << userName << " to clients list" << std::endl;
				h.send_update_message_to_client(clientSocket, "", "", onlineUsers); //send login update msg
			}
			if (typeCode == MT_CLIENT_UPDATE)
			{
				secUserName = h.getStringPartFromSocket(clientSocket, h.getIntPartFromSocket(clientSocket, 2)); //grab the second username
				message = h.getStringPartFromSocket(clientSocket, h.getIntPartFromSocket(clientSocket, 5)); //get the message that was sent
				MagshMsg mm(message, userName, secUserName); // make an MagshMsg class to save her for next actions
				if (!userName.empty() && !secUserName.empty())
					_file = makeFileName(mm);
				std::unique_lock<std::mutex> locker(m);
				if (!message.empty())
					_messages.push(mm);
				locker.unlock();
				cond.notify_one();
				onlineUsers = updateUsers();
				message = readChat();
				h.send_update_message_to_client(clientSocket, message, secUserName, onlineUsers); //send update msg
			}
		}
		closesocket(clientSocket);
	}
	catch (const std::exception& e)
	{
		if (!_users.empty())
		{
			_users.erase(std::remove(_users.begin(), _users.end(), userName), _users.end()); //remove the offline user
			onlineUsers = updateUsers(); // update the users list
		}
		std::cout << "REMOVED " << clientSocket << ", " << userName << " from clients list" << std::endl;
		closesocket(clientSocket);
	}
}
//This function return the output string from user list
std::string Server::updateUsers() const
{
	std::string output;
	for (int i = 0; i < _users.size(); i++)
	{
		if (i + 1 != _users.size())
			output += _users[i] + "&";
		else
			output += _users[i];
	}
	return output;
}
//This function helps compare strings for the std::sort function
bool compNames(std::string name1, std::string name2)
{
	return name1 < name2;
}
//This function build the file name for the users conversations
std::string Server::makeFileName(const MagshMsg mm) const
{
	std::string fileName;
	std::vector<std::string> names;
	names.push_back(mm.getAuthor());
	names.push_back(mm.getUser());
	std::sort(names.begin(), names.end(), compNames);
	fileName = names[0] + "&" + names[1];
	return fileName;
}
//This function saves users conversation from the chat to the txt file
void Server::saveChat()
{
	std::ofstream file;
	std::unique_lock<std::mutex> locker(m);
	while (true)
	{
		cond.wait(locker);
		if (!_file.empty())
		{
			file.open(_file + ".txt", std::ios_base::app);
			while (!_messages.empty())
			{
				file << _messages.front().getMsg(); //save the msg in the desirable format
				_messages.pop();
			}
			file.close();
		}
	}
}
//This function reads from the chat and returns string with all the messages
std::string Server::readChat() const
{
	std::ifstream myfile(_file + ".txt");
	std::string output, line;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
			output += line;
			myfile.close();
	}
	return output;
}
//This function returns the ip or the port from the config file
//std::string should be "port" to get port and "server_ip" to get the ip
std::string Server::getConfig(const std::string ipORport) const
{
	std::string line;
	std::ifstream config("config.txt");
	if (config.is_open())
	{
		while (getline(config, line))
		{
			if (line.find(ipORport) != std::string::npos)
			{
				line = line.substr(ipORport.size() + 1, line.size() - ipORport.size()); //from the line it gets the value(port or ip)
				config.close();
				return line;
			}
		}
		config.close();
	}
	else
		throw std::exception(__FUNCTION__ " - config");
}
