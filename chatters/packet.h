#pragma once

#ifndef _PACKET_H_
#define _PACKET_H_

#include "header_common.h"
#include "header_server.h"
//#include "../chClient/header_clnt.h"	//rev
#include "PT_SC_Data.h"
#include "PT_CS_Data.h"

#include "PTYPE.h"


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
	// Mutator
	void set_code(ProcCode);
	void set_msg(std::string&&);

public:		// field

private:	// method
private:	// field
	ProcCode _code;
	std::string _msg;
} PkInfo;



/*****************************************************************
 * Packet_Base class
	-
 *
 *****************************************************************/
class PacketManager;
struct Packet_Base
{
public:	
	/* Member method */
	virtual ~Packet_Base();

	static int ptoi(PTYPE);
	
	void serialize();	// Template method for serialize process
	void deserialize();	// Template method for serialize process
	virtual void processPacket(MachObject& const targetMObject) = 0;	// Process packet using strategy pattern

	// Accessor
	size_t get_packetSize();	// The size of whole packet include header space.
	const char * get_bufAddr() const;
	const PkInfo& get_pkInfo() const;
public:
	/* Member field */
protected:
	/* Member method */
	Packet_Base(PTYPE);

	virtual void doSerialProc() = 0;
	virtual void doDeserialProc() = 0;
protected:
	/* Member field */
	const PTYPE _id;
	std::stringstream _buf;
	PacketInfo _pkInfo;
private:
	/* Member method */
	Packet_Base();
	void _setHeaderSpace();	// Make (header) space for packet(_buf) size in _buf.
	void _writeHeader();	// At the end of serialize process, write _buf size on header space calling at the end of serialization.
	void _skipHeaderg();	// deserialize ��������, header space�� �ǳʶٴ� �Լ�.
	size_t _bufSize();		// The size of serialized information excluding header space
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