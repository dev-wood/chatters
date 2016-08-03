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
public:	
	/* Member method */
	PacketInfo();

	// Accessor
	ProcCode get_code() const;
	std::string get_msg() const;
	// Mutator
	void set_code(ProcCode);
	void set_msg(std::string&&);

public:
	/* Member field */		

private:
	/* Member method */
private:
	/* Member field */
	ProcCode _code;
	std::string _msg;
} PkInfo;



/*****************************************************************
 * Packet_Base class
	-
 *
 *****************************************************************/
class PacketManager_Base;
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
	void _skipHeaderg();	// deserialize 과정에서, header space를 건너뛰는 함수.
	size_t _bufSize();		// The size of serialized information excluding header space
};



/********************************************************************
 * PacketManager_Base class 
	- 
 * 
********************************************************************/
class PacketManager_Base
{
public:
	/* Member method */
	~PacketManager_Base();

	void addAgent(std::shared_ptr<MachObject>);		// add a new agent(server or client) to agent list
	void removeAgent(std::shared_ptr<MachObject>);	// remove agent(server or client) from agent list
	std::shared_ptr<MachObject> getAgent(int nth = 0);	// get agent(server or client) from agent list
	
	void sendPacket(std::shared_ptr<Packet_Base>);	// add packet to outgoing queue.
	std::shared_ptr<Packet_Base> recvPacket();		// get packet from incoming queue.
public:
	/* Member field */
protected:
	/* Member method */
	PacketManager_Base();

	virtual void _sending() = 0;	// transmit packet in outgoing queue via network.
	virtual void _receiving() = 0;	// receive packet to incoming queue via network.
protected:
	/* Member field */
	std::list<std::shared_ptr<MachObject>> _agentList;	// list of agent related to PM
	std::queue<std::shared_ptr<Packet_Base>> _outgoingQueue;	// queue storing packet to transmit
	std::queue<std::shared_ptr<Packet_Base>> _incomingQueue;	// queue storing packet received
};



#endif // !_PACKET_H_



//rev 모든 상속에 virtual dtor 정의
//? 서버측의 packet 처리 절차에서, 만약 서버의 MachObject가 여러개라면 
//	 packet이 MachObject의 정보를 수정해야할 때 MachObject 에 어떻게 접근?