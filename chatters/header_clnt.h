#pragma once

#ifndef _HEADER_CLNT_H_
#define _HEADER_CLNT_H_

#include <WinSock2.h>
#include "header_common.h"
#include "packet.h"
#include "PT_CS_Data.h"
#include "PT_SC_Data.h"



/***********************************************************
* class pre-declarations
*
***********************************************************/
class ClientState;



/***********************************************************
 * client-related class declarations
 *
 ***********************************************************/

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
	// mutator
	void set_wsaData(WSADATA wsaData);	//rev
	void set_sock(SOCKET sock);
	void set_servaddr(sockaddr sockAddr);
public:

private:
private:
	WSADATA _wsaData;
	SOCKET _sock;
	sockaddr _servAddr;
};

class StateMachine : public MachObject
{
public:
	virtual void init() = 0;	// State 초기화. 초기값, 데이터 등.
	virtual bool handle() = 0;
public:

protected:
	StateMachine();
	StateMachine(ClientState * pClntState);
	void changeState(ClientState *, StateMachine *);
protected:
	ClientState * const _pContext;
};

class LoginState : public StateMachine
{
public:
	LoginState(ClientState *);
	LoginState(LoginState&&);

	LoginState& operator=(LoginState&&);
	
	virtual void init();
	virtual bool handle();
public:

protected:
	LoginState();
	LoginState(const LoginState&);

	LoginState& operator=(const LoginState&);	// object copy is prohibited
protected:
	std::string _id;
	std::string _pw;
};

class LobbyState : public StateMachine
{
public:
	virtual void init();
	virtual bool handle();
public:

protected:
protected:
	std::vector<RoomInfoToken> _roomList;
};

class CreateRoomState : public StateMachine
{
public:
	virtual void init();
	virtual bool handle();


public:

protected:
protected:

};

class ChatState : public StateMachine
{
public:
	virtual void init();
	virtual bool handle();
public:

protected:
protected:
	static std::vector<UserInfoToken> _peerList;

};

// context of state pattern
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
	PacketManager& get_pPm();
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



/***********************************************************
 * etc functions declarations
 * 
 ***********************************************************/

void init();
void close();
std::string& stringCheck(std::string& str);



#endif // !_HEADER_CLNT_H_