/******************************************************
 * client main function
 *
******************************************************/

#include "header_clnt.h"



int main(int argc, char * argv)	//rev argv Ȯ��
{
	/****************************************************************
	 * variable declaration
	 *

	//rev static���� �����ؼ� ��𼭵� ������ �����������ȴµ�..
	//		�� �� ���õǰ� �����ϰ� �ʱ�ȭ�� ����� ������?
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