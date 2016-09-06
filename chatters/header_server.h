#pragma once

#include <memory>
#include <WinSock2.h>
#include <process.h>

//#include "packet.h"
//#include "header_common.h"
#include "PT_CS_Data.h"
#include "PT_SC_Data.h"

using std::cin;
using std::cout;
using std::endl;



/*********************************************************************
* HandleData class
- listen socket�� ���� ���� ����Ǵ� Ŭ���̾�Ʈ ������ �����ϴ�
Ŭ����
*********************************************************************/
class UserInfoToken;



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
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;



/*********************************************************************
 * PerIoData class
	- TCP IOCP ���ۿ��� I/O ���ۿ� ���õ� Ŭ����
 *********************************************************************/
// do not allocate object of PerIoData type using malloc(). Use only new op.
typedef struct PerIoData
{
public:
	enum : int {
		READ_HEADER = 0,
		READ_PACKET,
		WRITE
	};
public:
	PerIoData();
	PerIoData(size_t bufSz);
	~PerIoData();

	int get_refCount() const;
	void inc_refCount();
	void allocBuffer(size_t bufSz);
	void set_Buffer(char * bufPtr, int bufSz);
	char * get_buffer() const;
	size_t get_bufferLen() const;

	void operator delete(void * p);
public:
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	int rwMode;		// read mode / write mode distinguisher
private:
	void set_refCount(int newVal);
	void dec_refCount();
	void _releaseBuffer();
private:
	char * _buffer;
	size_t _bufferLen;
	int _refCount;
} PER_IO_DATA, *LPPER_IO_DATA;



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
class SvMach : public MachObject
{
public:
	/* Member method */
	bool addUser(const std::string& id, std::shared_ptr<HandleData> hData);	// add new user to user info list
	bool removeUser(UserKey uKey);				// remove user from user info list
	bool joinRoom(RoomKey rKey, UserKey uKey);	// user join in the chatting room
	bool leaveRoom(RoomKey rKey, UserKey uKey);	// user leaves the chatting room
	bool openRoom(UserKey uKey, const std::string& title);		// add new chatting room in chatting room list
	//std::vector<UserKey> closeRoom(RoomKey rKey);		// remove the chatting room from the chatting room list
	bool removeRoom(RoomKey rKey);	// remove the chatting room from the chatting room list
	bool updateUserInfo(UserKey uKey, RoomKey newRmKey);	// update user infomation
	
	std::unordered_map<UserKey, SvUserInfo>::const_iterator findUser(UserKey uKey) const;
	std::unordered_map<RoomKey, SvRoomInfo>::const_iterator findRoom(RoomKey rKey) const;

	std::unordered_map<UserKey, SvUserInfo>::iterator findUser(UserKey uKey);
	std::unordered_map<RoomKey, SvRoomInfo>::iterator findRoom(RoomKey rKey);

	// accessor
	const std::unordered_map<UserKey, SvUserInfo>& get_userList() const;
	const std::unordered_map<RoomKey, SvRoomInfo>& get_roomList() const;
public:
	/* Member field */

private:
	/* Member method */
private:
	/* Member field */
	std::unordered_map<UserKey, SvUserInfo> _uList;
	std::unordered_map<RoomKey, SvRoomInfo> _rList;
};



/************************************************************************
* SvPacketManager class
- Singleton pattern.
*
************************************************************************/
class SvPacketManager : public PacketManager_Base
{
public:
	/* Member method */
	static SvPacketManager& Instance();

	void sendPacket(std::shared_ptr<Packet_Base> spPk);// transmit packet via network.
	std::shared_ptr<Packet_Base> recvPacket();	// get packet from incoming packet queue.
public:
	/* Member field */
protected:
	/* Member method */
protected:
	/* Member field */
	static SvPacketManager _instance;
	std::queue<std::shared_ptr<Packet_Base>> _msgQueue;	// incoming packet queue

	friend DWORD WINAPI recvThreadMain(LPVOID);
};



/*********************************************************************
 * etc. functions
	- ��Ÿ �Լ� ����

*********************************************************************/
DWORD WINAPI recvThreadMain(LPVOID pComPort);
DWORD WINAPI packetProcessWorkerThreadMain(LPVOID pComPort);
void ErrorHandling(char * mesaage);