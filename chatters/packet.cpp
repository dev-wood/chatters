#include "packet.h"



/*********************************************************************
* ProcessInfo class implementation
*********************************************************************/
ProcessInfo::ProcessInfo() : _code(EMPTY), _msg("")
{
	// left blank intentionally
}
ProcInfo::ProcCode ProcessInfo::get_code() const
{
	return _code;
}
std::string ProcessInfo::get_msg() const
{
	return _msg;
}
void ProcessInfo::set_code(ProcCode code)
{
	_code = code;
}
void ProcessInfo::set_msg(std::string && str)
{
	_msg = str;
}




/*********************************************************************
 * Packet_Base class implementation 
 *********************************************************************/
Packet_Base::Packet_Base(PTYPE pType, const char * buf, size_t bufLen) : Packet_Base(pType)
{
	//_buf << buf;
	_buf.write(buf, bufLen);
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
	_buf << ptoi(id) << '|';

	// Do serialize process depend on each Packet class.
	_doSerialProc();

	// Write header at the last of serialize process.
	_writeHeader();
}
void Packet_Base::deserialize()
{
	// Ignore(skip) packet id
	_buf.ignore(std::numeric_limits<std::streamsize>::max(), '|');

	// Do deserailize process depend on each Packet class.
	_doDeserialProc();
}
void Packet_Base::setProcessInfo(ProcInfo::ProcCode resCode, std::string&& msg)
{
	_ProcInfo.set_code(resCode);
	_ProcInfo.set_msg(std::move(msg));	//rev //? 작동 확인
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
const std::stringstream & Packet_Base::get_buf() const
{
	return _buf;
}
const ProcInfo & Packet_Base::get_ProcInfo() const
{
	return _ProcInfo;
}
Packet_Base & Packet_Base::operator<<(const char * buf)
{
	_buf << buf;
	return *this;
}
Packet_Base::Packet_Base(PTYPE pt) : id(pt), _ProcInfo()
{
	sockList.reserve(1);
}
Packet_Base::Packet_Base() : id(PTYPE::PT_BASE), _ProcInfo()
{
	sockList.reserve(1);
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
	_buf.seekp(0, _buf.beg);	// move input sequence pos to header space

	_buf.write((const char*)(&bufSize), sizeof(size_t));	// write packet size in header space

	_buf.seekp(prevPos);	// restore previous input sequence pos	
}
size_t Packet_Base::_bufSize()
{
	//size_t bufSz = 0;
	//auto cPos = _buf.tellg();	// store current input sequence pos
	//_buf.seekg(_buf.end);
	//bufSz = static_cast<int>(_buf.tellg());
	
	//_buf.seekg(cPos);	// restore previous input sequence pos

	//bufSz -= sizeof(bufSz);	// exclude packet header space

	//return bufSz;
	
	size_t bufSz = 0;
	_buf.seekg(0, _buf.end);
	bufSz = static_cast<int>(_buf.tellg());

	_buf.seekg(0, _buf.beg);	// restore previous input sequence pos

	bufSz -= sizeof(size_t);	// exclude packet header space

	return bufSz;
}

/* PK_EMPTY */
PK_EMPTY::PK_EMPTY() : Packet_Base(PTYPE::PT_EMPTY)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_EMPTY::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>();
}
void PK_EMPTY::_doSerialProc()
{
	// left blank intentionally
}
void PK_EMPTY::_doDeserialProc()
{
	// left blank intentionally
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
* Etc. implementation

*********************************************************************/

