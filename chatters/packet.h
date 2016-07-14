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
	- packet processing(de/serialize) �� ��� �� ���� ������ ����
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

	void _setHeaderSpace();	// _buf�� packet(_buf) size ������ ���� (header) space Ȯ���ϴ� �Լ�.
	void _writeHeader();	// deserialize ����. Write _buf size on header space calling at the end of serialization.
	void _skipHeaderg();	// deserialize ��������, header space�� �ǳʶٴ� �Լ�.
	size_t _bufSize();		// The size of serialized information excluding header space
	
protected:
	//rev packet id �ֱ�, ctor�� ����.
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
	void set_pk(Packet_Base * pk);		//rev ��� �������̽��� reference param.�� �����ϴ°�..?
	void set_mach(MachObject * mach);	//rev ���� ��
public:
protected:
protected:
	MachObject * _mach;
	Packet_Base * _pk;

};


#endif // !_PACKET_H_



//rev ��� ��ӿ� virtual dtor ����