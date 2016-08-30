#pragma once

#ifndef _PTYPE_H_
#define _PTYPE_H_

#include "packet.h"

enum struct PTYPE : int
{
	// Packet_Base default packet
	PT_BASE = 0,



	// Server to Client packet
	PT_SC_LOGIN_ACCEPT,
	PT_SC_LOGIN_FAIL,

	PT_SC_LOBBY_JOINROOMOK,
	PT_SC_LOBBY_JOINROOMFAIL,
	PT_SC_LOBBY_LOAD_ROOMLIST,

	PT_SC_CREATEROOM_OK,
	PT_SC_CREATEROOM_FAIL,

	PT_SC_CHAT_QUITUSER,
	PT_SC_CHAT_CHAT,
	PT_SC_CHAT_LOAD_USERLIST,
	// 추후 확장 가능한 기능 
	PT_SC_CHAT_GUESTSTATEREADY,
	PT_SC_CHAT_GUESTSTATENOTREADY,
	PT_SC_CHAT_ROOMSTATEPLAYING,
	PT_SC_CHAT_ROOMSTATENOTPLAYING,



	// Client to Server packet
	PT_CS_LOGIN_REQUEST,

	PT_CS_LOBBY_JOINROOM,
	PT_CS_LOBBY_LOAD_ROOMLIST,

	PT_CS_CREATEROOM_CREATEROOM,

	PT_CS_CHAT_QUITROOM,
	PT_CS_CHAT_CHAT,
	// 추후 확장 가능한 기능
	PT_CS_CHAT_MYSTATEREADY,
	PT_CS_CHAT_MYSTATENOTREADY,
	PT_CS_CHAT_ROOMSTATEPLAYING,
	PT_CS_CHAT_ROOMSTATENOTPLAYING
};

std::shared_ptr<Packet_Base> extractPacketFromBuffer(char * buf)
{
	int pType;
	memcpy_s(&pType, sizeof(int), buf, sizeof(int));

	switch ((PTYPE)pType)
	{
	case PTYPE::PT_CS_LOGIN_REQUEST:
		return std::make_shared<PK_CS_LOGIN_REQUEST>((PTYPE)pType, buf);
	case PTYPE::PT_CS_LOBBY_JOINROOM:
		return std::make_shared<PK_CS_LOBBY_JOINROOM>((PTYPE)pType, buf);
	case PTYPE::PT_CS_LOBBY_LOAD_ROOMLIST:
		return std::make_shared<PK_CS_LOBBY_LOAD_ROOMLIST>((PTYPE)pType, buf);
	case PTYPE::PT_CS_CREATEROOM_CREATEROOM:
		return std::make_shared<PK_CS_CREATEROOM_CREATEROOM>((PTYPE)pType, buf);
	case PTYPE::PT_CS_CHAT_QUITROOM:
		return std::make_shared<PK_CS_CHAT_QUITROOM>((PTYPE)pType, buf);
	case PTYPE::PT_CS_CHAT_CHAT:
		return std::make_shared<PK_CS_CHAT_CHAT>((PTYPE)pType, buf);
	default:
		return nullptr;
	}
}



#endif   // !_PTYPE_H_