#pragma once

#ifndef _PT_CS_DATA_H_
#define _PT_CS_DATA_H_

#include "packet.h"



//rev
struct PK_CS_LOGIN_REQUEST : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	std::string userId;
	std::string userPassword;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */

};
struct PK_CS_LOBBY_JOINROOM : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
	RoomKey roomKey;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_CS_LOBBY_LOAD_ROOMLIST : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_CS_CREATEROOM_CREATEROOM : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
	std::string roomTitle;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_CS_CHAT_QUITROOM : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
	RoomKey roomKey;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_CS_CHAT_CHAT : Packet_Base
{
public:
	/* Member method */
	virtual void processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	RoomKey roomKey;
	std::string chat;
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};

#endif	// !_PT_CS_DATA_H_