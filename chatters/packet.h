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
		ABORT,		// Packet processing aborted by user.
		SOCKET_CLOSED	// Socket closed. Disconnected.		//rev
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
	static const size_t HEADER_SIZE = sizeof(size_t);
public:	
	/* Member method */
	Packet_Base(PTYPE pType, const char * buf, size_t bufLen);
	virtual ~Packet_Base();

	static int ptoi(PTYPE);
	
	void serialize();	// Template method for serialize process
	void deserialize();	// Template method for serialize process
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& targetMObject) = 0;	// Process received packet using strategy pattern
	void setProcessInfo(PkInfo::ProcCode resCode, std::string&& msg = "");

	// Accessor
	size_t get_packetSize();	// The size of whole packet include header space.
	size_t get_bufSize();		// The size of buffer(serialized information).
	const char * get_bufAddr() const;
	const std::stringstream& get_buf() const;
	const PkInfo& get_pkInfo() const;

	Packet_Base& operator<<(const char * buf);
public:
	/* Member field */
	const PTYPE _id;
	SOCKET sock;
protected:
	/* Member method */
	Packet_Base(PTYPE);

	virtual void doSerialProc() = 0;
	virtual void doDeserialProc() = 0;
protected:
	/* Member field */
	std::stringstream _buf;
	PacketInfo _pkInfo;
private:
	/* Member method */
	Packet_Base();
	void _setHeaderSpace();	// Make (header) space for packet(_buf) size in _buf.
	void _writeHeader();	// At the end of serialize process, write _buf size on header space calling at the end of serialization.
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





//rev packet incoming, outgoing�� ������ socket ������ ���� ����..?
//		�� ��� servaddr �� addr ���� ������ ���� ����ǳ�? 
//		�׷��ٸ� packet �ϳ� �� �����ؾ��ϴ� �������� �ʹ� �þ�� 
//		�׳� user ���� �����ϰ� �ϴ°� ���� ����

//rev outgoing Queue�� �ʿ� ���� �� ������.. �׷� chat packet�� ������ �� 
//		���� �� �������� socket list�� ������ ���� ���� ��� �����ؾ�
//		�ұ�..? 
//			- client: outgoing queue �ʿ� ����. ���� ������ �Һ��ϹǷ� �Ű� �� �ʿ�X
//			- Server: outgoing queue �ʿ� ����. WSASend ������ ȣ�� �ϴ� �ɷ�..

//rev packet�� const char* type�� argument�� �޴� ������ �߰�

//?	Packet ���� ���� ��� �ǳ�? char chunk���� �����ϴ� �����..?
//? sending �� sendPacket() �Ǵ� _sending() �Լ��� ��� ���������� �����ٱ�?