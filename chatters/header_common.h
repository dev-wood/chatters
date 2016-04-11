#pragma once

#ifndef __HEADER_COMMON_H__
#define __HEADER_COMMON_H__

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>

typedef int UserKey;
typedef int RoomKey;

class UserInfoToken
{
private:
	static UserKey UniqueKey;
public:
	UserKey key;
	std::string id;
private:
	UserKey getUniqueKey();
public:
	UserInfoToken();
	UserInfoToken(const std::string& _id);
	UserInfoToken(UserInfoToken&&);
	UserInfoToken& operator=(UserInfoToken&&);
};
int UserInfoToken::UniqueKey = 0;

class RoomInfoToken
{
private:
	static RoomKey UniqueKey;
public:	
	RoomKey key;
	std::string title;
	int numOfPeer;
private:
	UserKey getUniqueKey();
public:
	RoomInfoToken();
	RoomInfoToken(const std::string& _title);
	RoomInfoToken(RoomInfoToken&&);
	RoomInfoToken& operator=(RoomInfoToken&&);
};
int RoomInfoToken::UniqueKey = 0;

class UserInfoTokenList
{
private:
	std::unordered_map<UserKey, UserInfoToken> list;
public:
	void add(UserInfoToken tk);
	bool remove(UserKey uKey);
	UserInfoToken& find(UserKey uKey);

public:

};

class RoomInfoTokenList
{
private:
	std::unordered_map<RoomKey, RoomInfoToken> list;
public:
	void add(RoomInfoToken tk);
	bool remove(RoomKey rKey);
	RoomInfoToken& find(RoomKey rKey);

public:

};

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

	std::stringstream buf;
public:
	PacketStream_Base& operator<<(const UserInfoToken& utk);
	PacketStream_Base& operator<<(const RoomInfoToken& rtk);
} PSB;

#endif