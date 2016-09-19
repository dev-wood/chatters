#pragma once

#ifndef _PACKET_H_
#define _PACKET_H_

#define NOMINMAX

#include "PTYPE.h"
#include "header_common.h"
//#include "header_server.h"
//#include "header_clnt.h"
//#include "PT_SC_Data.h"
//#include "PT_CS_Data.h"



/*****************************************************************
 * ProcessInfo class
	- packet processing(de/serialize) 의 결과 및 관련 정보를 저장
 *
 *****************************************************************/
typedef class ProcessInfo
{
public:
	enum ProcCode :int {
		EMPTY,		// Packet was initialized but no follow-up processing.
		SUCCESS,	// Packet processing successfully.
		FAIL,		// Packet processing failed.
		ABORT,		// Packet processing aborted by user.
		SOCKET_CLOSED	// Socket closed. Disconnected.		//rev
	};
public:	
	/* Member method */
	ProcessInfo();

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
} ProcInfo;



/*****************************************************************
 * Packet_Base class
	-
 *
 *****************************************************************/
class PacketManager_Base;
struct Packet_Base
{
public:
	static const size_t HEADER_SIZE = sizeof(size_t);
public:	
	/* Member method */
	Packet_Base(PTYPE pType, const char * buf, size_t bufLen);
	virtual ~Packet_Base();

	static int ptoi(PTYPE);
	
	void serialize();	// Template method for serialize process
	void deserialize();	// Template method for serialize process
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject) = 0;	// Process received packet using strategy pattern
	void setProcessInfo(ProcInfo::ProcCode resCode, std::string&& msg = "");

	// Accessor
	size_t get_packetSize();	// The size of whole packet include header space.
	size_t get_bufSize();		// The size of buffer(serialized information).
	const char * get_bufAddr() const;
	const std::stringstream& get_buf() const;
	const ProcInfo& get_ProcInfo() const;

	Packet_Base& operator<<(const char * buf);
public:
	/* Member field */
	const PTYPE id;
	std::vector<SOCKET> sockList;
protected:
	/* Member method */
	Packet_Base(PTYPE);

	virtual void _doSerialProc() = 0;
	virtual void _doDeserialProc() = 0;
protected:
	/* Member field */
	std::stringstream _buf;
	ProcessInfo _ProcInfo;
private:
	/* Member method */
	Packet_Base();
	void _setHeaderSpace();	// Make (header) space for packet(_buf) size in _buf.
	void _writeHeader();	// At the end of serialize process, write _buf size on header space calling at the end of serialization.
	size_t _bufSize();		// The size of serialized information excluding header space
};



/********************************************************************
 * PK_EMPTY class
	- Packet 생성 과정에서 어떤 이유로든 error가 발생한 경우 이에 대한
	정보가 저장된 packet class
 *
********************************************************************/
class PK_EMPTY : public Packet_Base
{
public:
	/* Member method */;
	PK_EMPTY();

	virtual std::shared_ptr<Packet_Base> processPacket(MachObject & targetMObject) override;
public:
	/* Member field */

protected:
	/* Member method */
	virtual void _doSerialProc() override;
	virtual void _doDeserialProc() override;
protected:
	/* Member field */
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
	virtual ~PacketManager_Base();

	void setAgent(MachObject * pMObj);
	MachObject& getAgent();
	
	virtual void sendPacket(std::shared_ptr<Packet_Base>) = 0;			// add packet to outgoing queue.
	virtual std::shared_ptr<Packet_Base> recvPacket() = 0;	// get packet from incoming queue.
public:
	/* Member field */
protected:
	/* Member method */
	PacketManager_Base();

protected:
	/* Member field */
	MachObject * _pMachObject;
};



/*********************************************************************
* Etc. implementation

*********************************************************************/



#endif // !_PACKET_H_





//rev packet incoming, outgoing에 목적지 socket 정보도 같이 저장..?
//		이 경우 servaddr 등 addr 관련 정보도 같이 저장되나? 
//		그렇다면 packet 하나 당 저장해야하는 정보량이 너무 늘어나니 
//		그냥 user 정보 참조하게 하는게 나을 수도

//rev outgoing Queue는 필요 없을 것 같은데.. 그럼 chat packet을 전송할 때 
//		같은 방 참가자의 socket list를 저장해 놓는 것은 어떻게 구현해야
//		할까..? 
//			- client: outgoing queue 필요 없음. 연결 정보는 불변하므로 신경 쓸 필요X
//			- Server: outgoing queue 필요 없음. WSASend 여러번 호출 하는 걸로..

//rev packet에 const char* type을 argument로 받는 생성자 추가

//?	Packet 생성 절차 어떻게 되나? char chunk에서 생성하는 방법이..?
//? sending 시 sendPacket() 또는 _sending() 함수로 어떻게 연결정보를 보내줄까?