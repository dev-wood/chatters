#pragma once

#ifndef __HEADER_COMMON_H__
#define __HEADER_COMMON_H__

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <WinSock2.h>

typedef int Key;
typedef int UserKey;
typedef int RoomKey;

namespace CHATTERS
{
	const int MAX_PARTICIPANT = 4;
	const int NO_ROOM = -1;
	const int NUM_OF_ROOM_PER_PAGE = 20;
}

class InfoToken
{
public:
protected:
	Key _key;
public:
protected:
	InfoToken();
	~InfoToken();
};

class UserInfoToken : public InfoToken
{
private:
	static UserKey _UniqueKey;
	std::string _id;
public:
private:
	UserKey _getUniqueKey();
public:
	UserInfoToken();
	UserInfoToken(const std::string& id);
	UserInfoToken(const UserInfoToken&);
	UserInfoToken(UserInfoToken&&);
	UserInfoToken& operator=(UserInfoToken&&);
	UserInfoToken& operator=(const UserInfoToken& utk);

	// accessor	
	UserKey get_key() const;
	std::string get_id() const;
};
__declspec(selectany) UserKey UserInfoToken::_UniqueKey = 0;
std::ostream& operator << (std::ostream&, const UserInfoToken&);

class RoomInfoToken : public InfoToken
{
private:
	static RoomKey _UniqueKey;
	std::string _title;
public:
	int _numOfPeer;
private:
	UserKey _getUniqueKey();
public:
	RoomInfoToken();
	RoomInfoToken(const std::string& _title);
	RoomInfoToken(const RoomInfoToken&);
	RoomInfoToken(RoomInfoToken&&);
	RoomInfoToken& operator=(RoomInfoToken&&);
	RoomInfoToken& operator=(const RoomInfoToken& rtk);	
	~RoomInfoToken();

	// accessor
	RoomKey get_key() const;
	std::string get_title() const;
};
__declspec(selectany) RoomKey RoomInfoToken::_UniqueKey = 0;
std::ostream& operator<< (std::ostream&, const RoomInfoToken&);

class PacketStream_Base
{
public:
	enum struct PType : int
	{
		// Client to Server packet
		CTOS_LOGIN_REQUEST = 0,
		
		CTOS_LOBBY_JOINROOM,
		CTOS_LOBBY_LOAD_ROOMLIST,
		
		CTOS_CREATEROOM_CREATEROOM,
		
		CTOS_CHAT_QUITROOM,
		CTOS_CHAT_CHAT,
		// 추후 확장 가능한 기능
		CTOS_CHAT_MYSTATEREADY,
		CTOS_CHAT_MYSTATENOTREADY,
		CTOS_CHAT_ROOMSTATEPLAYING,
		CTOS_CHAT_ROOMSTATENOTPLAYING,
		

		// Server to Client packet
		STOC_LOGIN_ACCEPT,
		STOC_LOGIN_FAIL,
		
		STOC_LOBBY_JOINROOMOK,
		STOC_LOBBY_JOINROOMFAIL,
		STOC_LOBBY_LOAD_ROOMLIST,
		
		STOC_CREATEROOM_OK,
		STOC_CREATEROOM_FAIL,
		
		STOC_CHAT_QUITUSER,
		STOC_CHAT_CHAT,
		STOC_CHAT_LOAD_USERLIST,
		// 추후 확장 가능한 기능 
		STOC_CHAT_GUESTSTATEREADY,
		STOC_CHAT_GUESTSTATENOTREADY,
		STOC_CHAT_ROOMSTATEPLAYING,
		STOC_CHAT_ROOMSTATENOTPLAYING
		
	};
};
typedef PacketStream_Base PSB;

int ptoi(PSB::PType pt);

class PacketStream : public PacketStream_Base
{
public:
	std::stringstream _buf;
protected:

public:
	virtual void serialize(PType) = 0;
	int extractReqType();
	virtual bool determineShrType(PType) = 0;
	PacketStream& operator<<(const UserInfoToken & utk);
	PacketStream& operator<<(const RoomInfoToken & rtk);
};

class RcvMessage : public PacketStream_Base
{
public:
	PType _rqType;
	bool _sharable;
	std::stringstream _buf;
protected:

public:
	RcvMessage();
	RcvMessage(PType, bool, std::stringstream);
	RcvMessage(const RcvMessage&);
	RcvMessage(RcvMessage&&);
	RcvMessage& operator= (RcvMessage&&);
};

class MachObject
{
public:
	virtual void init() = 0;

protected:
	MachObject()
	{
		// left blank intentionally
	}
};



#endif