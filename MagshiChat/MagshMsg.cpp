#include "MagshMsg.h"
//This function builds the Magshi message
MagshMsg::MagshMsg(const std::string data, const std::string author, const std::string secUser)
{
	_data = data; 
	_author = author;
	_secUser = secUser;
	_msg = "&MAGSH_MESSAGE&&Author&" + author + "&DATA&" + data;
}
MagshMsg::~MagshMsg() {}
//This function returns the msg in the desirable format
std::string MagshMsg::getMsg() const
{
	return _msg;
}
//This function returns the author name
std::string MagshMsg::getAuthor() const
{
	return _author;
}
//This function returns the user name
std::string MagshMsg::getUser() const
{
	return _secUser;
}
//This function returns data of the message
std::string MagshMsg::getData() const
{
	return _data;
}