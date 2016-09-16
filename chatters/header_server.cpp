#include "header_server.h"
#include "header_common.h"
#include "DBConnector.h"


/*********************************************************************
 * PerIoData class implementation
*********************************************************************/
PerIoData::PerIoData() : _buffer(nullptr), _bufferLen(0), _refCount(0)
{
	wsaBuf.len = _bufferLen;
	wsaBuf.buf = _buffer;
}
PerIoData::PerIoData(size_t bufSz) : PerIoData()
{
	allocBuffer(bufSz);
}
PerIoData::~PerIoData()
{
	_releaseBuffer();
	//std::cout << "~PER_IO_DATA()" << endl;
}
int PerIoData::get_refCount() const
{
	return _refCount;
}
void PerIoData::inc_refCount()
{
	_refCount++;
}
void PerIoData::allocBuffer(size_t bufSz)
{
	_releaseBuffer();
	_buffer = new char[bufSz];
	_bufferLen = bufSz;
	wsaBuf.len = bufSz;
	wsaBuf.buf = _buffer;
}
void PerIoData::set_Buffer(char * bufPtr, int bufSz)
{
	if (_buffer != nullptr)
		delete[] _buffer;

	_buffer = bufPtr;
	_bufferLen = bufSz;
	wsaBuf.len = bufSz;
	wsaBuf.buf = _buffer;
}
char * PerIoData::get_buffer() const
{
	return _buffer;
}
size_t PerIoData::get_bufferLen() const
{
	return _bufferLen;
}
void PerIoData::operator delete(void * p)
{
	auto targetPtr = static_cast<LPPER_IO_DATA>(p);
	if (targetPtr->_refCount <= 1)
	{
		//std::cout << "delete PER_IO_DATA(addr: " << p << ") called. Object deleted." << endl;
		free(p);
		return;
	}
	targetPtr->_refCount--;
	//std::cout << "delete PER_IO_DATA(addr: " << p << ") called. refCount: " << targetPtr->_refCount << endl;
}
void PerIoData::set_refCount(int newVal)
{
	_refCount = newVal;
}
void PerIoData::dec_refCount()
{
	_refCount--;
}
void PerIoData::_releaseBuffer()
{
	if (_buffer != nullptr)
		delete[] _buffer;

	_bufferLen = 0;
	wsaBuf.len = 0;
	wsaBuf.buf = nullptr;
}



/*********************************************************************
 * SvConInfo class definition
*********************************************************************/
SvConInfo::SvConInfo()
{
}
SvConInfo::SvConInfo(WSADATA newWsaData, SOCKET newSock, sockaddr newServAddr)
{
	wsaData = newWsaData;
	sock = newSock;
	servAddr = newServAddr;
}



/*********************************************************************
 * SvUserInfo class definition
*********************************************************************/
SvUserInfo::SvUserInfo(const std::string & id, SOCKET sock) 
	: utk(id), 
	curRmNum(CHATTERS::NO_ROOM),
	_socket(sock)
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
		_socket = ui._socket;
	}

	return *this;
}
SvUserInfo & SvUserInfo::operator=(const SvUserInfo & ui)
{
	if (this != &ui)
	{
		utk = ui.utk;
		curRmNum = ui.curRmNum;
		_socket = ui._socket;
	}

	return *this;
}
SOCKET SvUserInfo::get_socket() const
{
	return _socket;
}
void SvUserInfo::set_socket(SOCKET socket)
{
	_socket = socket;
}



/*********************************************************************
 * SvRoomInfo definition
*********************************************************************/
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



