#include "PT_SC_Data.h"

/*********************************************
 * Packet from server to client

*********************************************/

void PK_SC_LOGIN_ACCEPT::serialize()
{
	//_buf << ptoi(PType::PT_SC) << '|';// << _pm._svMach->	//rev
	_buf << ptoi(PTYPE::SC::LOGIN_ACCEPT) << '|';
}

void PK_SC_LOGIN_ACCEPT::deserialize()
{
	//rev
}

PK_SC_LOGIN_ACCEPT::PK_SC_LOGIN_ACCEPT() : Packet_Base()
{
}
