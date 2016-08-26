#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"



/*********************************************************************
* PacketInfo class implementation
*********************************************************************/
PacketInfo::PacketInfo() : _code(EMPTY), _msg("")
{
	// left blank intentionally
}
PkInfo::ProcCode PacketInfo::get_code() const
{
	return _code;
}
std::string PacketInfo::get_msg() const
{
	return _msg;
}
void PacketInfo::set_code(ProcCode code)
{
	_code = code;
}
void PacketInfo::set_msg(std::string && str)
{
	_msg = str;
}




/*********************************************************************
 * Packet_Base class implementation 
 *********************************************************************/
Packet_Base::Packet_Base(PTYPE pType, char * buf) : _id(pType)
{
	_buf << buf;
}
Packet_Base::~Packet_Base()
{
	// left blank intentionally
}
int Packet_Base::ptoi(PTYPE pt)
{
	return static_cast<int>(pt);
}
void Packet_Base::serialize()
{
	// Reserve header space for buf length.
	_setHeaderSpace();

	// Write packet ID on buf.
	_buf << ptoi(_id) << '|';

	// Do serialize process depend on each Packet class.
	doSerialProc();

	// Write header at the last of serialize process.
	_writeHeader();
}
void Packet_Base::deserialize()
{
	// Do deserailize process depend on each Packet class.
	doDeserialProc();
}
size_t Packet_Base::get_packetSize()
{
	return _bufSize() + sizeof(size_t);
}
size_t Packet_Base::get_bufSize()
{
	return _bufSize();
}
const char * Packet_Base::get_bufAddr() const
{
	return _buf.str().c_str();
}
const PkInfo & Packet_Base::get_pkInfo() const
{
	return _pkInfo;
}
Packet_Base & Packet_Base::operator<<(const char * buf)
{
	_buf << buf;
}
Packet_Base::Packet_Base(PTYPE pt) : _id(pt), _pkInfo()
{
	// left blank intentionally
}
Packet_Base::Packet_Base() : _id(PTYPE::PT_BASE), _pkInfo()
{
	// left blank intentionally
}
void Packet_Base::_setHeaderSpace()
{
	//rev setfill, width 사용해서 수정.
	char ch[sizeof(size_t) + 1];
	std::fill_n(ch, sizeof(size_t), '0');
	ch[sizeof(size_t)] = 0;

	_buf << ch;
}
void Packet_Base::_writeHeader()
{
	size_t bufSize = _bufSize();	// get packet size

	auto prevPos = _buf.tellp();	// store current input sequence pos
	_buf.seekp(_buf.beg);	// move input sequence pos to header space

	_buf.write((const char*)&bufSize, sizeof(bufSize));	// write packet size in header space

	_buf.seekp(prevPos);	// restore previous input sequence pos	
}
size_t Packet_Base::_bufSize()
{
	size_t bufSz = 0;
	auto cPos = _buf.tellg();	// store current input sequence pos
	_buf.seekg(_buf.end);
	bufSz = static_cast<int>(_buf.tellg());

	_buf.seekg(cPos);	// restore previous input sequence pos

	bufSz -= sizeof(bufSz);	// exclude packet header space

	return bufSz;
}



/*********************************************************************
* PacketManager class implementation
*********************************************************************/
PacketManager_Base::~PacketManager_Base()
{
	// left blank intentionally
}
void PacketManager_Base::setAgent(MachObject * pMObj)
{
	_pMachObject = pMObj;
}
MachObject & PacketManager_Base::getAgent()
{
	return *_pMachObject;
}
PacketManager_Base::PacketManager_Base() : _pMachObject(nullptr)
{
	// left blank intentionally
}

/*********************************************************************
 * ClntPacketManager class implementation

*********************************************************************/
ClntPacketManager & ClntPacketManager::Instance()
{
	static ClntPacketManager _instance;

	return _instance;
}
void ClntPacketManager::sendPacket(SOCKET sock, std::shared_ptr<Packet_Base> spPk)
{
	send(sock, spPk->get_bufAddr, spPk->get_packetSize, 0);
}
std::shared_ptr<Packet_Base> ClntPacketManager::recvPacket(SOCKET& sock)
{
	//rev Packet을 c 스타일 TCP sending으로 TCP 전송.
	size_t pkSize, recvBytes, headerSize;
	
	headerSize = sizeof(size_t);
	recvBytes = 0;
	while (recvBytes < headerSize)
		recvBytes += recv(sock, (char *)(&pkSize) + recvBytes, headerSize - recvBytes, 0);

	recvBytes = 0;
	char * buf = new char[pkSize + 1];
	while(recvBytes<headerSize) 
		recvBytes += recv(sock, buf + recvBytes, pkSize - recvBytes, 0);
	buf[pkSize] = NULL;

	return extractPacketFromBuffer(buf);
}

/*********************************************************************
 * SvPacketManager class implementation

*********************************************************************/



/*********************************************************************
* Etc. implementation

*********************************************************************/