/*********************************************************************
 * SvMach class definitions
*********************************************************************/
SvMach::SvMach() : _dbc()
{
	if (_dbc.connect() != SQL_SUCCESS)
	{
		cout << "Database object creation failed." << endl;
		exit(1);
	}
}
SvMach::~SvMach()
{
	_dbc.close();
	_uList.clear();
	_rList.clear();
}
bool SvMach::db_signin(const std::string & id, const std::string & pw)
{
	// SELECT * FROM tb1 WHERE id='str_id' AND pw='str_pw'
	std::string stmt = "SELECT * FROM tb1 ";
	stmt += "WHERE id='" + id + "' AND pw='" + pw + "'";

	RETCODE errmsg;

	if ((errmsg = _dbc.excute(stmt)) != SQL_SUCCESS)
	{	// excute() failed
		std::cout << "DBConnector::excute() got SQL error." << std::endl;
		exit(1);
	}

	int foundNumOfRow = 0;
	if ((errmsg = _dbc.getResultNum(foundNumOfRow)) != SQL_SUCCESS)
	{	// getResultNum(..) failed
		std::cout << "DBConnector::getResultNum() got SQL error." << std::endl;
		exit(1);
	}

	if (foundNumOfRow == 1)
		return true;
	else
		return false;
}
bool SvMach::db_signup(const std::string & id, const std::string & pw)
{
	RETCODE errmsg;
	int foundRow;

	// check the primary key(id) available
	std::string stmt = "SELECT * FROM tb1 WHERE id='" + id + "'";
	if (errmsg = _dbc.excute(stmt) != SQL_SUCCESS)
	{
		std::cout << "DBConnector::excute(..) got SQL error." << std::endl;
		exit(1);
	}

	foundRow = 0;
	if (_dbc.getResultNum(foundRow) != SQL_SUCCESS)
	{
		std::cout << "DBConnector::getResultNum(..) get SQL error" << std::endl;
		exit(1);
	}

	if (foundRow > 0)	// primary key duplicated.
	{
		std::cout << "SvMach::db_signup(..) error: duplicated key" << std::endl;
		return false;
	}

	// INSERT INTO tb1 VALUES ('str_id', 'str_pw')
	std::string stmt = "INSERT INTO tb1 VALUES ('" + id + "', '" + pw + "')";

	if (errmsg = _dbc.excute(stmt) != SQL_SUCCESS)
	{
		std::cout << "DBConnector::excute(..) get SQL error" << std::endl;
		exit(1);
	}
	return true;
}
UserKey SvMach::addUser(const std::string& id, SOCKET sock)
{
	SvUserInfo user(id, sock);
	
	auto resPair = _uList.insert(std::make_pair(user.utk.get_key(), std::move(user))); //rev	//? move() 사용하나? // auto? auto&?
	bool opResult = resPair.second;
	UserKey uKey = resPair.first->first;

	if (opResult)	
	{	// add new user succesfully
		return uKey;
	}
	else
	{	// add new user failed
		return InfoToken::INVALID_KEY;
	}
}
bool SvMach::removeUser(UserKey uKey)
{
	size_t result = _uList.erase(uKey);

	if (result == 0)
		return false;

	return true;
}
bool SvMach::joinRoom(RoomKey rKey, UserKey uKey)
{
	auto rmIter = findRoom(rKey);

	if (rmIter == _rList.end()) {
		std::cout << "Cannot find Room #" << rKey << "(Request from user #" << uKey << ")" << endl;
		return false;
	}

	bool joinRoomResult = rmIter->second.addUser(uKey);
	bool updateUserInfoResult;
	
	if (!joinRoomResult) {// fail to join room
		std::cout << "Fail to join room #" << rKey << "(Request from user #" << uKey << ")" << endl;
		return false;
	}
	else				// succeed to join room
		updateUserInfoResult = updateUserInfo(uKey, rKey);	// update user informaion(room#)
		
	if (!updateUserInfoResult)	// fail to update user information(room#)
	{
		std::cout << "Fail to update user information of user #" << uKey << ")" << endl;
		rmIter->second.removeUser(uKey);// rewind changes
		return false;
	}
	else
		return true;
}
bool SvMach::leaveRoom(RoomKey rKey, UserKey uKey)
{
	auto rmIter = findRoom(rKey);

	if (rmIter == _rList.end()) {
		std::cout << "Cannot find Room(#" << rKey << ")" << endl;
		return false;
	}
	
	bool leaveRoomResult = rmIter->second.removeUser(uKey);
	bool updateUserInfoResult;

	if (!leaveRoomResult)	// fail to leave room
		return false;
	else					// succeed to leave room
		updateUserInfoResult = updateUserInfo(uKey, CHATTERS::NO_ROOM);	// update user informaion(room#)
		
	if (!updateUserInfoResult)	// fail to update user information(room#)
	{
		rmIter->second.addUser(uKey);// rewind changes
		return false;
	}
	else
		return true;
}
RoomKey SvMach::openRoom(UserKey uKey, const std::string & title)
{
	SvRoomInfo rm(title);
	RoomKey rmKey = rm.rtk.get_key();

	auto resPair = _rList.insert(std::make_pair(rm.rtk.get_key(), std::move(rm)));
	bool opResult = resPair.second;
	RoomKey rmKey = resPair.first->first;

	if (!opResult)	// fail to add room to room list
	{
		std::cout << "Create room failed." << std::endl;
		return InfoToken::INVALID_KEY;
	}

	bool joinResult = joinRoom(rmKey, uKey);

	if (!joinResult)	// fail to enter the room
	{
		_rList.erase(rmKey);	// rewind changes
		return InfoToken::INVALID_KEY;
	}
	
	return rmKey;
}
//std::vector<UserKey> SvMach::closeRoom(RoomKey rKey)
//{
//	auto it = _rList.find(rKey);
//	auto target = _rList.erase(it);
//
//	//target	// check
//	std::vector<UserKey> rtnVec;
//	std::swap(it->second.userList, rtnVec);
//
//	return rtnVec;
//}
bool SvMach::removeRoom(RoomKey rKey)
{
	auto removed = _rList.erase(rKey);
	if (removed == 0) {
		return false;
	}

	return true;
}
bool SvMach::updateUserInfo(UserKey uKey, RoomKey rmKey)
{
	auto uiIter = findUser(uKey);

	if (uiIter == _uList.end())	// fail to update user information operation
	{
		std::cout << "Cannot find user(user#" << uKey << ")" << std::endl;
		return false;
	}

	uiIter->second.curRmNum = rmKey;
	return true;
}
std::unordered_map<UserKey, SvUserInfo>::const_iterator SvMach::findUser(UserKey uKey) const
{
	return _uList.find(uKey);
}
std::unordered_map<RoomKey, SvRoomInfo>::const_iterator SvMach::findRoom(RoomKey rKey) const
{
	return _rList.find(rKey);
}
std::unordered_map<UserKey, SvUserInfo>::iterator SvMach::findUser(UserKey uKey)
{
	return _uList.find(uKey);
}
std::unordered_map<RoomKey, SvRoomInfo>::iterator SvMach::findRoom(RoomKey rKey)
{
	return _rList.find(rKey);
}
const std::unordered_map<UserKey, SvUserInfo>& SvMach::get_userList() const
{
	return _uList;
}
const std::unordered_map<RoomKey, SvRoomInfo>& SvMach::get_roomList() const
{
	return _rList;
}



