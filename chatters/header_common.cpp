#include "header_common.h"


UserKey UserInfoToken::getUniqueKey()
{
	return UniqueKey++;
}

UserInfoToken::UserInfoToken()
{
	key = getUniqueKey();
}
UserInfoToken::UserInfoToken(UserInfoToken&& tk)
{
	*this = std::move(tk);
}
UserInfoToken & UserInfoToken::operator=(UserInfoToken&& tk)
{
	if (this == &tk) 
		return *this;

	this->key = tk.key;
	std::swap(this->id, tk.id);

	return *this;
}
UserInfoToken::UserInfoToken(const std::string& _id)
{
	id = _id;
	key = getUniqueKey();
}

UserKey RoomInfoToken::getUniqueKey()
{
	return UniqueKey++;
}

RoomInfoToken::RoomInfoToken()
{
	numOfPeer = 0;
	key = getUniqueKey();
}
RoomInfoToken::RoomInfoToken(RoomInfoToken && tk)
{
	*this = std::move(tk);
}
RoomInfoToken & RoomInfoToken::operator=(RoomInfoToken && tk)
{
	if (this == &tk)
		return *this;

	this->key = tk.key;
	std::swap(this->title, tk.title);
	
	return *this;
}
RoomInfoToken::RoomInfoToken(const std::string& _title)
{
	title = _title;
	numOfPeer = 0;
	key = getUniqueKey();
}

void UserInfoTokenList::add(UserInfoToken tk)
{
	list.insert(std::make_pair(tk.key, tk));
}

bool UserInfoTokenList::remove(UserKey uKey)
{
	if (list.erase(uKey))
		return true;
	else 
		return false;
}

UserInfoToken & UserInfoTokenList::find(UserKey uKey)
{
	return (list.find(uKey))->second;
}

void RoomInfoTokenList::add(RoomInfoToken tk)
{
	list.insert(std::make_pair(tk.key, tk));
}

bool RoomInfoTokenList::remove(RoomKey rKey)
{
	if (list.erase(rKey))
		return true;
	else
		return false;
}

RoomInfoToken & RoomInfoTokenList::find(RoomKey rKey)
{
	return (list.find(rKey))->second;
}

PacketStream_Base & PacketStream_Base::operator<<(const UserInfoToken & utk)
{
	buf << utk.key << '|' << utk.id << '|';

	return *this;
}

PacketStream_Base & PacketStream_Base::operator<<(const RoomInfoToken & rtk)
{
	buf << rtk.key << '|' << rtk.title << '|' << rtk.numOfPeer << '|';

	return *this;
}
