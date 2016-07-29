#pragma once

#ifndef _HEADER_CLNT_H_
#define _HEADER_CLNT_H_

#include <WinSock2.h>
#include "header_common.h"
#include "packet.h"
#include "PT_CS_Data.h"
#include "PT_SC_Data.h"



/************************************************************************
 * class pre-declarations
 *
 ************************************************************************/
class ClientState;



/************************************************************************
 * ConnectInfo class
 *
 ************************************************************************/
class ConnectInfo
{
public:
	ConnectInfo();

	// accessor
	const WSADATA& get_wsaData() const;
	const SOCKET& get_sock() const;
	const sockaddr& get_servaddr() const;
	
	WSADATA& get_wsaData();
	SOCKET& get_sock();
	sockaddr& get_servaddr();
public:

private:
private:
	WSADATA _wsaData;
	SOCKET _sock;
	sockaddr _servAddr;
};

/************************************************************************
 * StateMachine class
	- abstract base class of all other 'state' class of state machine.
 ************************************************************************/
class State : public MachObject
{
public:
	virtual void run() = 0;
	
	// Accessor
	ClientState& get_pContext();

	// Mutator
	void set_pContext(ClientState& context);
public:

protected:
	State();
	State(ClientState * pClntState);

	virtual void init() = 0;	// State �ʱ�ȭ. �ʱⰪ, ������ ��.
protected:
	ClientState * _pContext;
	bool _exitFlag;	//rev constructor���� field �ʱ�ȭ���� ��.
};

/************************************************************************
 * InitState class
	- State loop ���� ���� TCP ���� �� �ڷᱸ�� �� �ʱ�ȭ�ϴ� state
 *
 ************************************************************************/
class InitState : public State
{
public:
	static InitState& Instance();

	virtual void init();
	virtual bool handle();
public:

protected:
	InitState();
	InitState& operator=(const InitState&);	// prohibit object copying
protected:
	static InitState _instance;
};

/************************************************************************
 * LoginState class
	- Login �� ���õ� ���μ����� ����ϴ� state
 *
 ************************************************************************/
class LoginState : public State
{
public:	
	static LoginState& Instance();

	virtual void init();
	virtual bool handle();
public:

protected:
	LoginState();
	LoginState& operator=(const LoginState&);	// prohibit object copying.
protected:
	static LoginState _instance;
	std::string _id;
	std::string _pw;
};

/************************************************************************
 * LobbyState class
	- 
 *
 ************************************************************************/
class LobbyState : public State
{
public:
	static LobbyState& Instance();
	virtual void init();
	virtual bool handle();	//rev
public:

protected:
	LobbyState();
	LobbyState& operator=(const LobbyState&);	// prohibit object copying.
protected:
	static LobbyState _instance;
	std::vector<RoomInfoToken> _roomList;
};

/************************************************************************
 * CreateRoomState class
	- 
 *
 ************************************************************************/
class CreateRoomState : public State
{
public:
	static CreateRoomState& Instance();
	virtual void init();
	virtual bool handle();
public:

protected:
	CreateRoomState();	//rev
protected:
	static CreateRoomState _instance;
};

/************************************************************************
 * //class name
 *
 ************************************************************************/
class ChatState : public State
{
public:
	static ChatState& Instance();
	virtual void init();
	virtual bool handle();
public:

protected:
	ChatState();	//rev
protected:
	static ChatState _instance;
	static std::vector<UserInfoToken> _peerList;

};

/************************************************************************
 * ExitState class
	- ���α׷� ���� �� TCP ���� ���� �� ���� wrap up process�� �����ϴ� state
 *
 ************************************************************************/
class ExitState :State
{
public:
	static ExitState& Instance();
public:

protected:
	ExitState();
	ExitState& operator=(const ExitState&);	// prohibit object copying.
protected:
	static ExitState _instance;
};

/************************************************************************
 * ClientState
	- context of state pattern
 ************************************************************************/
class ClientState : public MachObject
{
public:
	ClientState();
	ClientState(ConnectInfo conInfo, State * pState, PacketManager * pm);
	void init();		//rev
	void _sending();	//rev
	void _receiving();

	bool request();

	void changeState(State&);

	// accessor
	const ConnectInfo& get_conInfo() const;
	const UserInfoToken& get_myInfo() const;
	
	ConnectInfo& get_conInfo();
	UserInfoToken& get_myInfo();
	PacketManager& get_pPM();
	// mutator
	void set_conInfo(ConnectInfo);
	void set_myInfo(UserInfoToken);
	void set_pPM(PacketManager *);
public:
	
protected:
protected:
	ConnectInfo _conInfo;	// tcp connection related inform structure
	UserInfoToken _myInfo;	// client's inform
	State * _pState;	// state context
	PacketManager * _pPM;
private:
	friend State;
};



/************************************************************************
 * etc functions declarations
 * 
 ************************************************************************/

void init();	//rev
void close();
std::string& stringCheck(std::string& str);



#endif // !_HEADER_CLNT_H_