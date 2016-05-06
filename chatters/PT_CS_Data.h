#pragma once

#ifndef _PT_CS_DATA_H_
#define _PT_CS_DATA_H_

#include "packet.h"

namespace PTYPE {
	enum struct CS : int
	{
		// Client to Server packet
		LOGIN_REQUEST = 0,

		LOBBY_JOINROOM,
		LOBBY_LOAD_ROOMLIST,

		CREATEROOM_CREATEROOM,

		CHAT_QUITROOM,
		CHAT_CHAT,
		// 추후 확장 가능한 기능
		CHAT_MYSTATEREADY,
		CHAT_MYSTATENOTREADY,
		CHAT_ROOMSTATEPLAYING,
		CHAT_ROOMSTATENOTPLAYING
	};
};

//rev
struct PK_CS_LOGIN_REQUEST : Packet_Base
{};
struct PK_CS_LOBBY_JOINROOM : Packet_Base
{};
struct PK_CS_LOBBY_LOAD_ROOMLIST : Packet_Base
{};
struct PK_CS_CREATEROOM_CREATEROOM : Packet_Base
{};
struct PK_CS_CHAT_QUITROOM : Packet_Base
{};
struct PK_CS_CHAT_CHAT : Packet_Base
{};

#endif	// !_PT_CS_DATA_H_