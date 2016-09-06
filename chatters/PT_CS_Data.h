#pragma once

#ifndef _PT_CS_DATA_H_
#define _PT_CS_DATA_H_

#include "packet.h"



//rev member method 정의하기
struct PK_CS_LOGIN_REQUEST : public Packet_Base
{
public:
	/* Member method */
	PK_CS_LOGIN_REQUEST();
<<<<<<< HEAD
	PK_CS_LOGIN_REQUEST(PTYPE ptype, const char* buf);
=======
	PK_CS_LOGIN_REQUEST(PTYPE ptype, const char* buf, size_t bufLen);
>>>>>>> svtest

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
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
	PK_CS_LOBBY_JOINROOM();
<<<<<<< HEAD
	PK_CS_LOBBY_JOINROOM(PTYPE ptype, const char* buf);
=======
	PK_CS_LOBBY_JOINROOM(PTYPE ptype, const char* buf, size_t bufLen);
>>>>>>> svtest

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
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
	PK_CS_LOBBY_LOAD_ROOMLIST();
<<<<<<< HEAD
	PK_CS_LOBBY_LOAD_ROOMLIST(PTYPE ptype, const char* buf);
=======
	PK_CS_LOBBY_LOAD_ROOMLIST(PTYPE ptype, const char* buf, size_t bufLen);
>>>>>>> svtest

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
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
	PK_CS_CREATEROOM_CREATEROOM();
<<<<<<< HEAD
	PK_CS_CREATEROOM_CREATEROOM(PTYPE ptype, const char* buf);
=======
	PK_CS_CREATEROOM_CREATEROOM(PTYPE ptype, const char* buf, size_t bufLen);
>>>>>>> svtest

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
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
	PK_CS_CHAT_QUITROOM();
<<<<<<< HEAD
	PK_CS_CHAT_QUITROOM(PTYPE ptype, const char* buf);
=======
	PK_CS_CHAT_QUITROOM(PTYPE ptype, const char* buf, size_t bufLen);
>>>>>>> svtest

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
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
	PK_CS_CHAT_CHAT();
<<<<<<< HEAD
	PK_CS_CHAT_CHAT(PTYPE ptype, const char* buf);
=======
	PK_CS_CHAT_CHAT(PTYPE ptype, const char* buf, size_t bufLen);
>>>>>>> svtest

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject);	// Received packet process procedure using strategy pattern
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



/*********************************************************************
* etc. functions
-
*********************************************************************/
<<<<<<< HEAD
std::shared_ptr<Packet_Base> extractPacketFromBuffer(char * buf);
=======
std::shared_ptr<Packet_Base> extractPacketFromBuffer(char * buf, size_t bufLen);
>>>>>>> svtest



#endif	// !_PT_CS_DATA_H_