#include "PT_SC_Data.h"

/*********************************************
 * Packet from server to client

*********************************************/


std::shared_ptr<Packet_Base> extractSCPacket(char * buf, size_t bufLen)
{
	std::string temp;
	const char * idx = buf;
	while (*idx != '|') {
		temp += *idx;
		idx++;
	}

	int pType = stoi(temp);

	switch ((PTYPE)pType)
	{
	case PTYPE::PT_SC_LOGIN_ACCEPT:
		return std::make_shared<PK_SC_LOGIN_ACCEPT>(pType, buf, bufLen);
	case PTYPE::PT_SC_LOGIN_FAIL:
		return std::make_shared<PK_SC_LOGIN_FAIL>(pType, buf, bufLen);
	case PTYPE::PT_SC_LOBBY_JOINROOMOK:
		return std::make_shared<PK_SC_LOBBY_JOINROOMOK>(pType, buf, bufLen);
	case PTYPE::PT_SC_LOBBY_JOINROOMFAIL:
		return std::make_shared<PK_SC_LOBBY_JOINROOMFAIL>(pType, buf, bufLen);
	case PTYPE::PT_SC_LOBBY_LOAD_ROOMLIST:
		return std::make_shared<PK_SC_LOBBY_LOAD_ROOMLIST>(pType, buf, bufLen);
	case PTYPE::PT_SC_CREATEROOM_OK:
		return std::make_shared<PK_SC_CREATEROOM_OK>(pType, buf, bufLen);
	case PTYPE::PT_SC_CREATEROOM_FAIL:
		return std::make_shared<PK_SC_CREATEROOM_FAIL>(pType, buf, bufLen);
	case PTYPE::PT_SC_CHAT_QUITUSER:
		return std::make_shared<PK_SC_CHAT_QUITUSER>(pType, buf, bufLen);
	case PTYPE::PT_SC_CHAT_CHAT:
		return std::make_shared<PK_SC_CHAT_CHAT>(pType, buf, bufLen);
	case PTYPE::PT_SC_CHAT_LOAD_USERLIST:
		return std::make_shared<PK_SC_CHAT_LOAD_USERLIST>(pType, buf, bufLen);
	default:
		return nullptr;
	}
}