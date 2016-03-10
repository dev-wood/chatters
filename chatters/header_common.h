#pragma once

#ifndef __HEADER_COMMON_H__
#define __HEADER_COMMON_H__

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <map>

typedef int UserKey;
typedef int RoomKey;

class UserInfoToken
{
private:
	static int UniqueKey;
public:
	UserKey key;
	std::string id;
public:
	UserInfoToken();
	UserInfoToken(const std::string& _id);
};
int UserInfoToken::UniqueKey = 0;

class RoomInfoToken
{
private:
	static int UniqueKey;
public:	
	RoomKey key;
	std::string title;
	int numOfPeer;
public:
	RoomInfoToken();
	RoomInfoToken(const std::string& _title);

};
int RoomInfoToken::UniqueKey = 0;



#endif