#pragma once

#include "header_common.h"
#include <memory>
#include <WinSock2.h>

/*********************************************************************
 * HandleData class
	- listen socket�� ���� ���� ����Ǵ� Ŭ���̾�Ʈ ������ �����ϴ�
	 Ŭ����
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
	- Server�� ������� ������ �����ϴ� Ŭ����
 *********************************************************************/
class SvConInfo
{
public:
	/* Member method */
	SvConInfo();	//rev �׿� constructor ����
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
	- server�� ���� ���� client�� user information�� �����ϴ�  Ŭ����
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
	UserInfoToken utk;	// user ������ ��� �ִ� token
	int curRmNum;		// ���� ���� ���� ä�ù�. ���� �� -1
	std::shared_ptr<HandleData> spHData;
private:
	/* Member method */
	SvUserInfo();
private:
	/* Member field */
};

/*********************************************************************
 * SvRoomInfo class
	- ���� �����Ǿ� �ִ� ä�ù� ���� ������ �����ϴ� Ŭ����

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

/*********************************************************************
 * SvRoomInfo class
	- ���� �����Ǿ� �ִ� ä�ù� ���� ������ �����ϴ� Ŭ����

 *********************************************************************/
class SvMach
{
public:
	/* Member method */
	bool addUser(UserInfoToken&& utk);
	bool removeUser(UserKey uKey);
	bool enterRoom(RoomKey rKey, UserKey uKey);
	bool exitRoom(RoomKey rKey, UserKey uKey);
	bool updateUserInfo(/**/);
	bool updateRoomInfo(/**/);
public:
	/* Member field */

private:
	/* Member method */
private:
	/* Member field */
	std::unordered_map<UserKey, SvUserInfo> _uList;
	std::unordered_map<RoomKey, SvRoomInfo> _rList;

};
