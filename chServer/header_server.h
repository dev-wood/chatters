#pragma once

#include <WinSock2.h>
#include "../chatters/header_common.h"

class SvConInfo
{
public:
	WSADATA _wsaData;
	SOCKET _sock;
	sockaddr _servAddr;
public:
	SvConInfo();	//rev 그외 constructor 구현
	SvConInfo(WSADATA wsaData, SOCKET sock, sockaddr servAddr);
private:
};

class SvUserInfo
{
public:
	UserInfoToken _utk;	// user 정보를 담고 있는 token
	int _curRmNum;		// 현재 참여 중인 채팅방. 없을 시 -1
	SOCKET _sockNum;	// 해당 user의 클라이언트와의 socket
public:
	SvUserInfo();
	SvUserInfo(const SvUserInfo& ui);
	SvUserInfo(SvUserInfo&& ui);
	SvUserInfo& operator= (SvUserInfo&& ui);
	SvUserInfo& operator= (const SvUserInfo& ui);
private:
private:
};

class SvRoomInfo
{
public:
	RoomInfoToken _rtk;
	std::vector<std::pair<UserKey, SOCKET>> _sockList;
public:
	SvRoomInfo(RoomInfoToken rtk);	//? &&로 받아야하나?
private:
private:
	SvRoomInfo();
};

class SvSndMessage: public PacketStream_Base
{
	//rev
public:

public:
private:
private:
};

class SvMach
{
	//rev
private:
private:
public:
public:
};
