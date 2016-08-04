#pragma once

#ifndef _PT_SC_DATA_H_
#define _PT_SC_DATA_H_

#include "packet.h"



struct PK_SC_LOGIN_ACCEPT : public Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserInfoToken userTk;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};

struct PK_SC_LOGIN_FAIL : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_SC_LOBBY_JOINROOMOK : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */

};
struct PK_SC_LOBBY_JOINROOMFAIL : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */

};
struct PK_SC_LOBBY_LOAD_ROOMLIST : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;	//rev	UserKey 필요 없을 듯..?
	std::vector<RoomInfoToken> roomTkList;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */

};
struct PK_SC_CREATEROOM_OK : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;		//rev UserKey는 필요 없을 듯..?
	RoomInfoToken roomTk;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_SC_CREATEROOM_FAIL : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;	//rev UserKey는 필요 없을 듯..?
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_SC_CHAT_QUITUSER : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;	//rev UserKey는 필요 없을 듯..?
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_SC_CHAT_CHAT : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	std::string chat;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_SC_CHAT_LOAD_USERLIST : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	std::vector<UserInfoToken > userTkList;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};



#endif // !_PT_SC_DATA_H_



//rev 
// PK_SC_CREATEROOM_OK class가 RoomInfoToken 갖도록 수정(Roomkey 필요하기 때문)
// 