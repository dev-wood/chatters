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
	int get_bufSize();
	//std::stringstream& get_buf() const;	//rev
	//const char * get_bufAddr() const;
	
protected:
	Packet_Base();
	void _setHeaderSpace();
	void _skipHeaderg();

	std::stringstream _buf;
	PacketManager * const _pm;
};

/*******************************************************
 * PacketManager class 
 * 


*******************************************************/
class PacketManager
{
public:
	PacketManager();
	PacketManager(MachObject * mach);
	void _setPacket(Packet_Base * pk);
	void _serialize();
	void _deserialize();

	// accessor
	Packet_Base& get_pk();
	MachObject& get_mach();

	// mutator
	void set_pk(Packet_Base * pk);
	void set_mach(MachObject * mach);
public:
protected:
protected:
	MachObject * _mach;
	Packet_Base * _pk;

};


#endif // !_PACKET_H_