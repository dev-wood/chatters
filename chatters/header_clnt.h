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
class StateMachine : public MachObject
{
public:
	virtual void init() = 0;	// State 초기화. 초기값, 데이터 등.
	virtual bool handle() = 0;
	void set_pContext(ClientState& context);
public:

protected:
	StateMachine();
	StateMachine(ClientState * pClntState);
	void changeState(ClientState *, StateMachine *);
protected:
	ClientState * _pContext;
};

/************************************************************************
 * LoginState class
	- 
 *
 ************************************************************************/
class LoginState : public StateMachine
{
public:	
	static LoginState& Instance();

	virtual void init();
	virtual bool handle();
public:

protected:
	LoginState();

	LoginState& operator=(const LoginState&);	// object copy is prohibited
protected:
	static LoginState _instance;
	std::string _id;
	std::string _pw;
};

/************************************************************************
 * //class name
 *
 ************************************************************************/
class LobbyState : public StateMachine
{
public:
	static LobbyState& Instance();
	virtual void init();
	virtual bool handle();
public:

protected:
	LobbyState();	//rev
protected:
	static LobbyState _instance;
	std::vector<RoomInfoToken> _roomList;
};

/************************************************************************
 * //class name
 *
 ************************************************************************/
class CreateRoomState : public StateMachine
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
class ChatState : public StateMachine
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

// context of state pattern
/************************************************************************
 * //class name
 *
 ************************************************************************/
class ClientState : public MachObject
{
public:
	ClientState();
	ClientState(ConnectInfo conInfo, StateMachine * pState, PacketManager * pm);
	void init();		//rev
	void _sending();	//rev
	void _receiving();

	bool request();

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
	StateMachine * _pState;	// state context
	PacketManager * _pPM;
private:
	friend StateMachine;
};



/************************************************************************
 * etc functions declarations
 * 
 ************************************************************************/

void init();	//rev
void close();
std::string& stringCheck(std::string& str);



#endif // !_HEADER_CLNT_H_