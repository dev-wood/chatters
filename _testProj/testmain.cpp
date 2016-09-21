#include "../chatters/header_server.h"

void main()
{
	std::stringstream ss;

	//ss << static_cast<int>(PTYPE::PT_CS_CHAT_CHAT) << '|' << "Hello, world!" << '|';
	Packet_Base * ppk = new PK_CS_CHAT_CHAT();
	dynamic_cast<PK_CS_CHAT_CHAT *>(ppk)->roomKey = 10;
	dynamic_cast<PK_CS_CHAT_CHAT *>(ppk)->chat = "Hello, World!";
	
	ppk->serialize();

	int pkSz = ppk->get_packetSize();
	int bufSz = ppk->get_bufSize();

	int i = 1;

	//auto shpk = extractCSPacket(const_cast<char *>(ss.str().c_str()), ss.str().length());

	//auto shpk = extractCSPacket(const_cast<char *>(pk.get_bufAddr())+sizeof(size_t), pk.get_bufSize());

	delete ppk;

	return;
}