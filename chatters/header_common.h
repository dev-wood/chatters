#pragma once

#ifndef __HEADER_COMMON_H__
#define __HEADER_COMMON_H__

#define NOMINMAX

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
	/* Member method */
	virtual ~InfoToken();
public:
	/* Member field */
protected:
	/* Member method */
	InfoToken();
	InfoToken(Key key);
protected:
	/* Member field */
	Key _key;
};

class UserInfoToken : public InfoToken
{
public:
	/* Member method */
	UserInfoToken();
	UserInfoToken(const std::string& id);
	UserInfoToken(const UserInfoToken&);
	UserInfoToken(UserInfoToken&&);
	virtual ~UserInfoToken();
	
	UserInfoToken& operator=(UserInfoToken&&);
	UserInfoToken& operator=(const UserInfoToken& utk);

	// accessor	
	UserKey get_key() const;
	std::string get_id() const;

	void operator<<(std::stringstream& sstream);
public:
	/* Member field */
private:
	/* Member method */
	UserKey _getUniqueKey();
private:
	/* Member field */
	static UserKey _UniqueKey;
	std::string _id;

	//friend std::stringstream& operator<<(std::stringstream& sstream, UserInfoToken& utk);
};
__declspec(selectany) UserKey UserInfoToken::_UniqueKey = 0;
std::ostream& operator << (std::ostream&, const UserInfoToken&);

std::stringstream& operator<<(std::stringstream& sstream, UserInfoToken& utk);

class RoomInfoToken : public InfoToken
{
public:
	/* Member method */
	RoomInfoToken();
	RoomInfoToken(const std::string& _title);
	RoomInfoToken(const RoomInfoToken&);
	RoomInfoToken(RoomInfoToken&&);
	~RoomInfoToken();
	
	RoomInfoToken& operator=(RoomInfoToken&&);
	RoomInfoToken& operator=(const RoomInfoToken& rtk);
	
	// accessor
	RoomKey get_key() const;
	std::string get_title() const;

	void operator<<(std::stringstream& sstream);
public:
	/* Member field */
	int _numOfPeer;
private:
	/* Member method */
	UserKey _getUniqueKey();
private:
	/* Member field */
	static RoomKey _UniqueKey;
	std::string _title;
};
__declspec(selectany) RoomKey RoomInfoToken::_UniqueKey = 0;
std::ostream& operator<< (std::ostream&, const RoomInfoToken&);

std::stringstream& operator<<(std::stringstream& sstream, RoomInfoToken& rtk);

class MachObject
{
public:
	virtual ~MachObject();

protected:
	/* Member method */
	MachObject();
	virtual void _dcastEnableFunc();
};



#endif