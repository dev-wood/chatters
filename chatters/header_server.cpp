#include "header_server.h"

/* SvConInfo class definition */
SvConInfo::SvConInfo()
{
}
SvConInfo::SvConInfo(WSADATA newWsaData, SOCKET newSock, sockaddr newServAddr)
{
	wsaData = newWsaData;
	sock = newSock;
	servAddr = newServAddr;
}

/* SvUserInfo class definition */
SvUserInfo::SvUserInfo(const std::string & id, std::shared_ptr<HandleData> hData) 
	: utk(id), 
	curRmNum(CHATTERS::NO_ROOM),
	spHData(hData)
{
	// left blank intentionally
}
SvUserInfo::SvUserInfo(const SvUserInfo & ui)
{
	*this = ui;
}
SvUserInfo::SvUserInfo(SvUserInfo && ui)
{
	*this = std::move(ui);
}
SvUserInfo & SvUserInfo::operator=(SvUserInfo && ui)
{
	if (this != &ui)
	{
		utk = std::move(ui.utk);
		curRmNum = ui.curRmNum;
		spHData = ui.spHData;
	}

	return *this;
}
SvUserInfo & SvUserInfo::operator=(const SvUserInfo & ui)
{
	if (this != &ui)
	{
		utk = ui.utk;
		curRmNum = ui.curRmNum;
		spHData = ui.spHData;
	}

	return *this;
}

/* SvRoomInfo definition */
SvRoomInfo::SvRoomInfo(const std::string & title) : rtk(title)
{
	userList.reserve(CHATTERS::MAX_PARTICIPANT);
}
bool SvRoomInfo::addUser(UserKey uKey)
{
	if (userList.size() < CHATTERS::MAX_PARTICIPANT) {
		userList.push_back(uKey);
		return true;
	}
	return false;
}
bool SvRoomInfo::removeUser(UserKey uKey)
{
	for (auto & it = userList.begin(); it != userList.cend(); ++it)
	{
		if (*it == uKey) {
			userList.erase(it);
			return true;
		}
	}

	return false;
}
SvRoomInfo::SvRoomInfo()
{
	std::cout << "SvRoomInfo() called." << std::endl;
}

/* etc. functions definitions */
DWORD WINAPI recvThreadMain(LPVOID pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&sock, (LPOVERLAPPED *)&ioInfo, INFINITE);
		cout << "GetQueuedComplitionStatus(mode: " << ioInfo->rwMode << ") bytesTrans = " << bytesTrans << endl;

		if (ioInfo->rwMode == PerIoData::READ_HEADER)
		{
			cout << "|READ_HEADER| sock: " << sock << endl;
			if (bytesTrans == 0)	// EOF 전송시
			{
				std::cout << "|READ_HEADER| closing socket(bytesTrans == 0) socket: " << sock << std::endl;
				closesocket(sock);
				delete(ioInfo);
				continue;
			}

			// 받은 바이트수 체크
			ioInfo->wsaBuf.len -= bytesTrans;
			ioInfo->wsaBuf.buf += bytesTrans;
			if (ioInfo->wsaBuf.len != 0)
			{
				cout << "|READ_HEADER| fragmented packet. WSARecv() called again." << endl;
				WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
			}
			else {
				// ioInfo reset to wait for packet
				size_t pkSize = static_cast<int>(*(ioInfo->get_buffer()));
				cout << "|READ_HEADER| Packet size: " << pkSize << ". rwMode changed to READ_PACKET." << endl;
				memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
				ioInfo->allocBuffer(pkSize);
				ioInfo->rwMode = PerIoData::READ_PACKET;

				// wsarecv call
				WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
			}
		}
		else if (ioInfo->rwMode == PerIoData::READ_PACKET)
		{
			cout << "|READ_PACKET| sock: " << sock << endl;

			if (bytesTrans == 0)
			{
				std::cout << "|READ_PACKET| closing socket(bytesTrans == 0)" << std::endl;
				closesocket(sock);
				delete(ioInfo);
				continue;
			}

			// 받은 바이트수 체크
			ioInfo->wsaBuf.len -= bytesTrans;
			ioInfo->wsaBuf.buf += bytesTrans;
			if (ioInfo->wsaBuf.len != 0)
			{	// incomplete receive
				cout << "|READ_PACKET| fragmented packet. WSARecv() called again." << endl;
				WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
			}
			else {	// complete receive
				auto shPk = extractPacketFromBuffer(ioInfo->get_buffer);
				shPk->sock = sock;
				
				SvPacketManager::Instance()._msgQueue.push(std::move(shPk));	//rev lock 걸어야할까..?
			}
		}
		else
		{
			puts("|WRITE| message sent");
			delete(ioInfo);
		}
	}
	return 0;
}

DWORD WINAPI packetProcessWorkerThreadMain(LPVOID pComPort)
{
	auto pm = SvPacketManager::Instance();
	
	auto shRecvPk = pm.recvPacket();

	shRecvPk->deserialize();
	auto shSendPk = shRecvPk->processPacket(pm.getAgent());

	if (shSendPk != nullptr)
	{
		// packet sending process
		shSendPk->serialize();
		pm.sendPacket(shSendPk);
	}
	
	return 0;
}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}