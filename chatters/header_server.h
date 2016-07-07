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

class SvSndMessage: public PacketStream
{
public:	// member

public:	// methods
	SvSndMessage(PType, SvMach);	//rev
	
	SvSndMessage& operator<< (const UserInfoToken & utk);
	SvSndMessage& operator<< (const RoomInfoToken & rtk);
	SvSndMessage& operator<< (const SvRoomInfo&);
	SvSndMessage& operator<< (const SvUserInfo&);

private:
private:	//rev
	SvSndMessage();
	//void _callSerialization(PType pt, SvMach svm);	
	void _serialize(PType pt);	// Packet type �� ������ packet
	void _serialize(PType pt, std::unordered_map<RoomKey, SvRoomInfo> roomList);	// room list ������ ������ packet
	//void _serialize(PType pt, std::vector<std::pair<UserKey, SOCKET>> userList);	//rev user info�� �ʿ��� �� �ִµ� �̷� �Ķ���� ���·� �ʿ��� ���� �ƴѵ�.. ���� �ʿ�.
};

class SvMach
{
	//rev
public:
public:

private:
	std::unordered_map<UserKey, SvUserInfo> _uList;
	std::unordered_map<RoomKey, SvRoomInfo> _rList;

	std::deque<std::pair<SOCKET, RcvMessage>> _rcvMsgQue;
	std::deque<std::pair<std::vector<SOCKET>, SvSndMessage>> _sndMsgQue;
private:

	friend SvSndMessage;
};
