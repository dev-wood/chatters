#pragma once

#ifndef _PACKET_H_
#define _PACKET_H_

#include "header_common.h"
#include "header_server.h"
//#include "../chClient/header_clnt.h"	//rev
#include "PT_SC_Data.h"
#include "PT_CS_Data.h"

/*****************************************************************
 * PacketInfo class
	- packet processing(de/serialize) 의 결과 및 관련 정보를 저장
 *
 *****************************************************************/
typedef class PacketInfo
{
public:
	enum ProcCode :int {
		EMPTY,		// Packet was initialized but no follow-up processing.
		SUCCESS,	// Packet processing successfully.
		FAIL,		// Packet processing failed.
		ABORT		// Packet processing aborted by user.
	};
public:		// method
	PacketInfo();

	// Accessor
	ProcCode get_code() const;
	std::string get_msg() const;

public:		// field

	friend class Packet_Base;
private:	// method
	// Mutator
	void set_code(ProcCode);
	void set_msg(std::string&&);
private:	// field
	ProcCode _code;
	std::string _msg;
} PkInfo;

//rev
/*****************************************************************
 * Packet_Base class
	-
 *
 *****************************************************************/
struct Packet_Base
{
public:
	static int ptoi(PTYPE::SC pt);
	static int ptoi(PTYPE::CS pt);

	Packet_Base();
	Packet_Base(PacketManager * const pm);
	virtual void serialize() = 0;
	virtual void deserialize() = 0;
	virtual void process() = 0;
	//void setPacketManager(const PacketManager * pm);	//rev
	int get_bufSize();
	//std::stringstream& get_buf() const;	//rev
	//const char * get_bufAddr() const;
public:

protected:
	Packet_Base();
	void _setHeaderSpace();
	void _skipHeaderg();

protected:
	std::stringstream _buf;
	PacketManager * const _pm;
	PacketInfo _pkInfo;

};

/********************************************************************
 * PacketManager class 
 * 


********************************************************************/
class PacketManager
{
public:
	PacketManager();
	PacketManager(MachObject * mach);
	void _setPacket(Packet_Base & pk);
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