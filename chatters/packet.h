#pragma once

#ifndef _PACKET_H_
#define _PACKET_H_

#include "header_common.h"
#include "header_server.h"
//#include "../chClient/header_clnt.h"	//rev
#include "PT_SC_Data.h"
#include "PT_CS_Data.h"

#include "PTYPE.h"



typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

// do not allocate object of PerIoData type using malloc(). Use only new op.
typedef struct PerIoData
{
public:
	enum : int {
		READ_HEADER = 0,
		READ_PACKET,
		WRITE
	};
public:
	PerIoData();
	PerIoData(size_t bufSz);
	~PerIoData();

	int get_refCount() const;
	void inc_refCount();
	void allocBuffer(size_t bufSz);
	void set_Buffer(char * bufPtr, int bufSz);
	char * get_buffer() const;
	size_t get_bufferLen() const;

	void operator delete(void * p);
public:
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	int rwMode;		// read mode / write mode distinguisher
private:
	void set_refCount(int newVal);
	void dec_refCount();
	void _releaseBuffer();
private:
	char * _buffer;
	size_t _bufferLen;
	int _refCount;
} PER_IO_DATA, *LPPER_IO_DATA;



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
		SOCKET_CLOSED	// Socket closed. Disconnected.
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
	Packet_Base(PTYPE pType, char * buf);
	virtual ~Packet_Base();

	static int ptoi(PTYPE);
	
	void serialize();	// Template method for serialize process
	void deserialize();	// Template method for serialize process
	virtual std::shared_ptr<Packet_Base> processPacket(MachObject& const targetMObject) = 0;	// Process received packet using strategy pattern

	// Accessor
	size_t get_packetSize();	// The size of whole packet include header space.
	size_t get_bufSize();		// The size of buffer(serialized information).
	const char * get_bufAddr() const;
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
	
	virtual void sendPacket(SOCKET, std::shared_ptr<Packet_Base>) = 0;			// add packet to outgoing queue.
	virtual std::shared_ptr<Packet_Base> recvPacket(SOCKET& sock) = 0;	// get packet from incoming queue.
public:
	/* Member field */
protected:
	/* Member method */
	PacketManager_Base();

protected:
	/* Member field */
	MachObject * _pMachObject;
};



/************************************************************************
 * ClntPacketManager class
	- Singleton pattern.
 *
 ************************************************************************/
class ClntPacketManager : public PacketManager_Base
{
public:
	/* Member method */
	static ClntPacketManager& Instance();
	
	void sendPacket(SOCKET sock, std::shared_ptr<Packet_Base> spPk);// send packet via network.
	std::shared_ptr<Packet_Base> recvPacket(SOCKET& sock);	// receive packet in outgoing queue via network.
public:
	/* Member field */
protected:
	/* Member method */
protected:
	/* Member field */
	static ClntPacketManager _instance;
};



/************************************************************************
 * SvPacketManager class
	- Singleton pattern.
 *
 ************************************************************************/
class SvPacketManager : public PacketManager_Base
{
public:
	/* Member method */
	static SvPacketManager& Instance();

	void sendPacket(SOCKET sock, std::shared_ptr<Packet_Base> spPk);// transmit packet via network.
	std::shared_ptr<Packet_Base> recvPacket(SOCKET& sock);	// get packet from incoming packet queue.
public:
	/* Member field */
protected:
	/* Member method */
protected:
	/* Member field */
	static SvPacketManager _instance;

	std::queue<std::pair<SOCKET, std::shared_ptr<Packet_Base>>> _msgQueue;	// incoming packet queue
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