/*********************************************************************
 * SvPacketManager class implementation

*********************************************************************/
SvPacketManager & SvPacketManager::Instance()
{
	static SvPacketManager _instance;

	return _instance;
}
void SvPacketManager::sendPacket(std::shared_ptr<Packet_Base> spPk)
{
	SOCKET hClntSock;
	size_t pkSz;
	DWORD flags = 0;
	LPPER_IO_DATA ioInfo;

	hClntSock = spPk->sock;
	pkSz = spPk->get_packetSize();

	ioInfo = new PerIoData(pkSz);
	memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
	memcpy_s(ioInfo->get_buffer(), ioInfo->get_bufferLen(), spPk->get_bufAddr(), pkSz);
	ioInfo->rwMode = PerIoData::WRITE;

	WSASend(hClntSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
}
std::shared_ptr<Packet_Base> SvPacketManager::recvPacket()
{
	auto& pm = SvPacketManager::Instance();

	while (pm._msgQueue.empty())	// queue에 packet이 없으면 대기
		;

	//rev lock..? pop 한줄에만 걸어도 될 것 같은데?
	auto shPk = pm._msgQueue.front();
	pm._msgQueue.pop();

	return std::move(shPk);	
}



/*********************************************************************
 * etc. functions definitions
*********************************************************************/
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
				cout << "|READ_PACKET| received. >>" << ioInfo->get_buffer() << endl;
				auto shPk = extractCSPacket(ioInfo->get_buffer(), ioInfo->get_bufferLen());
				if (shPk == nullptr)
					cout << "|READ_PACKET| Packet extraction failed." << endl;
				else {
					shPk->sock = sock;
					SvPacketManager::Instance()._msgQueue.push(std::move(shPk));	//rev lock 걸어야할까..?

					// ioInfo reset to wait for header
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					ioInfo->allocBuffer(Packet_Base::HEADER_SIZE);
					ioInfo->rwMode = PerIoData::READ_HEADER;

					// wsarecv call
					WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
				}
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
	auto& pm = SvPacketManager::Instance();

	while (1) {
		auto shRecvPk = pm.recvPacket();

		shRecvPk->deserialize();
		auto shSendPk = shRecvPk->processPacket(pm.getAgent());

		if (shSendPk != nullptr)
		{
			// packet sending process
			shSendPk->serialize();
			pm.sendPacket(shSendPk);
		}
	}

	return 0;
}

//// test code	//rev
//DWORD WINAPI packetProcessWorkerThreadMain2(LPVOID pComPort)
//{
//	auto& pm = SvPacketManager::Instance();
//
//	while (1) {
//		auto shRecvPk = pm.recvPacket();
//
//		if (shRecvPk != nullptr) {
//			//std::cout << "shPtr::use_count(): " << shRecvPk.use_count() << std::endl;
//			std::cout << "Packet(_id: " << static_cast<int>(shRecvPk->_id) << ")::_buf: " << shRecvPk->get_buf().str() << std::endl;
//		}
//		else
//			std::cout << "Packet::nullptr" << endl;
//	}
//
//	return 0;
//}

void ErrorHandling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



//rev
// 변경 사항
// 9. singleton 보다 확실하게 지켜지도록 생성자 제한

//rev
// completed
// 1. completion key in svmain create~ 를 hClntSock으로 바꿈
// 2. complete recv/incomplete recv 주석 달기
// 3. |READ_PACKET| complete recv 시 메시지 출력
// 4. extractPacket..(..) 시 nullptr 출력될 시 처리
// 5. extractPacket..(..) 내용 수정
// 6. packetProcessWorkerThreadMain(..)을 SvPacketManager에 friend 추가
// 7. shPk 넣는 과정에서std::move 사용으로 의미 명확히 하기 2군데
// 8. auto pm -> auto& pm으로 바꾸기.
// 10. extractPacket..(..) arg로 버퍼 길이를 같이 받도록 수정
// 11. Packet_Base(..) ctor 가 buf를 arg로 받을 때 bufLen도 같이 받도록 수정