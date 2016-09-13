#include "PT_SC_Data.h"

/*********************************************
 * Packet from server to client

*********************************************/
PK_SC_LOGIN_ACCEPT::PK_SC_LOGIN_ACCEPT() : Packet_Base(PTYPE::PT_SC_LOGIN_ACCEPT)
{
	// left blank intentionally
}
PK_SC_LOGIN_ACCEPT::PK_SC_LOGIN_ACCEPT(PTYPE pType, char * buf, size_t bufLen)
	:Packet_Base(PTYPE::PT_SC_LOGIN_ACCEPT, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_LOGIN_ACCEPT::processPacket(MachObject & targetMObject)
{
	// left blank intentionally
	return std::shared_ptr<Packet_Base>(nullptr);
}
void PK_SC_LOGIN_ACCEPT::doSerialProc()
{
	// left blank intentionally
}
void PK_SC_LOGIN_ACCEPT::doDeserialProc()
{
	userTk << _buf;
}



/*********************************************
* etc. function

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
