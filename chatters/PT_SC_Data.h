#pragma once

#ifndef _PT_SC_DATA_H_
#define _PT_SC_DATA_H_

#include "packet.h"



// pre-declaration
class SvUserInfo;
class SvRoomInfo;
class SvMach;
class SvPacketManager;



/*********************************************************************
 * PK_SC_LOGIN_ACCEPT class
	- Packet 구조
	  (packet size)|(packet id)|(user key)|(user id)|
*********************************************************************/
struct PK_SC_LOGIN_ACCEPT : public Packet_Base
{
public:
	/* Member method */
	PK_SC_LOGIN_ACCEPT();
	PK_SC_LOGIN_ACCEPT(PTYPE pType, char* buf, size_t bufLen);

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserInfoToken userTk;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
* PK_SC_LOGIN_FAIL class
- Packet 구조
	  (packet size)|(packet id)|
*********************************************************************/
struct PK_SC_LOGIN_FAIL : public Packet_Base
{
public:
	/* Member method */
	PK_SC_LOGIN_FAIL();
	PK_SC_LOGIN_FAIL(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_SC_LOBBY_JOINROOM_ACCEPT class
	- Packet 구조 : 유저의 수만큼 <<, >> 안의 내용을 반복
	  (packet size)|(packet id)|(room key)|(room title)|(user 수)|<<(user key)|(user id)|>>
*********************************************************************/
struct PK_SC_LOBBY_JOINROOM_ACCEPT : public Packet_Base
{
public:
	/* Member method */
	PK_SC_LOBBY_JOINROOM_ACCEPT();
	PK_SC_LOBBY_JOINROOM_ACCEPT(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	RoomInfoToken roomTk;
	std::vector<UserInfoToken> userList;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */

};

/*********************************************************************
 * PK_SC_LOBBY_JOINROOM_FAIL class
	- Packet 구조
	  (packet size)|(packet id)|
*********************************************************************/
struct PK_SC_LOBBY_JOINROOM_FAIL : public Packet_Base
{
public:
	/* Member method */
	PK_SC_LOBBY_JOINROOM_FAIL();
	PK_SC_LOBBY_JOINROOM_FAIL(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */

};

/*********************************************************************
 * PK_SC_LOBBY_LOAD_ROOMLIST class
	- Packet 구조 : 유저의 수만큼 <<, >> 안의 내용을 반복
	  (packet size)|(packet id)|(room 수)|<<(room key)|(room title)|>>
*********************************************************************/
struct PK_SC_LOBBY_LOAD_ROOMLIST : public Packet_Base
{
public:
	/* Member method */
	PK_SC_LOBBY_LOAD_ROOMLIST();
	PK_SC_LOBBY_LOAD_ROOMLIST(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	const std::unordered_map<RoomKey, SvRoomInfo> * pRmList;
	std::vector<RoomInfoToken> rtkList;

protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */

};

/*********************************************************************
 * PK_SC_CREATEROOM_OK class
	- Packet 구조
	  (packet size)|(packet id)|(room key)|(room title)|(number of peer in the room)|
*********************************************************************/
struct PK_SC_CREATEROOM_OK : public Packet_Base
{
public:
	/* Member method */
	PK_SC_CREATEROOM_OK();
	PK_SC_CREATEROOM_OK(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	RoomInfoToken roomTk;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_SC_CREATEROOM_FAIL class
	- Packet 구조
	  (packet size)|(packet id)|
*********************************************************************/
struct PK_SC_CREATEROOM_FAIL : public Packet_Base
{
public:
	/* Member method */
	PK_SC_CREATEROOM_FAIL();
	PK_SC_CREATEROOM_FAIL(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_SC_CHAT_QUITUSER class
	- Packet 구조
	  (packet size)|(packet id)|(user key)|
*********************************************************************/
struct PK_SC_CHAT_QUITUSER : public Packet_Base
{
public:
	/* Member method */
	PK_SC_CHAT_QUITUSER();
	PK_SC_CHAT_QUITUSER(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_SC_CHAT_CHAT class
	- Packet 구조
	  (packet size)|(packet id)|(user key)|(chatting)|
*********************************************************************/
struct PK_SC_CHAT_CHAT : public Packet_Base
{
public:
	/* Member method */
	PK_SC_CHAT_CHAT();
	PK_SC_CHAT_CHAT(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey uKey;
	std::string chat;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_SC_CHAT_LOAD_USERLIST class
	- Packet 구조 : 유저의 수만큼 <<, >> 안의 내용을 반복
	  (packet size)|(packet id)|(user의 수)|<<(user key)|(user id)|>>
*********************************************************************/
struct PK_SC_CHAT_LOAD_USERLIST : public Packet_Base
{
public:
	/* Member method */
	PK_SC_CHAT_LOAD_USERLIST();
	PK_SC_CHAT_LOAD_USERLIST(PTYPE pType, char* buf, size_t bufLen);
	
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	std::vector<UserInfoToken > userList;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};



/*********************************************************************
 * etc. functions
	- 
*********************************************************************/
std::shared_ptr<Packet_Base> extractSCPacket(char * buf, size_t bufLen);



#endif // !_PT_SC_DATA_H_



//rev 
// PK_SC_CREATEROOM_OK class가 RoomInfoToken 갖도록 수정(Roomkey 필요하기 때문)
// 