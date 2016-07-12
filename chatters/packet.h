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



/*****************************************************************
 * Packet_Base class
	-
 *
 *****************************************************************/
struct Packet_Base
{
public:
	static int ptoi(PTYPE);

	virtual void serialize() = 0;
	virtual void deserialize() = 0;
	virtual void process() = 0;
	virtual std::shared_ptr<Packet_Base> cast() = 0;	// return empty packet object.

	// Accessor
	size_t _packetSize();	// The size of whole packet include header space.
	const char * get_bufAddr() const;

	// Mutator
	void set_pm(PacketManager& pm);
public:

protected:
	Packet_Base();
	virtual ~Packet_Base();

	void _setHeaderSpace();	// _buf에 packet(_buf) size 저장을 위한 (header) space 확보하는 함수.
	void _writeHeader();	// deserialize 과정. Write _buf size on header space calling at the end of serialization.
	void _skipHeaderg();	// deserialize 과정에서, header space를 건너뛰는 함수.
	size_t _bufSize();		// The size of serialized information excluding header space
	
protected:
	//rev packet id 넣기, ctor에 적용.
	const PTYPE _id;
	std::stringstream _buf;
	PacketManager * _pm;
	PacketInfo _pkInfo;

};



/********************************************************************
 * PacketManager class 
	- 
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
	void set_pk(Packet_Base * pk);		//rev 모든 인터페이스는 reference param.로 통일하는게..?
	void set_mach(MachObject * mach);	//rev 위와 동
public:
protected:
protected:
	MachObject * _mach;
	Packet_Base * _pk;

};


#endif // !_PACKET_H_



//rev 모든 상속에 virtual dtor 정의