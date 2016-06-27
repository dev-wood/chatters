#pragma once

#ifndef _PACKET_H_
#define _PACKET_H_

#include "header_common.h"
#include "header_server.h"
//#include "../chClient/header_clnt.h"	//rev
#include "PT_SC_Data.h"
#include "PT_CS_Data.h"

//rev
/****************************************************
 * Packet_Base class
 *


****************************************************/
struct Packet_Base
{
public:
	static int ptoi(PTYPE::SC pt);
	static int ptoi(PTYPE::CS pt);

	Packet_Base(PacketManager * const pm);
	virtual void serialize() = 0;
	virtual void deserialize() = 0;
	virtual void process() = 0;
	//void setPacketManager(const PacketManager * pm);	//rev
	int get_bufSize() const;
	//std::stringstream& get_buf() const;	//rev
	//const char * get_bufAddr() const;
	
protected:
	Packet_Base();

	std::stringstream _buf;
	PacketManager * _pm;
};

/*******************************************************
 * PacketManager class 
 * 


*******************************************************/
class PacketManager
{
public:
	PacketManager(SvMach * mach);
	//PacketManager(ClMach * mach);	//rev
	//void _setPacket(Packet_Base&);	//rev 필요 없을 듯
	void _serialize(Packet_Base&);
	void _deserialize(Packet_Base&);
public:
	SvMach * _svMach;
	//ClMach * _clMach;	//rev
protected:
	PacketManager();

};


#endif // !_PACKET_H_