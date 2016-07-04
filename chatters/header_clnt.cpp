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

ClientState & State::get_pContext()
{
	return *_pContext;
}

void State::set_pContext(ClientState & context)
{
	_pContext = &context;
}

State::State() : _pContext(nullptr)
{
	// left blank intentionally
}

State::State(ClientState * pClntState) : _pContext(pClntState)
{
	// left blank intentionally
}

void LoginState::init()
{
	_id.clear();
	_pw.clear();
}

void LoginState::run()
{
	init();

	std::cout << "Please put your information" << std::endl;
	std::cout << "All information cannot have character '|'." << std::endl;
	std::cout << "ID:";
	std::cin >> _id;
	std::cout << "PASSWORD:";
	std::cin >> _pw;

	// packet generating
	PK_CS_LOGIN_REQUEST packet;

	// packet set in PacketManager
	get_pContext().get_pPM()._setPacket(packet);
	get_pContext().get_pPM()._serialize();

	// sending packet
	_pContext->_sending();

	// receiving packet
	_pContext->_receiving();

	// process packet
	_pContext->get_pPM()._deserialize();
}

LoginState & LoginState::Instance()
{
	static LoginState _instance;
	return _instance;
}

LoginState::LoginState()
{
	// left blank intentionally
}

LoginState::LoginState(const LoginState &)
{
	// left blank intentionally
}

LobbyState & LobbyState::Instance()
{
	static LobbyState _instance;
	return _instance;
}

void LobbyState::init()
{
	// roomList initializing and reserve memory space.
	_roomList.clear();
	_roomList.reserve(CHATTERS::NUM_OF_ROOM_PER_PAGE);
	
	return;
}

bool LobbyState::handle()
{	

	// 
	std::cout << "Loading chatting room list.." << std::endl;
	
	// Sending request for room list in lobby.
	// Packet generation
	PK_CS_LOBBY_LOAD_ROOMLIST packet;
	
	// Packet set in PacketManager
	get_pContext().get_pPM()._setPacket(packet);
	get_pContext().get_pPM()._serialize();

	// Sending packet
	get_pContext()._sending();
	
	// Receiving packet
	get_pContext()._receiving();

	// Processing packet
	//	printing chatting room list.
	get_pContext().get_pPM()._deserialize();

	// 
	char input;
	char conLoopFlag = true;
	while(conLoopFlag) {
		std::cout << "Please put your command. ";
		std::cout << "(C: Create room, J: Join, R: Reload list, E: Exit)" << std::endl;
		std::cout << "> ";
		std::cin >> input;

		switch (input) {
		case 'c':
		case 'C':
			std::cout << "Select to create room." << std::endl;
			
			// change state to CreateRoom state
			get_pContext().changeState(CreateRoomState::Instance());

			conLoopFlag = false;
			break;
		case 'j':
		case 'J':
			std::cout << "Select to join room." << std::endl;
			
			//rev
			// choose room # to join process
			int rNum;	// room number
			while (1)	// valid input checking loop
			{
				std::cout << "Put the room number to join (0: Return to lobby): ";
				std::cin >> rNum;

				if (rNum >= 0) {
					break;
				}
				std::cout << "Wrong input. Please try again." << std::endl;
			}

			if (rNum == 0) {
				// change state to lobby.
				//	go back to lobby
				break;
			}
			else {
				// Processing packet
				//	
				//rev 
				// input value receiving 과정이 packet::_serialize()에 
				//	포함되어야할까..? 
				//	그게 아니라면 input을 어떻게 packet에 전해주나?


			}
			
			conLoopFlag = false;
			break;
		case 'r':
		case 'R':
			std::cout << "Select to reload room list." << std::endl;
			//rev
			// change state to this state (iterate)

			conLoopFlag = false;
			break;
		case 'e':
		case 'E':
			std::cout << "Select to exit program." << std::endl;
			//rev 
			// change state to Close state

			conLoopFlag = false;
			break;
		default:
			std::cout << "Wrong input. Try again." << std::endl;
			conLoopFlag = true;
			break;
		}

	}

	return false;	
	//rev 
	// InitState calss implementation.
	// CloseState class implementation.
}

LobbyState::LobbyState()
{
	// roomList initializing and reserve memory space.
	_roomList.reserve(CHATTERS::NUM_OF_ROOM_PER_PAGE);
}

CreateRoomState & CreateRoomState::Instance()
{
	static CreateRoomState _instance;
	return _instance;
}

CreateRoomState::CreateRoomState()
{
	//rev
}

ChatState & ChatState::Instance()
{
	static ChatState _instance;
	return _instance;
}

ChatState::ChatState()
{
	//rev
	// peerList 초기화
}

ClientState::ClientState(ConnectInfo conInfo, State * pState, PacketManager * pm) : _pPM(pm), _conInfo(conInfo), _pState(pState)
{
	// left blank intentionally
}

bool ClientState::request()
{
	return _pState->handle();
}

void ClientState::changeState(State & nextState)
{
	_pState = &nextState;
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

PacketManager & ClientState::get_pPM()
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
	LoginState::Instance();
	LobbyState::Instance();
	CreateRoomState::Instance();
	ChatState::Instance();

	//LoginState::Instance();
	//LobbyState::Instance();
	//CreateRoomState::Instance();
	//ChatState::Instance();

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

