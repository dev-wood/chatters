#pragma once

#ifndef _PT_CS_DATA_H_
#define _PT_CS_DATA_H_

#include "packet.h"



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