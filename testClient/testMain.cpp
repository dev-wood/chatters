#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <winsock2.h>

#include <iostream>
#include <string>
#include <future>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;

#define BUF_SIZE 1024

enum struct PTYPE : int
{
	// Packet_Base default packet
	PT_BASE = 0,



	// Server to Client packet
	PT_SC_LOGIN_ACCEPT,
	PT_SC_LOGIN_FAIL,

	PT_SC_LOBBY_JOINROOMOK,
	PT_SC_LOBBY_JOINROOMFAIL,
	PT_SC_LOBBY_LOAD_ROOMLIST,

	PT_SC_CREATEROOM_OK,
	PT_SC_CREATEROOM_FAIL,

	PT_SC_CHAT_QUITUSER,
	PT_SC_CHAT_CHAT,
	PT_SC_CHAT_LOAD_USERLIST,
	// 추후 확장 가능한 기능 
	PT_SC_CHAT_GUESTSTATEREADY,
	PT_SC_CHAT_GUESTSTATENOTREADY,
	PT_SC_CHAT_ROOMSTATEPLAYING,
	PT_SC_CHAT_ROOMSTATENOTPLAYING,



	// Client to Server packet
	PT_CS_LOGIN_REQUEST,

	PT_CS_LOBBY_JOINROOM,
	PT_CS_LOBBY_LOAD_ROOMLIST,

	PT_CS_CREATEROOM_CREATEROOM,

	PT_CS_CHAT_QUITROOM,
	PT_CS_CHAT_CHAT,
	// 추후 확장 가능한 기능
	PT_CS_CHAT_MYSTATEREADY,
	PT_CS_CHAT_MYSTATENOTREADY,
	PT_CS_CHAT_ROOMSTATEPLAYING,
	PT_CS_CHAT_ROOMSTATENOTPLAYING
};

void ErrorHandling(char * message);
void sendRoutine(SOCKET sock, char * buf);
void recvRoutine(SOCKET sock, char * buf);
int buildPacket(std::stringstream & strbuf, PTYPE ptype, const std::string & contents);

int main(int argc, char * argv[])
{
	// variables declaration
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	char message[BUF_SIZE];

	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
		exit(1);
	}

	// set WSADATA 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	// get socket handle
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	// setting SOCKADDR_IN
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	// try connect to server
	if (connect(hSocket, (SOCKADDR *)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");
	else
		cout << "Connected.........." << endl;

	// chatting & echoing routine start
	std::future<void> recvThread(std::async(std::launch::async, recvRoutine, hSocket, message));
	std::future<void> sendThread(std::async(std::launch::async, sendRoutine, hSocket, message));

	sendThread.get();
	recvThread.get();

	//// close socket	//rev
	closesocket(hSocket);
	// wrap up WSADATA
	WSACleanup();

	return 0;
}

void sendRoutine(SOCKET sock, char * buf)
{
	while (1) {
		int strLen = 0;
		std::string msg;
		std::stringstream strbuf;

		cout << "Input message(Q to quit): ";
		std::getline(cin, msg);

		if (msg == "q" || msg == "Q")
			break;

		int pkLen = buildPacket(strbuf, PTYPE::PT_CS_CHAT_CHAT, msg);

		cout << "Processed message: " << strbuf.str() << endl;
		send(sock, strbuf.str().c_str(), pkLen, 0);
	}
	cout << "closing socket.." << endl;
	closesocket(sock);	//rev 
	cout << "socket closed" << endl;
	cout << "sendThread released" << endl;
}

void recvRoutine(SOCKET sock, char * buf)
{
	while (1)
	{
		int headerLen, expectedLen;
		int recvLen = 0;
		char message[BUF_SIZE];

		headerLen = sizeof(size_t);


		recvLen = recv(sock, (char *)&expectedLen, headerLen, 0);
		if (recvLen == -1) {
			cout << "recvLen is -1. " << endl;
			break;
		}

		recvLen = recv(sock, message, expectedLen, 0);
		if (recvLen == -1) {
			cout << "recvLen is -1. " << endl;
			break;
		}

		message[expectedLen] = NULL;

		cout << "\n\tPacket content: " << message << endl;
		cout << "Input message(Q to quit): ";
	}
	cout << "recvThread released" << endl;
}

int buildPacket(std::stringstream & strbuf, PTYPE ptype, const std::string & contents)
{
	int pkLen, msgLen;	// packet 전체 크기, msg만의 길이

	int headerLen = sizeof(size_t);

	std::stringstream tempbuf;
	tempbuf << static_cast<int>(ptype) << '|' << contents;

	msgLen = tempbuf.str().size();
	pkLen = headerLen + msgLen;

	strbuf.write((const char *)(&msgLen), headerLen);
	strbuf << tempbuf.str();
	cout << "Processed message: " << strbuf.str() << endl;

	return pkLen;
}

void ErrorHandling(char * message)
{
	cout << message << endl;
	exit(1);
}