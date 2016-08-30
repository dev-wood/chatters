#include "packet.h"



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
* PacketInfo class implementation
*********************************************************************/
PacketInfo::PacketInfo() : _code(EMPTY), _msg("")
{
	// left blank intentionally
}
PkInfo::ProcCode PacketInfo::get_code() const
{
	return _code;
}
std::string PacketInfo::get_msg() const
{
	return _msg;
}
void PacketInfo::set_code(ProcCode code)
{
	_code = code;
}
void PacketInfo::set_msg(std::string && str)
{
	_msg = str;
}




/*********************************************************************
 * Packet_Base class implementation 
 *********************************************************************/
Packet_Base::Packet_Base(PTYPE pType, char * buf) : Packet_Base(pType)
{
	_buf << buf;
}
Packet_Base::~Packet_Base()
{
	// left blank intentionally
}
int Packet_Base::ptoi(PTYPE pt)
{
	return static_cast<int>(pt);
}
void Packet_Base::serialize()
{
	// Reserve header space for buf length.
	_setHeaderSpace();

	// Write packet ID on buf.
	_buf << ptoi(_id) << '|';

	// Do serialize process depend on each Packet class.
	doSerialProc();

	// Write header at the last of serialize process.
	_writeHeader();
}
void Packet_Base::deserialize()
{
	// Do deserailize process depend on each Packet class.
	doDeserialProc();
}
size_t Packet_Base::get_packetSize()
{
	return _bufSize() + sizeof(size_t);
}
size_t Packet_Base::get_bufSize()
{
	return _bufSize();
}
const char * Packet_Base::get_bufAddr() const
{
	return _buf.str().c_str();
}
const PkInfo & Packet_Base::get_pkInfo() const
{
	return _pkInfo;
}
Packet_Base & Packet_Base::operator<<(const char * buf)
{
	_buf << buf;
}
Packet_Base::Packet_Base(PTYPE pt) : _id(pt), _pkInfo(), sock(INVALID_SOCKET)
{
	// left blank intentionally
}
Packet_Base::Packet_Base() : _id(PTYPE::PT_BASE), _pkInfo(), sock(INVALID_SOCKET)
{
	// left blank intentionally
}
void Packet_Base::_setHeaderSpace()
{
	//rev setfill, width 사용해서 수정.
	char ch[sizeof(size_t) + 1];
	std::fill_n(ch, sizeof(size_t), '0');
	ch[sizeof(size_t)] = 0;

	_buf << ch;
}
void Packet_Base::_writeHeader()
{
	size_t bufSize = _bufSize();	// get packet size

	auto prevPos = _buf.tellp();	// store current input sequence pos
	_buf.seekp(_buf.beg);	// move input sequence pos to header space

	_buf.write((const char*)&bufSize, sizeof(bufSize));	// write packet size in header space

	_buf.seekp(prevPos);	// restore previous input sequence pos	
}
size_t Packet_Base::_bufSize()
{
	size_t bufSz = 0;
	auto cPos = _buf.tellg();	// store current input sequence pos
	_buf.seekg(_buf.end);
	bufSz = static_cast<int>(_buf.tellg());

	_buf.seekg(cPos);	// restore previous input sequence pos

	bufSz -= sizeof(bufSz);	// exclude packet header space

	return bufSz;
}



/*********************************************************************
* PacketManager class implementation
*********************************************************************/
PacketManager_Base::~PacketManager_Base()
{
	// left blank intentionally
}
void PacketManager_Base::setAgent(MachObject * pMObj)
{
	_pMachObject = pMObj;
}
MachObject & PacketManager_Base::getAgent()
{
	return *_pMachObject;
}
PacketManager_Base::PacketManager_Base() : _pMachObject(nullptr)
{
	// left blank intentionally
}

/*********************************************************************
 * ClntPacketManager class implementation

*********************************************************************/
ClntPacketManager & ClntPacketManager::Instance()
{
	static ClntPacketManager _instance;

	return _instance;
}
void ClntPacketManager::sendPacket(std::shared_ptr<Packet_Base> spPk)
{
	send(spPk->sock, spPk->get_bufAddr, spPk->get_packetSize, 0);
}
std::shared_ptr<Packet_Base> ClntPacketManager::recvPacket()
{
	// Packet을 TCP 전송.
	size_t pkSize, recvBytes, headerSize;
	SOCKET sock = ConnectInfo::Instance().get_sock();
	
	headerSize = sizeof(size_t);
	recvBytes = 0;
	while (recvBytes < headerSize)
		recvBytes += recv(sock, (char *)(&pkSize) + recvBytes, headerSize - recvBytes, 0);

	recvBytes = 0;
	char * buf = new char[pkSize + 1];
	while(recvBytes<headerSize) 
		recvBytes += recv(sock, buf + recvBytes, pkSize - recvBytes, 0);
	buf[pkSize] = NULL;

	return extractPacketFromBuffer(buf);
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
	auto pm = SvPacketManager::Instance();

	while (pm._msgQueue.empty())	// queue에 packet이 없으면 대기
		;

	//rev lock..? pop 한줄에만 걸어도 될 것 같은데?
	auto shPk = pm._msgQueue.front();
	pm._msgQueue.pop();
	
	return shPk;
}



/*********************************************************************
* Etc. implementation

*********************************************************************/
 