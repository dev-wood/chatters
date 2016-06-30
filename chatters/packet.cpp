#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"
#include "packet.h"

int Packet_Base::ptoi(PTYPE::SC pt)
{
	return static_cast<int>(pt);
}
int Packet_Base::ptoi(PTYPE::CS pt)
{
	return static_cast<int>(pt);
}
Packet_Base::Packet_Base(PacketManager * const pm) : _pm(pm) 
{
	_setHeaderSpace();
}
int Packet_Base::get_bufSize()
{
	int bufSz = 0;
	auto cPos = _buf.tellg();	// store current input sequence pos
	_buf.seekg(_buf.end);
	bufSz = static_cast<int>(_buf.tellg());

	_buf.seekg(cPos);	// restore previous input sequence pos

	bufSz -= sizeof(size_t);	// exclude packet header space
	
	return bufSz;
}
Packet_Base::Packet_Base() : _pm(nullptr)
{
	_setHeaderSpace();
}

void Packet_Base::_setHeaderSpace()
{
	int headerSz = sizeof(size_t);
	for (int i = 0; i < headerSz; ++i)
		_buf << '0';
}

void Packet_Base::_skipHeaderg()
{
	_buf.seekg(sizeof(size_t));
}

// PacketManager class
PacketManager::PacketManager() : _mach(nullptr), _pk(nullptr)
{
	// left blank intentionally
}
PacketManager::PacketManager(MachObject * mach) : _mach(mach)
{
	// left black intentionally
}
void PacketManager::_setPacket(Packet_Base * pk)
{
	set_pk(pk);
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
