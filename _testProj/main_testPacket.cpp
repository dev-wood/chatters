#include "../chatters/header_server.h"

void main()
{
	Packet_Base * ppk;

	///////////////////////////////////////////////////////////////////////////////////
	// Packet to test put below

	// Packet type definition
	PK_CS_CHAT_CHAT packet;
	ppk = &packet;
	
	// Packet information
	dynamic_cast<PK_CS_CHAT_CHAT *>(ppk)->roomKey = 10;
	dynamic_cast<PK_CS_CHAT_CHAT *>(ppk)->chat = "Hello, World!";
	
	// locate above
	///////////////////////////////////////////////////////////////////////////////////
	
	// packet serialize
	ppk->serialize();

	// const char *
	const char * ch = ppk->get_bufAddr();

	int pkSz = ppk->get_packetSize();
	int bufSz = pkSz - sizeof(size_t);
	
	// char array
	char charr[100];
	memcpy_s(charr, 100, ppk->get_buf().str().c_str(), pkSz);
	charr[pkSz] = NULL;

	char charr2[100];
	memcpy_s(charr2, 100, charr + 4, bufSz);

	auto shpk = extractCSPacket(charr2, bufSz);
	shpk->deserialize();
	
	return;
}

//////////////////////////////////////////////////////////////////////////
// Packet to test put below

// Packet type definition

// Packet information

// locate above
//////////////////////////////////////////////////////////////////////////