#include "PT_CS_Data.h"
#include "header_server.h"



PK_CS_LOGIN_REQUEST::PK_CS_LOGIN_REQUEST() : Packet_Base(PTYPE::PT_CS_LOGIN_REQUEST)
{
	// left blank intentionally
}
PK_CS_LOGIN_REQUEST::PK_CS_LOGIN_REQUEST(PTYPE ptype, const char * buf, size_t bufLen) 
	: Packet_Base(PTYPE::PT_CS_LOGIN_REQUEST, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_CS_LOGIN_REQUEST::processPacket(MachObject & targetMObject)
{
	auto & agent = dynamic_cast<SvMach&>(targetMObject);

	if (agent.db_signin(userId, userPassword))
	{	// sign in success
		// build return packet
		auto shPk = std::make_shared<PK_SC_LOGIN_ACCEPT>();

		// set processing result
		shPk->setProcessInfo(PkInfo::ProcCode::SUCCESS);

		return shPk;
	}
	else
	{	//sign in failed
		// build return packet
		auto shPk = std::make_shared<PK_SC_LOGIN_FAIL>();

		// set processing result
		shPk->setProcessInfo(PkInfo::ProcCode::SUCCESS);

		return shPk;
	}
}
void PK_CS_LOGIN_REQUEST::doSerialProc()
{
	_buf << userId << '|' << userPassword << '|';
}

void PK_CS_LOGIN_REQUEST::doDeserialProc()
{
	std::getline(_buf, userId, '|');
	std::getline(_buf, userPassword, '|');
}

PK_CS_LOBBY_JOINROOM::PK_CS_LOBBY_JOINROOM() : Packet_Base(PTYPE::PT_CS_LOBBY_JOINROOM)
{
	// left blank intentionally
}

PK_CS_LOBBY_JOINROOM::PK_CS_LOBBY_JOINROOM(PTYPE ptype, const char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_CS_LOBBY_JOINROOM, buf, bufLen)
{
	// left blank intentionally
}

std::shared_ptr<Packet_Base> PK_CS_LOBBY_JOINROOM::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<Packet_Base>();
}

void PK_CS_LOBBY_JOINROOM::doSerialProc()
{
}

void PK_CS_LOBBY_JOINROOM::doDeserialProc()
{
}

PK_CS_LOBBY_LOAD_ROOMLIST::PK_CS_LOBBY_LOAD_ROOMLIST() : Packet_Base(PTYPE::PT_CS_LOBBY_LOAD_ROOMLIST)
{
	// left blank intentionally
}

PK_CS_LOBBY_LOAD_ROOMLIST::PK_CS_LOBBY_LOAD_ROOMLIST(PTYPE ptype, const char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_CS_LOBBY_LOAD_ROOMLIST, buf, bufLen)
{
	// left blank intentionally
}

std::shared_ptr<Packet_Base> PK_CS_LOBBY_LOAD_ROOMLIST::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<Packet_Base>();
}

void PK_CS_LOBBY_LOAD_ROOMLIST::doSerialProc()
{
}

void PK_CS_LOBBY_LOAD_ROOMLIST::doDeserialProc()
{
}

PK_CS_CREATEROOM_CREATEROOM::PK_CS_CREATEROOM_CREATEROOM() : Packet_Base(PTYPE::PT_CS_CREATEROOM_CREATEROOM)
{
	// left blank intentionally
}

PK_CS_CREATEROOM_CREATEROOM::PK_CS_CREATEROOM_CREATEROOM(PTYPE ptype, const char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_CS_CREATEROOM_CREATEROOM, buf, bufLen)
{
	// left blank intentionally
}

std::shared_ptr<Packet_Base> PK_CS_CREATEROOM_CREATEROOM::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<Packet_Base>();
}

void PK_CS_CREATEROOM_CREATEROOM::doSerialProc()
{
}

void PK_CS_CREATEROOM_CREATEROOM::doDeserialProc()
{
}

PK_CS_CHAT_QUITROOM::PK_CS_CHAT_QUITROOM() : Packet_Base(PTYPE::PT_CS_CHAT_QUITROOM)
{
	// left blank intentionally
}

PK_CS_CHAT_QUITROOM::PK_CS_CHAT_QUITROOM(PTYPE ptype, const char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_CS_CHAT_QUITROOM, buf, bufLen)
{
	// left blank intentionally
}

std::shared_ptr<Packet_Base> PK_CS_CHAT_QUITROOM::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<Packet_Base>();
}

void PK_CS_CHAT_QUITROOM::doSerialProc()
{
}

void PK_CS_CHAT_QUITROOM::doDeserialProc()
{
}

PK_CS_CHAT_CHAT::PK_CS_CHAT_CHAT() : Packet_Base(PTYPE::PT_CS_CHAT_CHAT)
{
	// left blank intentionally
}

PK_CS_CHAT_CHAT::PK_CS_CHAT_CHAT(PTYPE ptype, const char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_CS_CHAT_CHAT, buf, bufLen)
{
	// left blank intentionally
}

std::shared_ptr<Packet_Base> PK_CS_CHAT_CHAT::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<Packet_Base>();
}

void PK_CS_CHAT_CHAT::doSerialProc()
{
}

void PK_CS_CHAT_CHAT::doDeserialProc()
{
}



std::shared_ptr<Packet_Base> extractCSPacket(char * buf, size_t bufLen)
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
	case PTYPE::PT_CS_LOGIN_REQUEST:
		return std::make_shared<PK_CS_LOGIN_REQUEST>((PTYPE)pType, buf, bufLen);
	case PTYPE::PT_CS_LOBBY_JOINROOM:
		return std::make_shared<PK_CS_LOBBY_JOINROOM>((PTYPE)pType, buf, bufLen);
	case PTYPE::PT_CS_LOBBY_LOAD_ROOMLIST:
		return std::make_shared<PK_CS_LOBBY_LOAD_ROOMLIST>((PTYPE)pType, buf, bufLen);
	case PTYPE::PT_CS_CREATEROOM_CREATEROOM:
		return std::make_shared<PK_CS_CREATEROOM_CREATEROOM>((PTYPE)pType, buf, bufLen);
	case PTYPE::PT_CS_CHAT_QUITROOM:
		return std::make_shared<PK_CS_CHAT_QUITROOM>((PTYPE)pType, buf, bufLen);
	case PTYPE::PT_CS_CHAT_CHAT:
		return std::make_shared<PK_CS_CHAT_CHAT>((PTYPE)pType, buf, bufLen);
	default:
		return nullptr;
	}
}