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
	InfoToken();
	~InfoToken();
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
	UserInfoToken(const UserInfoToken&);
	UserInfoToken(UserInfoToken&&);
	UserInfoToken& operator=(UserInfoToken&&);
};
__declspec(selectany) UserKey UserInfoToken::_UniqueKey = 0;
std::ostream& operator << (std::ostream&, const UserInfoToken&);

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
	RoomInfoToken(const RoomInfoToken&);
	RoomInfoToken(RoomInfoToken&&);
	RoomInfoToken& operator=(RoomInfoToken&&);
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
	RcvMessage();
	RcvMessage(PType, bool, std::stringstream);
	RcvMessage(const RcvMessage&);
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