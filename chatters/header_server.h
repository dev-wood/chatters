#pragma once

#include "header_common.h"
#include <memory>
#include <WinSock2.h>

/*********************************************************************
 * HandleData class
	- listen socket을 통해 새로 연결되는 클라이언트 정보를 저장하는
	 클래스
 *********************************************************************/
typedef struct HandleData
{
public:
	/* Member method */
public:
	/* Member field */
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
private:
	/* Member method */
private:
	/* Member field */
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

/*********************************************************************
 * SvConInfo class
	- Server의 연결관련 정보가 저장하는 클래스
 *********************************************************************/
class SvConInfo
{
public:
	/* Member method */
	SvConInfo();	//rev 그외 constructor 구현
	SvConInfo(WSADATA newWsaData, SOCKET newSock, sockaddr newServAddr);
public:
	/* Member field */
	WSADATA wsaData;
	SOCKET sock;
	sockaddr servAddr;
private:
	/* Member method */
private:
	/* Member field */
};

/*********************************************************************
 * SvUserInfo class
	- server에 접속 중인 client의 user information을 저장하는  클래스
 *********************************************************************/
class SvUserInfo
{
public:
	/* Member method */
	SvUserInfo(const std::string& id, std::shared_ptr<HandleData> hData);

	SvUserInfo(const SvUserInfo& ui);
	SvUserInfo(SvUserInfo&& ui);
	SvUserInfo& operator= (SvUserInfo&& ui);
	SvUserInfo& operator= (const SvUserInfo& ui);
public:
	/* Member field */
	UserInfoToken utk;	// user 정보를 담고 있는 token
	int curRmNum;		// 현재 참여 중인 채팅방. 없을 시 -1
	std::shared_ptr<HandleData> spHData;
private:
	/* Member method */
	SvUserInfo();
private:
	/* Member field */
};

/*********************************************************************
 * SvRoomInfo class
	- 현재 생성되어 있는 채팅방 관련 정보를 저장하는 클래스

 *********************************************************************/
class SvRoomInfo
{
public:
	/* Member method */
	SvRoomInfo(const std::string& title);

	bool addUser(UserKey uKey);
	bool removeUser(UserKey uKey);
public:
	/* Member field */
	RoomInfoToken rtk;
	std::vector<UserKey> userList;

private:
	/* Member method */
	SvRoomInfo();
private:
	/* Member field */
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
