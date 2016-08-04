#pragma once

#ifndef _PT_SC_DATA_H_
#define _PT_SC_DATA_H_

#include "packet.h"

//rev
struct PK_SC_LOGIN_ACCEPT : public Packet_Base
{
	PK_SC_LOGIN_ACCEPT(const PacketManager& pm) : Packet_Base(pm) { }
	void serialize();
	void deserialize();

protected:
	PK_SC_LOGIN_ACCEPT();
	void process();
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
protected:
	/* Member method */
	virtual void doSerialProc();
	virtual void doDeserialProc();
protected:
	/* Member field */

};

#endif // !_PT_SC_DATA_H_