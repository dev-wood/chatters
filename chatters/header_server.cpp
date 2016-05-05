#include "header_server.h"

SvConInfo::SvConInfo()
{
}
SvConInfo::SvConInfo(WSADATA wsaData, SOCKET sock, sockaddr servAddr)
{
	_wsaData = wsaData;
	_sock = sock;
	_servAddr = servAddr;
}

SvUserInfo::SvUserInfo()
{
}
SvUserInfo::SvUserInfo(const std::string & id, SOCKET sock) : _utk(id), _curRmNum(CHATTERS::NO_ROOM)
{
	_sockNum = sock;
}
SvUserInfo::SvUserInfo(const SvUserInfo & ui)
{
	_utk = ui._utk;
	_curRmNum = ui._curRmNum;
	_sockNum = ui._sockNum;
}
SvUserInfo::SvUserInfo(SvUserInfo && ui)
{
	*this = std::move(ui);
}
SvUserInfo & SvUserInfo::operator=(SvUserInfo && ui)
{
	if (this != &ui)
	{
		_utk = std::move(ui._utk);
		_curRmNum = ui._curRmNum;
		_sockNum = ui._sockNum;
	}

	return *this;
}
SvUserInfo & SvUserInfo::operator=(const SvUserInfo & ui)
{
	_utk = ui._utk;
	_curRmNum = ui._curRmNum;
	_sockNum = ui._sockNum;

	return *this;
}

SvRoomInfo::SvRoomInfo(const std::string & title) : _rtk(title)
{
	_sockList.reserve(4);
}

bool SvRoomInfo::addUser(UserKey uKey, SOCKET uSock)
{
	if (_sockList.size() < CHATTERS::MAX_PARTICIPANT) {
		_sockList.push_back(std::make_pair(uKey, uSock));
		return true;
	}
	return false;
}

bool SvRoomInfo::removeUser(UserKey uKey)
{
	//auto it = std::find_if(_sockList.cbegin(), _sockList.cend(), [=](std::pair<UserKey,SOCKET>& p)->bool {
	//	if (p.first == uKey)
	//		return true;
	//	return false;
	//});

	for (std::vector<std::pair<UserKey, SOCKET>>::const_iterator & it = _sockList.cbegin(); it != _sockList.cend(); ++it)
	{
		if (it->first == uKey) {
			_sockList.erase(it);
			return true;
		}
	}

	return false;
}

SvRoomInfo::SvRoomInfo()
{
}
