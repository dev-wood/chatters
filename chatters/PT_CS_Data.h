#pragma once

#ifndef _PT_CS_DATA_H_
#define _PT_CS_DATA_H_

#include "packet.h"



// Pre-declation
class SvMach;



/*********************************************************************
 * PK_CS_LOGIN_REQUEST class
	-
*********************************************************************/
struct PK_CS_LOGIN_REQUEST : public Packet_Base
{
public:
	/* Member method */
	PK_CS_LOGIN_REQUEST();
	PK_CS_LOGIN_REQUEST(PTYPE ptype, const char* buf, size_t bufLen);

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	std::string userId;
	std::string userPassword;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */

};

/*********************************************************************
 * PK_CS_LOBBY_JOINROOM class
	-
*********************************************************************/
struct PK_CS_LOBBY_JOINROOM : public Packet_Base
{
public:
	/* Member method */
	PK_CS_LOBBY_JOINROOM();
	PK_CS_LOBBY_JOINROOM(PTYPE ptype, const char* buf, size_t bufLen);

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
	RoomKey roomKey;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_CS_LOBBY_LOAD_ROOMLIST class
	-
*********************************************************************/
struct PK_CS_LOBBY_LOAD_ROOMLIST : public Packet_Base
{
public:
	/* Member method */
	PK_CS_LOBBY_LOAD_ROOMLIST();
	PK_CS_LOBBY_LOAD_ROOMLIST(PTYPE ptype, const char* buf, size_t bufLen);

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
 * PK_CS_CREATEROOM_CREATEROOM class
	-
*********************************************************************/
struct PK_CS_CREATEROOM_CREATEROOM : public Packet_Base
{
public:
	/* Member method */
	PK_CS_CREATEROOM_CREATEROOM();
	PK_CS_CREATEROOM_CREATEROOM(PTYPE ptype, const char* buf, size_t bufLen);

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
	std::string roomTitle;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_CS_CHAT_QUITROOM class
	-
*********************************************************************/
struct PK_CS_CHAT_QUITROOM : public Packet_Base
{
public:
	/* Member method */
	PK_CS_CHAT_QUITROOM();
	PK_CS_CHAT_QUITROOM(PTYPE ptype, const char* buf, size_t bufLen);

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	UserKey userKey;
	RoomKey roomKey;
protected:
	/* Member method */
	virtual void _doSerialProc();
	virtual void _doDeserialProc();
protected:
	/* Member field */
};

/*********************************************************************
 * PK_CS_CHAT_CHAT class
	-
*********************************************************************/
struct PK_CS_CHAT_CHAT : public Packet_Base
{
public:
	/* Member method */
	PK_CS_CHAT_CHAT();
	PK_CS_CHAT_CHAT(PTYPE ptype, const char* buf, size_t bufLen);

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
public:
	/* Member field */
	RoomKey roomKey;
	std::string chat;
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
std::shared_ptr<Packet_Base> extractCSPacket(char * buf, size_t bufLen);



#endif	// !_PT_CS_DATA_H_