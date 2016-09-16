#include "PT_CS_Data.h"
#include "header_server.h"


/*********************************************
 * Packet from client to server

*********************************************/
/* PK_CS_LOGIN_REQUEST class */
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
		// add user on user list	//rev
		if (agent.addUser(userId, sockList[0]))
		{
			// build return packet
			auto rtnShPk = std::make_shared<PK_SC_LOGIN_ACCEPT>();

			// set processing result
			rtnShPk->setProcessInfo(PkInfo::ProcCode::SUCCESS);

			// register packet receiver
			rtnShPk->sockList.push_back(sockList[0]);
			
			return rtnShPk;
		}
		else
		{
			// build return packet
			auto rtnShPk = std::make_shared<PK_SC_LOGIN_FAIL>();

			// set processing result
			rtnShPk->setProcessInfo(PkInfo::ProcCode::FAIL, "SvMach::addUser(..) failed.");

			// register packet receiver
			rtnShPk->sockList.push_back(sockList[0]);

			return rtnShPk;
		}
	}
	else
	{	//sign in failed
		// build return packet
		auto rtnShPk = std::make_shared<PK_SC_LOGIN_FAIL>();

		// set processing result
		rtnShPk->setProcessInfo(PkInfo::ProcCode::SUCCESS);

		// register packet receiver
		rtnShPk->sockList.push_back(sockList[0]);

		return rtnShPk;
	}
}
void PK_CS_LOGIN_REQUEST::_doSerialProc()
{
	_buf << userId << '|' << userPassword << '|';
}
void PK_CS_LOGIN_REQUEST::_doDeserialProc()
{
	std::getline(_buf, userId, '|');
	std::getline(_buf, userPassword, '|');
}

/* PK_CS_LOBBY_JOINROOM class */
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
	// casting agent
	auto& agent = dynamic_cast<SvMach &>(targetMObject);

	// packet processing procedure
	if (agent.joinRoom(roomKey, userKey))
	{	// join room success
		// build return packet
		auto rtnShPk = std::make_shared<PK_SC_LOBBY_JOINROOM_ACCEPT>();

		// set processing result
		rtnShPk->setProcessInfo(PkInfo::ProcCode::SUCCESS);

		// register packet receiver
		rtnShPk->sockList.push_back(sockList[0]);

		return rtnShPk;
	}
	else {	// join room failed
		// build return packet
		auto rtnShPk = std::make_shared<PK_SC_LOBBY_JOINROOM_FAIL>();

		// set processing result
		rtnShPk->setProcessInfo(PkInfo::ProcCode::FAIL, "Join room failed.");

		// register packet receiver
		rtnShPk->sockList.push_back(sockList[0]);

		return rtnShPk;
	}
}
void PK_CS_LOBBY_JOINROOM::_doSerialProc()
{
	// serialize member field depending on the packet type
	
	_buf << userKey << '|';
	_buf << roomKey << '|';
}
void PK_CS_LOBBY_JOINROOM::_doDeserialProc()
{
	// deserialize member field depending on the packet type
	
	std::string token;

	// extract user key
	std::getline(_buf, token, '|');
	userKey = std::stoi(token);

	// extract room key
	std::getline(_buf, token, '|');
	roomKey = std::stoi(token);
}

/* PK_CS_LOBBY_LOAD_ROOMLIST class */
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
	return std::shared_ptr<Packet_Base>(nullptr);
}
void PK_CS_LOBBY_LOAD_ROOMLIST::_doSerialProc()
{
	// serialize member field depending on the packet type

}
void PK_CS_LOBBY_LOAD_ROOMLIST::_doDeserialProc()
{
	// deserialize member field depending on the packet type

}

/* PK_CS_CREATEROOM_CREATEROOM class */
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
void PK_CS_CREATEROOM_CREATEROOM::_doSerialProc()
{
	// serialize member field depending on the packet type

}
void PK_CS_CREATEROOM_CREATEROOM::_doDeserialProc()
{
	// deserialize member field depending on the packet type

}

/* PK_CS_CHAT_QUITROOM class */
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
void PK_CS_CHAT_QUITROOM::_doSerialProc()
{
	// serialize member field depending on the packet type

}
void PK_CS_CHAT_QUITROOM::_doDeserialProc()
{
	// deserialize member field depending on the packet type

}

/* PK_CS_CHAT_CHAT class */
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
void PK_CS_CHAT_CHAT::_doSerialProc()
{
	// serialize member field depending on the packet type

}
void PK_CS_CHAT_CHAT::_doDeserialProc()
{
	// deserialize member field depending on the packet type

}



/*********************************************
 * Etc. function

*********************************************/
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

/*
{
	// casting agent
	auto& agent = dynamic_cast<SvMach &>(targetMObject);

	// packet processing procedure

	// build return packet
	auto rtnShPk = std::make_shared<..>(..);

	// set processing result
	rtnShPk->setProcessInfo(..);

	// register packet receiver
	rtnShPk->sockList.push_back(..);

	return rtnShPk;
}
*/