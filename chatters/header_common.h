#pragma once

#ifndef __HEADER_COMMON_H__
#define __HEADER_COMMON_H__

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>

typedef int Key;
typedef int UserKey;
typedef int RoomKey;

class InfoToken
{
public:
	Key _key;
protected:
public:
protected:
	//InfoToken();
};

class UserInfoToken : public InfoToken
{
private:
	static UserKey _UniqueKey;
public:
	std::string _id;
private:
	UserKey _getUniqueKey();
public:
	UserInfoToken();
	UserInfoToken(const std::string& id);
	UserInfoToken(UserInfoToken&&);
	UserInfoToken& operator=(UserInfoToken&&);
};
__declspec(selectany) UserKey UserInfoToken::_UniqueKey = 0;

class RoomInfoToken : public InfoToken
{
private:
	static RoomKey _UniqueKey;
public:	
	std::string _title;
	int _numOfPeer;
private:
	UserKey _getUniqueKey();
public:
	RoomInfoToken();
	RoomInfoToken(const std::string& _title);
	RoomInfoToken(RoomInfoToken&&);
	RoomInfoToken& operator=(RoomInfoToken&&);
};
__declspec(selectany) RoomKey RoomInfoToken::_UniqueKey = 0;

class PacketStream_Base
{
public:
	enum struct PType : int
	{
		// Client to Server packet
		CTOS_LOGIN_REQUEST = 0,
		
		CTOS_LOBBY_JOINROOM,
		CTOS_LOBBY_LOADLIST,
		CTOS_LOBBY_CREATEROOM,
		
		CTOS_CREATEROOM_CREATEROOM,
		
		CTOS_CHAT_QUITROOM,
		CTOS_CHAT_MYSTATEREADY,
		CTOS_CHAT_MYSTATENOTREADY,
		CTOS_CHAT_ROOMSTATEPLAYING,
		CTOS_CHAT_ROOMSTATENOTPLAYING,
		CTOS_CHAT_CHAT,

		// Server to Client packet
		STOC_LOGIN_ACCEPT,
		STOC_LOGIN_FAIL,
		
		STOC_LOBBY_CREATEROOMOK,
		STOC_LOBBY_CREATEROOMFAIL,
		STOC_LOBBY_JOINROOMOK,
		STOC_LOBBY_JOINROOMFAIL,
		STOC_LOBBY_LOADLIST,
		
		STOC_CREATEROOM_OK,
		STOC_CREATEROOM_FAIL,
		
		STOC_CHAT_QUITUSER,
		STOC_CHAT_GUESTSTATEREADY,
		STOC_CHAT_GUESTSTATENOTREADY,
		STOC_CHAT_ROOMSTATEPLAYING,
		STOC_CHAT_ROOMSTATENOTPLAYING,
		STOC_CHAT_CHAT
	};
};
typedef PacketStream_Base PSB;

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

class PacketStreamList
{
public:
	virtual PacketStream& peek() = 0;
	virtual bool empty() = 0;
	virtual void clear() = 0;
};

class RcvMessage : public PacketStream_Base
{
public:
	PType _rqType;
	bool _sharable;
	std::stringstream _buf;
protected:

public:
	RcvMessage(PType, bool, std::stringstream);
	RcvMessage(RcvMessage&&);
	RcvMessage& operator= (RcvMessage&&);
};

class RcvMessageList
{
public:
	void push_back(RcvMessage);
	void pop();
	RcvMessage& peek();
	bool empty();
	void clear();
protected:
public:
protected:
	std::queue<RcvMessage, std::list<RcvMessage>> _queue;
};



#endif