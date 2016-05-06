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
	std::cout << "SvRoomInfo() called." << std::endl;
}

//SvSndMessage::SvSndMessage(PType pt, SvMach svm)
//{
//	_callSerialization(pt, svm);
//}
SvSndMessage& SvSndMessage::operator<<(const UserInfoToken & utk)
{
	_buf << utk.get_key() << '|' << utk.get_id() << '|';

	return *this;
}
SvSndMessage& SvSndMessage::operator<<(const RoomInfoToken & rtk)
{
	_buf << rtk.get_key() << '|' << rtk.get_title() << '|' << rtk._numOfPeer << '|';

	return *this;
}
SvSndMessage & SvSndMessage::operator<<(const SvRoomInfo & ri)
{
	*this << ri._rtk;

	return *this;
}
SvSndMessage & SvSndMessage::operator<<(const SvUserInfo & ui)
{
	*this << ui._utk;

	return *this;
}
SvSndMessage::SvSndMessage()
{
	std::cout << "SvSndMessage() called." << std::endl;
}
//void SvSndMessage::_callSerialization(PType pt, SvMach svm)
//{
//	switch (pt)	//rev
//	{
//	case PSB::PType::STOC_LOGIN_ACCEPT:
//
//		break;
//
//	default:
//
//		break;		
//	}
//}
void SvSndMessage::_serialize(PType pt)
{
	_buf << ptoi(pt) << "|";
}
void SvSndMessage::_serialize(PType pt, std::unordered_map<RoomKey, SvRoomInfo> roomList)
{
	_buf << ptoi(pt) << "|" << roomList.size() << "|";
	for (const auto& a : roomList)
		*this << a.second;
}
//void SvSndMessage::_serialize(PType pt, std::vector<std::pair<UserKey, SOCKET>> userList)
//{
//	_buf << ptoi(pt) << "|" << userList.size() << "|";
//	for (const auto & a : userList) {
//		if()
//	}
//}

