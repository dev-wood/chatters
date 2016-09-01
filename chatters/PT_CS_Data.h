#pragma once

#ifndef _PT_CS_DATA_H_
#define _PT_CS_DATA_H_

#include "packet.h"



//rev
struct PK_CS_LOGIN_REQUEST : public Packet_Base
{
public:
	/* Member method */
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
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
struct PK_CS_LOBBY_JOINROOM : public Packet_Base
{
public:
	/* Member method */
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
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
struct PK_CS_LOBBY_LOAD_ROOMLIST : public Packet_Base
{
public:
	/* Member method */
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;	//rev 필요없을 듯..?
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */
};
struct PK_CS_CREATEROOM_CREATEROOM : public Packet_Base
{
public:
	/* Member method */
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
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
struct PK_CS_CHAT_QUITROOM : public Packet_Base
{
public:
	/* Member method */
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
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
struct PK_CS_CHAT_CHAT : public Packet_Base
{
public:
	/* Member method */
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& const targetMObject);	// Received packet process procedure using strategy pattern
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