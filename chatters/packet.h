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

	virtual void serialize() = 0;
	virtual void deserialize() = 0;
	virtual void process() = 0;
	
	// Accessor
	//rev bufSize는 proteced, packetSize는 public으로..?
	int _bufSize();		// The size of serialized information excluding header space
	int _packetSize();	// The size of whole packet include header space.
	const std::stringstream& get_buf() const;	//rev
	std::stringstream& get_buf();	//rev
	//const char * get_bufAddr() const;
	// Mutator
	void set_pm(PacketManager& pm);
public:

protected:
	Packet_Base();

	void _setHeaderSpace();	// _buf에 packet size 저장을 위한 (header) space 확보하는 함수.
	void _skipHeaderg();	// Packet을 deserialize 시 header space를 건너뛰는 함수.

protected:
	//rev packet id 넣기, ctor에 적용.
	std::stringstream _buf;
	PacketManager * _pm;
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