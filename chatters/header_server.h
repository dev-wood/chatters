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
	void _serialize(PType pt);	// Packet type 만 가지는 packet
	void _serialize(PType pt, std::unordered_map<RoomKey, SvRoomInfo> roomList);	// room list 정보를 가지는 packet
	//void _serialize(PType pt, std::vector<std::pair<UserKey, SOCKET>> userList);	//rev user info가 필요할 수 있는데 이런 파라미터 형태로 필요한 것은 아닌데.. 수정 필요.
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
