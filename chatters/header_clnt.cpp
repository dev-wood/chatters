#include "header_clnt.h"

/***********************************************************
 * global variable declaration
	- state machine object
 *
 ***********************************************************/

ClientState cState;		// Current state

LoginState stLogin(&cState);		// Login state
LobbyState stLobby(&cState);		// Lobby state
CreateRoomState stCrRoom(&cState);	// Create room state
ChatState stChat(&cState);		// Chatting state



/***********************************************************
 * client class member definitions
 *
 ***********************************************************/

const WSADATA & ConnectInfo::get_wsaData() const
{
	return _wsaData;
}

const SOCKET & ConnectInfo::get_sock() const
{
	return _sock;
}

const sockaddr & ConnectInfo::get_servaddr() const
{
	return _servAddr;
}

WSADATA & ConnectInfo::get_wsaData()
{
	return _wsaData;
}

SOCKET & ConnectInfo::get_sock()
{
	return _sock;
}

sockaddr & ConnectInfo::get_servaddr()
{
	return _servAddr;
}

StateMachine::StateMachine() : _pClientState(nullptr)
{
	// left blank intentionally
}

StateMachine::StateMachine(ClientState * pClntState) : _pClientState(pClntState)
{
	// left blank intentionally
}

void StateMachine::changeState(ClientState * cState, StateMachine * nextState)
{
	cState->_pState = nextState;
	return;
}

LoginState::LoginState(ClientState * pClnt) : StateMachine(pClnt)
{
	// left blank intentionally
}

LoginState::LoginState(LoginState && loginState)
{
	*this = std::move(loginState);
}

void LoginState::init()
{
	_id.clear();
	_pw.clear();
}

bool LoginState::handle()
{
	init();

	std::cout << "Please put your information" << std::endl;
	std::cout << "ID:";
	std::cin >> _id;
	std::cout << "PASSWORD:";
	std::cin >> _pw;

	//rev handle 내부 함수 작성
	// packet generating

	// packet set in PacketManager

	
}

LoginState & LoginState::operator=(LoginState && loginState)
{
	_id.swap(loginState._id);
	_pw.swap(loginState._pw);

	return *this;
}

LoginState::LoginState()
{
	// left blank intentionally
}

LoginState::LoginState(const LoginState &)
{
	// left blank intentionally
}

LoginState & LoginState::operator=(const LoginState &)
{
	return *this;
}

bool ClientState::request()
{
	return _pState->handle();
}

const ConnectInfo & ClientState::get_conInfo() const
{
	return _conInfo;
}

const UserInfoToken & ClientState::get_myInfo() const
{
	return _myInfo;
}

ConnectInfo & ClientState::get_conInfo()
{
	return _conInfo;
}

UserInfoToken & ClientState::get_myInfo()
{
	return _myInfo;
}

void ClientState::set_conInfo(ConnectInfo cInfo)
{
	_conInfo = std::move(cInfo);
}

void ClientState::set_myInfo(UserInfoToken uInfoToken)
{
	_myInfo = std::move(uInfoToken);
}



/***********************************************************
* etc functions definitions
*
***********************************************************/

std::string & stringCheck(std::string & str)
{
	for(auto & el : str)
	{
		if (el == '|')
			el = '_';
	}

	return str;
}


