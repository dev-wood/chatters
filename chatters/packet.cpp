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
{}
int Packet_Base::get_bufSize() const
{
	return 0;	//rev
}
Packet_Base::Packet_Base() : _pm(nullptr)
{}

// PacketManager class
PacketManager::PacketManager(SvMach * mach) : _svMach(mach)
{}
void PacketManager::serialize(Packet_Base & pk)
{
	pk.serialize();
}
void PacketManager::deserialize(Packet_Base & pk)
{
	pk.deserialize();
}
PacketManager::PacketManager()
{}
