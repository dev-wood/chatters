#include "header_common.h"

InfoToken::InfoToken() : _key(-1)
{
	//std::cout << "InfoToken() called" << std::endl;
}
InfoToken::~InfoToken()
{
	//std::cout << "~InfoToken() called" << std::endl;
}

UserKey UserInfoToken::_getUniqueKey()
{
	return _UniqueKey++;
}
UserInfoToken::UserInfoToken()
{
	_key = -1;
	_id = "DEFAULT";
}
UserInfoToken::UserInfoToken(UserInfoToken&& tk)
{
	*this = std::move(tk);
}
UserInfoToken & UserInfoToken::operator=(UserInfoToken&& tk)
{
	if (this == &tk) 
		return *this;

	this->_key = tk._key;
	tk._key = -1;
	std::swap(this->_id, tk._id);

	return *this;
}
UserInfoToken & UserInfoToken::operator=(const UserInfoToken & utk)
{
	_key = utk._key;
	_id = utk._id;

	return *this;
}
UserKey UserInfoToken::get_key()
{
	return _key;
}
const std::string & UserInfoToken::get_id()
{
	return _id;
}
UserInfoToken::UserInfoToken(const std::string& id)
{
	_id = id;
	_key = _getUniqueKey();
}
UserInfoToken::UserInfoToken(const UserInfoToken & tk)
{
	_key = tk._key;
	_id = tk._id;
}
std::ostream & operator<<(std::ostream & os, const UserInfoToken &utk)
{
	return os << "UserInfoToken object(key: " << utk.get_key()
		<< ", id: " << utk.get_id()
		<< ")" << std::endl;
}

UserKey RoomInfoToken::_getUniqueKey()
{
	return _UniqueKey++;
}
RoomInfoToken::RoomInfoToken()
{}
RoomInfoToken::RoomInfoToken(const std::string& title)
{
	_title = title;
	_numOfPeer = 0;
	_key = _getUniqueKey();
}
RoomInfoToken::RoomInfoToken(const RoomInfoToken & tk)
{
	_title = tk._title;
	_numOfPeer = tk._numOfPeer;
	_key = tk._key;
}
RoomInfoToken::RoomInfoToken(RoomInfoToken && tk)
{
	*this = std::move(tk);
}
RoomInfoToken & RoomInfoToken::operator=(RoomInfoToken && tk)
{
	if (this == &tk)
		return *this;

	this->_key = tk._key;
	tk._key = -1;
	std::swap(this->_title, tk._title);

	return *this;
}
RoomInfoToken & RoomInfoToken::operator=(const RoomInfoToken & rtk)
{
	_key = rtk._key;
	_title = rtk._title;
	_numOfPeer = rtk._numOfPeer;

	return *this;
}
RoomKey RoomInfoToken::get_key()
{
	return _key;
}
const std::string & RoomInfoToken::get_title()
{
	return _title;
}
std::ostream & operator<<(std::ostream & os, const RoomInfoToken & rtk)
{
	return os << "RoomInfoToken object(key: " << rtk.get_key()
		<< ", title: " << rtk.get_title()
		<< ", numOfPeer: " << rtk._numOfPeer
		<< ")" << std::endl;
}

int PacketStream::extractReqType()
{
	std::string tmp;
	std::getline(_buf, tmp, '|');
	
	return std::stoi(tmp);
}
PacketStream& PacketStream::operator<<(const UserInfoToken & utk)
{
	_buf << utk.get_key() << '|' << utk.get_id() << '|';

	return *this;
}
PacketStream& PacketStream::operator<<(const RoomInfoToken & rtk)
{
	_buf << rtk.get_key() << '|' << rtk.get_title() << '|' << rtk._numOfPeer << '|';

	return *this;
}

RcvMessage::RcvMessage()
{}
RcvMessage::RcvMessage(PType ptype, bool shr, std::stringstream ss) : _rqType(ptype), _sharable(shr)
{
	std::swap(_buf, ss);
}
RcvMessage::RcvMessage(const RcvMessage & tk) : _rqType(tk._rqType), _sharable(tk._sharable)
{
	_buf << tk._buf.rdbuf();
}
RcvMessage::RcvMessage(RcvMessage&& rMsg)
{
	*this = std::move(rMsg);
}
RcvMessage & RcvMessage::operator=(RcvMessage&& rMsg)
{
	if (this == &rMsg)
		return *this;
	
	_rqType = rMsg._rqType;
	_sharable = rMsg._sharable;
	std::swap(_buf, rMsg._buf);
	return *this;
}


