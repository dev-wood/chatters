#pragma once

#ifndef _PT_SC_DATA_H_
#define _PT_SC_DATA_H_

#include "packet.h"

namespace PTYPE
{
	enum struct SC : int
	{
		// Server to Client packet
		LOGIN_ACCEPT = 0,
		LOGIN_FAIL,

		LOBBY_JOINROOMOK,
		LOBBY_JOINROOMFAIL,
		LOBBY_LOAD_ROOMLIST,

		CREATEROOM_OK,
		CREATEROOM_FAIL,

		CHAT_QUITUSER,
		CHAT_CHAT,
		CHAT_LOAD_USERLIST,
		// 추후 확장 가능한 기능 
		CHAT_GUESTSTATEREADY,
		CHAT_GUESTSTATENOTREADY,
		CHAT_ROOMSTATEPLAYING,
		CHAT_ROOMSTATENOTPLAYING
	};
};

//rev
struct PK_SC_LOGIN_ACCEPT : public Packet_Base
{
	PK_SC_LOGIN_ACCEPT(const PacketManager& pm) : Packet_Base(pm) { }
	void serialize();
	void deserialize();

protected:
	PK_SC_LOGIN_ACCEPT();
	void process();
};

struct PK_SC_LOGIN_FAIL : Packet_Base
{

};
struct PK_SC_LOBBY_JOINROOMOK : Packet_Base
{
};
struct PK_SC_LOBBY_JOINROOMFAIL : Packet_Base
{
};
struct PK_SC_LOBBY_LOAD_ROOMLIST : Packet_Base
{
};
struct PK_SC_CREATEROOM_OK : Packet_Base
{
};
struct PK_SC_CREATEROOM_FAIL : Packet_Base
{
};
struct PK_SC_CHAT_QUITUSER : Packet_Base
{
};
struct PK_SC_CHAT_CHAT : Packet_Base
{
};
struct PK_SC_CHAT_LOAD_USERLIST : Packet_Base
{
};

#endif // !_PT_SC_DATA_H_