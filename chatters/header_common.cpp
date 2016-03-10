#include "header_common.h"


UserInfoToken::UserInfoToken()
{
	key = UniqueKey++;
}
UserInfoToken::UserInfoToken(const std::string& _id)
{
	id = _id;
	key = UniqueKey++;
}

RoomInfoToken::RoomInfoToken()
{
	numOfPeer = 0;
	key = UniqueKey++;
}
RoomInfoToken::RoomInfoToken(const std::string& _title)
{
	title = _title;
	numOfPeer = 0;
	key = UniqueKey++;
}
