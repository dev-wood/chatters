#pragma once

#include "header_common.h"
#include <WinSock2.h>

class SvConInfo
{
public:
	WSADATA _wsaData;
	SOCKET _sock;
	sockaddr _servAddr;
public:
	SvConInfo();	//rev �׿� constructor ����
	SvConInfo(WSADATA wsaData, SOCKET sock, sockaddr servAddr);
private:
};

class SvUserInfo
{
public:
	UserInfoToken _utk;	// user ������ ��� �ִ� token
	int _curRmNum;		// ���� ���� ���� ä�ù�. ���� �� -1
	SOCKET _sockNum;	// �ش� user�� Ŭ���̾�Ʈ���� socket
public:
	SvUserInfo(const std::string& id, SOCKET sock);

	SvUserInfo(const SvUserInfo& ui);
	SvUserInfo(SvUserInfo&& ui);
	SvUserInfo& operator= (SvUserInfo&& ui);
	SvUserInfo& operator= (const SvUserInfo& ui);
private:
private:
	SvUserInfo();
};

class SvRoomInfo
{
public:
	RoomInfoToken _rtk;
	std::vector<std::pair<UserKey, SOCKET>> _sockList;
public:
	SvRoomInfo(const std::string& title);

	bool addUser(UserKey uKey, SOCKET uSock);
	bool removeUser(UserKey uKey);
private:
private:
	SvRoomInfo();
};

//class SvSndMessage: public PacketStream_Base
//{
//	//rev
//public:
//
//public:
//private:
//private:
//};
//
//class SvMach
//{
//	//rev
//private:
//private:
//public:
//public:
//};
