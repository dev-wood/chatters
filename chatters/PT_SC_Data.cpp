#include "PT_SC_Data.h"
#include "../chatters/header_server.h"

/*********************************************
 * Packet from server to client

*********************************************/

/* PK_SC_LOGIN_ACCEPT class */
PK_SC_LOGIN_ACCEPT::PK_SC_LOGIN_ACCEPT() 
	: Packet_Base(PTYPE::PT_SC_LOGIN_ACCEPT)
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
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
void PK_SC_LOGIN_ACCEPT::_doSerialProc()
{
	_buf << userTk;
}
void PK_SC_LOGIN_ACCEPT::_doDeserialProc()
{
	userTk << _buf;
}

/* PK_SC_LOGIN_FAIL class */
PK_SC_LOGIN_FAIL::PK_SC_LOGIN_FAIL()
	: Packet_Base(PTYPE::PT_SC_LOGIN_FAIL)
{
	// left blank intentionally
}
PK_SC_LOGIN_FAIL::PK_SC_LOGIN_FAIL(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_SC_LOGIN_FAIL, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_LOGIN_FAIL::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
void PK_SC_LOGIN_FAIL::_doSerialProc()
{
	// left blank intentionally
}
void PK_SC_LOGIN_FAIL::_doDeserialProc()
{
	// left blank intentionally
}

/* PK_SC_LOBBY_JOINROOM_ACCEPT class */
PK_SC_LOBBY_JOINROOM_ACCEPT::PK_SC_LOBBY_JOINROOM_ACCEPT()
	: Packet_Base(PTYPE::PT_SC_LOBBY_JOINROOM_ACCEPT)
{
	// left blank intentionally
}
PK_SC_LOBBY_JOINROOM_ACCEPT::PK_SC_LOBBY_JOINROOM_ACCEPT(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_SC_LOBBY_JOINROOM_ACCEPT, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_LOBBY_JOINROOM_ACCEPT::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
void PK_SC_LOBBY_JOINROOM_ACCEPT::_doSerialProc()
{
	// serialize member field depending on the packet type

	// room information serialize
	_buf << roomTk;

	// user list serialize
	int numOfPeer = userList.size();
	_buf << numOfPeer << '|';
	for (const auto& el : userList)
		_buf << el;
}
void PK_SC_LOBBY_JOINROOM_ACCEPT::_doDeserialProc()
{
	// serialize member field depending on the packet type

	std::string token;
	
	// deserialize room information
	roomTk << _buf;

	// deserialize participants list of the room
	std::getline(_buf, token, '|');
	int numOfPeer = stoi(token);
	userList.reserve(numOfPeer);
	for (int i = 0; i < numOfPeer; i++)
	{
		UserInfoToken utk;
		utk << _buf;
		userList.push_back(std::move(utk));
	}
}

/* PK_SC_LOBBY_JOINROOM_FAIL class */
PK_SC_LOBBY_JOINROOM_FAIL::PK_SC_LOBBY_JOINROOM_FAIL()
	: Packet_Base(PTYPE::PT_SC_LOBBY_JOINROOM_FAIL)
{
	// left blank intentionally
}
PK_SC_LOBBY_JOINROOM_FAIL::PK_SC_LOBBY_JOINROOM_FAIL(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(pType, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_LOBBY_JOINROOM_FAIL::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
void PK_SC_LOBBY_JOINROOM_FAIL::_doSerialProc()
{
	// left blank intentionally
}
void PK_SC_LOBBY_JOINROOM_FAIL::_doDeserialProc()
{
	// left blank intentionally
}


/* PK_SC_LOBBY_LOAD_ROOMLIST class */
PK_SC_LOBBY_LOAD_ROOMLIST::PK_SC_LOBBY_LOAD_ROOMLIST()
	: Packet_Base(PTYPE::PT_SC_LOBBY_LOAD_ROOMLIST)
{
	// left bland intentionally
}
PK_SC_LOBBY_LOAD_ROOMLIST::PK_SC_LOBBY_LOAD_ROOMLIST(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(PTYPE::PT_SC_LOBBY_LOAD_ROOMLIST, buf, bufLen)
{
	// left bland intentionally
}
std::shared_ptr<Packet_Base> PK_SC_LOBBY_LOAD_ROOMLIST::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
class SvMach;
void PK_SC_LOBBY_LOAD_ROOMLIST::_doSerialProc()
{
	_buf << shpRmList->size() << '|';

	for (const auto& el : *shpRmList)
	{
		_buf << (el.second)->rtk;
	}	
}
void PK_SC_LOBBY_LOAD_ROOMLIST::_doDeserialProc()
{
	int numOfRoom;
	std::string token;

	// extract the number of room token
	std::getline(_buf, token, '|');
	numOfRoom = std::stoi(token);

	rtkList.reserve(numOfRoom);
	// extract room token
	for (int i = 0; i < numOfRoom; ++i)
	{
		RoomInfoToken rtk;
		rtk << _buf;

		rtkList.push_back(rtk);
	}
}

/* PK_SC_CREATEROOM_OK class */
PK_SC_CREATEROOM_OK::PK_SC_CREATEROOM_OK()
	: Packet_Base(PTYPE::PT_SC_CREATEROOM_OK)
{
	// left blank intentionally
}
PK_SC_CREATEROOM_OK::PK_SC_CREATEROOM_OK(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(pType, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_CREATEROOM_OK::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>();
}
void PK_SC_CREATEROOM_OK::_doSerialProc()
{
	_buf << roomTk;
}
void PK_SC_CREATEROOM_OK::_doDeserialProc()
{
	roomTk << _buf;
}

/* PK_SC_CREATEROOM_FAIL class implement */
PK_SC_CREATEROOM_FAIL::PK_SC_CREATEROOM_FAIL()
	: Packet_Base(PTYPE::PT_SC_CREATEROOM_FAIL)
{
	// left blank intentionally
}
PK_SC_CREATEROOM_FAIL::PK_SC_CREATEROOM_FAIL(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(pType, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_CREATEROOM_FAIL::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
void PK_SC_CREATEROOM_FAIL::_doSerialProc()
{
	// left blank intentionally
}
void PK_SC_CREATEROOM_FAIL::_doDeserialProc()
{
	// left blank intentionally
}

/* PK_SC_CHAT_QUITUSER */
PK_SC_CHAT_QUITUSER::PK_SC_CHAT_QUITUSER()
	: Packet_Base(PTYPE::PT_SC_CHAT_QUITUSER)
{
	// left blank intentionally
}
PK_SC_CHAT_QUITUSER::PK_SC_CHAT_QUITUSER(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(pType, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_CHAT_QUITUSER::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
void PK_SC_CHAT_QUITUSER::_doSerialProc()
{
	_buf << userKey << '|';
}
void PK_SC_CHAT_QUITUSER::_doDeserialProc()
{
	std::string token;

	// extract user key
	std::getline(_buf, token, '|');
	userKey = std::stoi(token);
}

/* PK_SC_CHAT_CHAT class implement */
PK_SC_CHAT_CHAT::PK_SC_CHAT_CHAT()
	: Packet_Base(PTYPE::PT_SC_CHAT_CHAT)
{
	// left blank intentionally
}

PK_SC_CHAT_CHAT::PK_SC_CHAT_CHAT(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(pType, buf, bufLen)
{
	// left blank intentionally
}

std::shared_ptr<Packet_Base> PK_SC_CHAT_CHAT::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}

void PK_SC_CHAT_CHAT::_doSerialProc()
{
	_buf << uKey << '|';
	_buf << chat << '|';
}

void PK_SC_CHAT_CHAT::_doDeserialProc()
{
	std::string token;

	// extract user key
	std::getline(_buf, token, '|');
	uKey = std::stoi(token);

	// extract chat content
	std::getline(_buf, chat, '|');
}

/* PK_SC_CHAT_LOAD_USERLIST class implement */
PK_SC_CHAT_LOAD_USERLIST::PK_SC_CHAT_LOAD_USERLIST()
	: Packet_Base(PTYPE::PT_SC_CHAT_LOAD_USERLIST)
{
	// left blank intentionally
}
PK_SC_CHAT_LOAD_USERLIST::PK_SC_CHAT_LOAD_USERLIST(PTYPE pType, char * buf, size_t bufLen)
	: Packet_Base(pType, buf, bufLen)
{
	// left blank intentionally
}
std::shared_ptr<Packet_Base> PK_SC_CHAT_LOAD_USERLIST::processPacket(MachObject & targetMObject)
{
	return std::shared_ptr<PK_EMPTY>(nullptr);
}
void PK_SC_CHAT_LOAD_USERLIST::_doSerialProc()
{
	_buf << userList.size() << '|';

	for (const auto & uitk : userList)
	{
		_buf << uitk;
	}
}
void PK_SC_CHAT_LOAD_USERLIST::_doDeserialProc()
{
	std::string token;

	// extract the number of user
	std::getline(_buf, token, '|');
	int num = std::stoi(token);

	userList.reserve(num);
	for (int i = 0; i < num; ++i)
	{
		UserInfoToken utk;

		utk << _buf;
		userList.push_back(utk);
	}
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
	case PTYPE::PT_SC_LOBBY_JOINROOM_ACCEPT:
		return std::make_shared<PK_SC_LOBBY_JOINROOM_ACCEPT>(pType, buf, bufLen);
	case PTYPE::PT_SC_LOBBY_JOINROOM_FAIL:
		return std::make_shared<PK_SC_LOBBY_JOINROOM_FAIL>(pType, buf, bufLen);
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
		return std::make_shared<PK_EMPTY>(PTYPE::PT_EMPTY);
	}
}

