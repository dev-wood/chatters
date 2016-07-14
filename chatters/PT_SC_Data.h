#pragma once

#ifndef _PT_SC_DATA_H_
#define _PT_SC_DATA_H_

#include "packet.h"

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