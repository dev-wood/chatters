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
void PerIoData::set_refCount(int newVal)
{
	_refCount = newVal;
}
int PerIoData::get_refCount() const
{
	return _refCount;
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
	// left blank intentionally9
}
SvUserInfo::SvUserInfo(const SvUserInfo & ui)
{
	*this = ui;
}
SvUserInfo::SvUserInfo(SvUserInfo && ui)
{
	*this = std::move(ui);
}
SvUserInfo::~SvUserInfo()
{
	// left blank intentionally
}
SvUserInfo & SvUserInfo::operator=(SvUserInfo && ui)
{
	if (this != &ui)
	{
		utk = std::move(ui.utk);
		curRmNum = ui.curRmNum;
		_socket = ui._socket;

		ui.curRmNum = RoomInfoToken::INVALID_KEY;
		ui._socket = INVALID_SOCKET;
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



SvUserInfoManager::SvUserInfoManager()
{
	// left blank intentionally
}
SvUserInfoManager::~SvUserInfoManager()
{
	clear();
}
std::shared_ptr<SvUserInfo> SvUserInfoManager::add(std::string id, SOCKET sock)
{
	auto shpUInfo = std::make_shared<SvUserInfo>(id, sock);
	_userList.insert(std::make_pair(shpUInfo->utk.get_key(), shpUInfo));

	return shpUInfo;
}
std::shared_ptr<SvUserInfo> SvUserInfoManager::remove(UserKey uKey)
{
	auto it = _userList.find(uKey);

	if (it == _userList.end())
	{
		return std::shared_ptr<SvUserInfo>();
	}
	else {
		auto shpUser = it->second;
		_userList.erase(uKey);
		
		return shpUser;
	}
}
std::shared_ptr<SvUserInfo> SvUserInfoManager::find(UserKey uKey)
{
	auto & it = _userList.find(uKey);

	if (it == _userList.end())
		return std::shared_ptr<SvUserInfo>();
	else
		return it->second;
}
const std::unordered_map<UserKey, std::shared_ptr<SvUserInfo>> & SvUserInfoManager::get() const
{
	return _userList;
}
void SvUserInfoManager::clear()
{
	_userList.clear();
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
		rtk.set_numOfPeer(userList.size());
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
			rtk.set_numOfPeer(userList.size());
			return true;
		}
	}

	return false;
}
SvRoomInfo::SvRoomInfo()
{
	// left blank intentionally
}



SvRoomInfoManager::SvRoomInfoManager()
{
	// left blank intentionally
}

SvRoomInfoManager::~SvRoomInfoManager()
{
	clear();
}
std::shared_ptr<SvRoomInfo> SvRoomInfoManager::add(std::string title)
{
	auto shpRmInfo = std::make_shared<SvRoomInfo>(title);
	_roomList.insert(std::make_pair(shpRmInfo->rtk.get_key(), shpRmInfo));

	return shpRmInfo;
}
std::shared_ptr<SvRoomInfo> SvRoomInfoManager::remove(RoomKey rmKey)
{
	auto & it = _roomList.find(rmKey);

	if (it == _roomList.end())
		return std::shared_ptr<SvRoomInfo>();
	else {
		auto shpRoom = it->second;
		_roomList.erase(rmKey);
		
		return shpRoom;
	}
}
std::shared_ptr<SvRoomInfo> SvRoomInfoManager::find(RoomKey rmKey)
{
	auto & it = _roomList.find(rmKey);

	if (it == _roomList.end())
		return std::shared_ptr<SvRoomInfo>();
	else {
		return it->second;
	}
}
const std::unordered_map<RoomKey, std::shared_ptr<SvRoomInfo>>& SvRoomInfoManager::get() const
{
	return _roomList;
}
void SvRoomInfoManager::clear()
{
	_roomList.clear();
}



