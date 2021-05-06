#pragma once
#include <string>
class MagshMsg
{
public:
	MagshMsg(const std::string data, const std::string author, const std::string secUser);
	~MagshMsg();
	std::string getMsg() const; // returns the message in the desirable format
	std::string getAuthor() const; // returns the author name
	std::string getUser() const; // returns the user name
	std::string getData() const; //returns the data of the message
private:
	std::string _author;
	std::string _secUser;
	std::string _data;
	std::string _msg;
};