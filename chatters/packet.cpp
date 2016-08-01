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
	// Skip header space.
	_skipHeaderg();

	// Do deserailize process depend on each Packet class.
	doDeserialProc();
}
size_t Packet_Base::get_packetSize()
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
void Packet_Base::_skipHeaderg()
{
	_buf.seekg(sizeof(size_t));
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
void PacketManager_Base::addAgent(std::shared_ptr<MachObject> pAgent)
{
	_agentList.push_back(pAgent);
}
void PacketManager_Base::removeAgent(std::shared_ptr<MachObject> pAgent)
{
	// check list if it is empty
	if (_agentList.empty()) {
		std::cout << "Error: PacketManager_Base::removeMachObject(): List is empty." << std::endl;
		exit(1);
	}

	// find target in the list
	auto it = std::find(_agentList.begin(), _agentList.end(), pAgent);
	if (it == _agentList.end()) {
		std::cout << "Error: PacketManager_Base::removeMachObject(): Cannot find target." << std::endl;
		exit(1);
	}
	_agentList.remove(*it);
}
std::shared_ptr<MachObject> PacketManager_Base::getAgent(int pos)
{
	if (pos >= _agentList.size() || pos < 0) {
		std::cout << "Error: PacketManater_Base::getMachObject(): Out of the range." << std::endl;
		exit(1);
	}

	auto it = _agentList.begin();
	std::advance(it, pos);

	return *it;
}
void PacketManager_Base::sendPacket(std::shared_ptr<Packet_Base> pPacket)
{
	if (_outgoingQueue.empty()) {
		_outgoingQueue.push(pPacket);
		return _sending();
	}
	else
	{
		_outgoingQueue.push(pPacket);	
		return;
	}
}
std::shared_ptr<Packet_Base> PacketManager_Base::recvPacket()
{
	while (_incomingQueue.empty())	// wait if there is no received packet.
		;
	auto rtnPk = _incomingQueue.front();
	_incomingQueue.pop();
	return rtnPk;
}
PacketManager_Base::PacketManager_Base()
{
	// left blank intentionally
}
