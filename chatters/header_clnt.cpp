#include "header_clnt.h"

/***********************************************************
 * global variable declaration
	- state machine object
 *
 ***********************************************************/




/***********************************************************
 * client class member definitions
 *
 ***********************************************************/

ConnectInfo::ConnectInfo()
{
	// left blank intentionally
}

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

StateMachine::StateMachine() : _pContext(nullptr)
{
	// left blank intentionally
}

StateMachine::StateMachine(ClientState * pClntState) : _pContext(pClntState)
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
	std::cout << "All information cannot have character '|'." << std::endl;
	std::cout << "ID:";
	std::cin >> _id;
	std::cout << "PASSWORD:";
	std::cin >> _pw;

	//rev handle 내부 함수 작성
	// packet generating
	PK_CS_LOGIN_REQUEST packet;

	// packet set in PacketManager
	_pContext->_pPM->_setPacket(packet);
	_pContext->_pPM->_serialize();

	// sending packet
	_pContext->_sending();

	// receiving packet
	_pContext->_receiving();

	// process packet
	_pContext->_pPM->_deserialize();	
}

LoginState & LoginState::operator=(LoginState && loginState)
{
	if (&loginState != this) {
		_id.swap(loginState._id);
		_pw.swap(loginState._pw);
	}

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

ClientState::ClientState(ConnectInfo conInfo, StateMachine * pState, PacketManager * pm) : _pPM(pm), _conInfo(conInfo), _pState(pState)
{
	// left blank intentionally
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

PacketManager & ClientState::get_pPm()
{
	return *_pPM;
}

void ClientState::set_conInfo(ConnectInfo cInfo)
{
	_conInfo = std::move(cInfo);
}

void ClientState::set_myInfo(UserInfoToken uInfoToken)
{
	_myInfo = std::move(uInfoToken);
}

void ClientState::set_pPM(PacketManager * pm)
{
	_pPM = pm;
}

ClientState::ClientState() : _conInfo(), _myInfo(), _pState(nullptr), _pPM(nullptr)
{
	// left blanck intentionally
}



/***********************************************************
* etc functions definitions
*
***********************************************************/

void init()
{
	//rev
}

std::string & stringCheck(std::string & str)
{
	for(auto & el : str)
	{
		if (el == '|')
			el = '_';
	}

	return str;
}


