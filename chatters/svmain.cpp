#include "../chatters/header_server.h"


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int recvBytes, i, flags = 0;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);
	
	// system thread 중에서 여분 1개와 packet 처리용 worker thread 1개를 제외한 나머지를 IOCP thread로 할당
	for (i = 0; i < (int)(sysInfo.dwNumberOfProcessors - 2); i++)
		_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))recvThreadMain, (LPVOID)hComPort, 0, NULL);
	_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))packetProcessWorkerThreadMain, (LPVOID)hComPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR *)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	cout << "server start successfully.." << endl;

	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR *)&clntAdr, &addrLen);
		handleInfo = new(PER_HANDLE_DATA);
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)hClntSock, 0);

		ioInfo = new PER_IO_DATA(Packet_Base::HEADER_SIZE);
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->rwMode = PerIoData::READ_HEADER;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
	}
	return 0;
}
