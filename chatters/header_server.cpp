#include "header_server.h"

/* SvConInfo class definition */
SvConInfo::SvConInfo()
{
}
SvConInfo::SvConInfo(WSADATA newWsaData, SOCKET newSock, sockaddr newServAddr)
{
	wsaData = newWsaData;
	sock = newSock;
	servAddr = newServAddr;
}

/* SvUserInfo class definition */
SvUserInfo::SvUserInfo(const std::string & id, std::shared_ptr<HandleData> hData) 
	: utk(id), 
	curRmNum(CHATTERS::NO_ROOM),
	spHData(hData)
{
	// left blank intentionally
}
SvUserInfo::SvUserInfo(const SvUserInfo & ui)
{
	*this = ui;
}
SvUserInfo::SvUserInfo(SvUserInfo && ui)
{
	*this = std::move(ui);
}
SvUserInfo & SvUserInfo::operator=(SvUserInfo && ui)
{
	if (this != &ui)
	{
		utk = std::move(ui.utk);
		curRmNum = ui.curRmNum;
		spHData = ui.spHData;
	}

	return *this;
}
SvUserInfo & SvUserInfo::operator=(const SvUserInfo & ui)
{
	if (this != &ui)
	{
		utk = ui.utk;
		curRmNum = ui.curRmNum;
		spHData = ui.spHData;
	}

	return *this;
}

/* SvRoomInfo definition */
SvRoomInfo::SvRoomInfo(const std::string & title) : rtk(title)
{
	userList.reserve(CHATTERS::MAX_PARTICIPANT);
}
bool SvRoomInfo::addUser(UserKey uKey)
{
	if (userList.size() < CHATTERS::MAX_PARTICIPANT) {
		userList.push_back(uKey);
		return true;
	}
	return false;
}
bool SvRoomInfo::removeUser(UserKey uKey)
{
	for (std::vector<UserKey>::const_iterator & it = userList.cbegin(); it != userList.cend(); ++it)
	{
		if (*it == uKey) {
			userList.erase(it);
			return true;
		}
	}

	return false;
}
SvRoomInfo::SvRoomInfo()
{
	std::cout << "SvRoomInfo() called." << std::endl;
}

