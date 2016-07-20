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
	// Skip header space.
	_skipHeaderg();

	// Do deserailize process depend on each Packet class.
	doDeserialProc();
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
void Packet_Base::_skipHeaderg()
{
	_buf.seekg(sizeof(size_t));
}
size_t Packet_Base::_packetSize()
{
	return _bufSize() + sizeof(size_t);
}
const char * Packet_Base::get_bufAddr() const
{
	return _buf.str().c_str();
}
const PkInfo & Packet_Base::get_pkInfo() const
{
	return _pkInfo;
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
void Packet_Base::set_pm(PacketManager& pm)
{
	_pm = &pm;
}
Packet_Base::Packet_Base() : _id(PTYPE::PT_BASE), _pm(nullptr), _pkInfo()
{
	// left blank intentionally
}
Packet_Base::Packet_Base(PTYPE pt) : _id(pt), _pm(nullptr), _pkInfo()
{
	// left blank intentionally
}
Packet_Base::~Packet_Base()
{
	// left blank intentionally
}



/*********************************************************************
* PacketManager class implementation
*********************************************************************/
PacketManager::PacketManager() : _mach(nullptr), _pk(nullptr)
{
	// left blank intentionally
}
PacketManager::PacketManager(MachObject * mach) : _mach(mach)
{
	// left black intentionally
}
void PacketManager::_setPacket(Packet_Base & pk)
{
	set_pk(&pk);
}
void PacketManager::_serialize()
{
	_pk->serialize();
}
void PacketManager::_deserialize()
{
	_pk->deserialize();
}
Packet_Base & PacketManager::get_pk()
{
	return *_pk;
}
MachObject & PacketManager::get_mach()
{
	return *_mach;
}
void PacketManager::set_pk(Packet_Base * pk)
{
	_pk = pk;
}
void PacketManager::set_mach(MachObject * mach)
{
	_mach = mach;
}
