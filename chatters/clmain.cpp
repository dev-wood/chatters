/******************************************************
 * client main function
 *
******************************************************/

#include "header_clnt.h"



int main(int argc, char * argv)	//rev argv 확인
{
	/****************************************************************
	 * variable declaration
	 *

	//rev static으로 선언해서 어디서든 접근이 가능해져버렸는데..
	//		좀 더 세련되게 선언하고 초기화할 방법이 없을까?
	 ****************************************************************/
	static PacketManager pm;

	// declaration of context of client state
	static ClientState cState;		// Current state

	// declaration of client state
	static LoginState stLogin(&cState);		// Login state
	static LobbyState stLobby(&cState);		// Lobby state
	static CreateRoomState stCrRoom(&cState);	// Create room state
	static ChatState stChat(&cState);		// Chatting state

	init();
	// Establish connecting to server process
	//rev

	cState.init();

	// State Running
	bool exitFlag = false;
	do {
		exitFlag = cState.request();
	} while (!exitFlag);
	//rev

	// Disconnecting and wrap up process
	//rev


	return 0;
}