SvSocketManager::SvSocketManager()
{
	// left blank intentionally
}
SvSocketManager::~SvSocketManager()
{
	clear();
}
bool SvSocketManager::entry(SOCKET sock, UserKey uKey)
{
	_socketList.insert(std::make_pair(sock, uKey));

	return true;
}
UserKey SvSocketManager::remove(SOCKET sock)
{
	auto & it = _socketList.find(sock);

	if (it == _socketList.end())
		return InfoToken::INVALID_KEY;
	else {
		UserKey rtnKey = it->second;

		_socketList.erase(sock);
		closesocket(sock);

		return rtnKey;
	}
}
UserKey SvSocketManager::find(SOCKET sock)
{
	auto & it = _socketList.find(sock);

	if (it == _socketList.end())
		return InfoToken::INVALID_KEY;
	else {
		return it->second;
	}
}
void SvSocketManager::clear()
{
	for (auto & pair : _socketList)
		closesocket(pair.first);
	
	_socketList.clear();
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
		//exit(1);
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
	std::string stmt;

	// check the primary key(id) available
	stmt = "SELECT * FROM tb1 WHERE id='" + id + "'";
	if (errmsg = _dbc.excute(stmt) != SQL_SUCCESS)
	{
		std::cout << "DBConnector::excute(..) got SQL error." << std::endl;
		exit(1);
	}

	foundRow = 0;
	if (_dbc.getResultNum(foundRow) != SQL_SUCCESS)
	{
		std::cout << "DBConnector::getResultNum(..) get SQL error" << std::endl;
		//exit(1);
	}

	if (foundRow > 0)	// primary key duplicated.
	{
		std::cout << "SvMach::db_signup(..) error: duplicated key" << std::endl;
		return false;
	}

	// INSERT INTO tb1 VALUES ('str_id', 'str_pw')
	stmt = "INSERT INTO tb1 VALUES ('" + id + "', '" + pw + "')";

	if (errmsg = _dbc.excute(stmt) != SQL_SUCCESS)
	{
		std::cout << "DBConnector::excute(..) get SQL error" << std::endl;
		exit(1);
	}
	return true;
}
UserKey SvMach::addUser(const std::string& id, SOCKET sock)
{
	auto res = _users.add(id, sock);

	if (res == nullptr)
	{
		return InfoToken::INVALID_KEY;
	}
	else {
		return res->utk.get_key();
	}
}
bool SvMach::removeUser(UserKey uKey)
{
	// remove from user list
	auto shpUser = _users.remove(uKey);
	
	if (shpUser != nullptr)
	{
		// close user's socket
		_sockets.remove(shpUser->get_socket());

		// update room info (if user has one it participate)
		if (shpUser->curRmNum != CHATTERS::NO_ROOM)
		{
			auto shpRm = _rooms.find(shpUser->curRmNum);
			if (shpRm != nullptr)
				shpRm->removeUser(uKey);
		}
		return true;
	}
	return false;
}
bool SvMach::removeUser(SOCKET sock)
{
	// remove socket from socket list
	auto uKey = _sockets.remove(sock);

	if (uKey != InfoToken::INVALID_KEY)
	{
		// remove user from user list
		auto shpUser = _users.remove(uKey);
		
		if (shpUser != nullptr)
		{
			// update room info (if user has one it participate)
			if (shpUser->curRmNum != CHATTERS::NO_ROOM)
			{
				auto shpRm = _rooms.find(shpUser->curRmNum);

				if (shpRm != nullptr) {
					shpRm->removeUser(uKey);
					if (shpRm->rtk.get_numOfPeer() == 0)
						_rooms.remove(shpRm->rtk.get_key());
				}
			}
			return true;
		}
	}	
	return false;
}
bool SvMach::joinRoom(RoomKey rmKey, UserKey uKey)
{
	auto shpRm = _rooms.find(rmKey);
	auto shpUser = _users.find(uKey);

	if ((shpRm != nullptr) && (shpUser != nullptr))
	{
		if (shpUser->curRmNum == CHATTERS::NO_ROOM) {
			// room information update
			bool opResult = shpRm->addUser(uKey);

			if (opResult) {
				// user information update
				shpUser->curRmNum = rmKey;
				return true;
			}
			else {
				cout << "Error: Failed to add user to the user list of the room." << endl;
				shpRm->removeUser(uKey);
				return false;
			}
		}
		else
		{
			cout << "Error: User already entried to the other room." << endl;
			return false;
		}
	}
	cout << "Error: Cannot find user or room." << endl;
	return false;
}
bool SvMach::leaveRoom(RoomKey rmKey, UserKey uKey)
{
	auto shpRm = _rooms.find(rmKey);
	auto shpUser = _users.find(uKey);

	if (shpRm != nullptr && shpUser != nullptr)
	{
		if (shpUser->curRmNum == rmKey) {
			// room information update
			bool opResult = shpRm->removeUser(uKey);

			if (opResult) {
				// user information update
				shpUser->curRmNum = CHATTERS::NO_ROOM;

				// check the room if it is empty
				if (shpRm->rtk.get_numOfPeer() == 0)
					_rooms.remove(rmKey);
				return true;
			}
		}
		else
		{
			cout << "Error: User information(room key) didn't match while SvMach::leaveRoom(..) operation." << endl;
			return false;
		}
	}

	cout << "Error: Cannot find user or room." << endl;
	return false;
}
RoomKey SvMach::openRoom(UserKey uKey, const std::string & title)
{
	auto shpUser = _users.find(uKey);

	if ((shpUser != nullptr) && (shpUser->curRmNum == CHATTERS::NO_ROOM)) {
		// create new room
		auto shpRm = _rooms.add(title);

		if (shpRm != nullptr)
		{	// success creating room
			auto rmKey = shpRm->rtk.get_key();
			
			// user participate room as first participant
			bool opResult = joinRoom(rmKey, uKey);
			if (opResult) 
			{	// success
				return rmKey;
			}
			else
			{	// failed
				_rooms.remove(rmKey);
				return InfoToken::INVALID_KEY;
			}
		}
	}
	return InfoToken::INVALID_KEY;
}
const std::shared_ptr<SvUserInfo> SvMach::findUser(UserKey uKey) 
{
	return _users.find(uKey);
}
const std::shared_ptr<SvRoomInfo> SvMach::findRoom(RoomKey rmKey)
{
	return _rooms.find(rmKey);
}
const std::unordered_map<UserKey, std::shared_ptr<SvUserInfo>>& SvMach::get_users() const
{
	return _users.get();
}
const std::unordered_map<RoomKey, std::shared_ptr<SvRoomInfo>>& SvMach::get_rooms() const
{
	return _rooms.get();
}
//std::unordered_map<UserKey, SvUserInfo>::const_iterator SvMach::findUser(UserKey uKey) const
//{
//	return _uList.find(uKey);
//}
//std::unordered_map<RoomKey, SvRoomInfo>::const_iterator SvMach::findRoom(RoomKey rKey) const
//{
//	return _rList.find(rKey);
//}
//std::unordered_map<UserKey, SvUserInfo>::iterator SvMach::findUser(UserKey uKey)
//{
//	return _uList.find(uKey);
//}
//std::unordered_map<RoomKey, SvRoomInfo>::iterator SvMach::findRoom(RoomKey rKey)
//{
//	return _rList.find(rKey);
//}
//const std::unordered_map<UserKey, SvUserInfo>* SvMach::get_userList() const
//{
//	return &_uList;
//}
//const std::unordered_map<RoomKey, SvRoomInfo>* SvMach::get_roomList() const
//{
//	return &_rList;
//}



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
	cout << "SvPacketManager::sendPacket(..) called." << endl;
	size_t pkSz;
	DWORD flags = 0;
	LPPER_IO_DATA ioInfo;

	if (spPk != nullptr)
	{
		pkSz = spPk->get_packetSize();

		ioInfo = new PerIoData(pkSz);
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		memcpy_s(ioInfo->get_buffer(), ioInfo->get_bufferLen(), spPk->get_buf().str().c_str(), pkSz);
		ioInfo->rwMode = PerIoData::WRITE;

		ioInfo->set_refCount(spPk->sockList.size());
		for (const auto& el : spPk->sockList)
		{
			cout << "WSASending packet.." << endl;
			WSASend(el, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
		}
	}	
}
std::shared_ptr<Packet_Base> SvPacketManager::recvPacket()
{
	auto& pm = SvPacketManager::Instance();

	while (pm._msgQueue.empty())	// queue에 packet이 없으면 대기
		;

	//rev lock..? pop 한줄에만 걸어도 될 것 같은데?
	auto shPk = pm._msgQueue.front();
	pm._msgQueue.pop();

	//return std::move(shPk);		//rev
	return shPk;
}
SvPacketManager::SvPacketManager()
{
	// left blank intentionally
}
SvPacketManager::SvPacketManager(const SvPacketManager &)
{
	// left blank intentionally
}
SvPacketManager::SvPacketManager(SvPacketManager &&)
{
	// left blank intentionally
}
//SvPacketManager & SvPacketManager::operator=(const SvPacketManager &)
//{
//	// left blank intentionally
//	// TODO: 여기에 반환 구문을 삽입합니다.
//}



/*********************************************************************
 * etc. functions definitions
*********************************************************************/
DWORD WINAPI recvThreadMain(LPVOID pComPort)
{
	cout << "Receiving thread starts.." << endl;

	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&sock, (LPOVERLAPPED *)&ioInfo, INFINITE);
		cout << endl << "GetQueuedComplitionStatus(mode: " << ioInfo->rwMode << ") bytesTrans = " << bytesTrans << endl;

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
					shPk->sockList.push_back(sock);
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
	cout << "Receiving thread released.." << endl;
	return 0;
}

DWORD WINAPI packetProcessWorkerThreadMain(LPVOID pComPort)
{
	cout << "Packet processing worker thread starts.." << endl;
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

	cout << "Packet processing worker thread released." << endl;

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

