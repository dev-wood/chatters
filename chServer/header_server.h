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
	SvRoomInfo(RoomInfoToken rtk);	//? &&�� �޾ƾ��ϳ�?
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
