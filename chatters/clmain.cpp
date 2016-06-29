/******************************************************
 * client main function
 *
******************************************************/

#include "header_clnt.h"

ConnectInfo conInfo;

PacketManager pm;

ClientState cState(conInfo, nullptr, &pm);		// Current state

LoginState stLogin(&cState);		// Login state
LobbyState stLobby(&cState);		// Lobby state
CreateRoomState stCrRoom(&cState);	// Create room state
ChatState stChat(&cState);		// Chatting state


int main(int argc, char * argv)	//rev argv »Æ¿Œ
{
	// Establish connecting to server process
	//rev


	// State Running
	//rev


	// Disconnecting and wrap up process
	//rev


	return 0;
}