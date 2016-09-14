#pragma once

#ifndef _PTYPE_H_
#define _PTYPE_H_



/*********************************************************************
 * Pre-declaration
	-
*********************************************************************/



/*********************************************************************
 * PTYPE declaration
	-
*********************************************************************/
enum struct PTYPE : int
{
	// Packet_Base default packet
	PT_BASE = 0,



	// Server to Client packet
	PT_SC_LOGIN_ACCEPT,
	PT_SC_LOGIN_FAIL,

	PT_SC_LOBBY_JOINROOM_ACCEPT,
	PT_SC_LOBBY_JOINROOM_FAIL,
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



/*********************************************************************
 * etc. functions
	- 
*********************************************************************/



#endif   // !_PTYPE_H_