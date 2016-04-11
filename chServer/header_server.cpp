#include "header_server.h"

SvConInfo::SvConInfo()
{
}
SvConInfo::SvConInfo(WSADATA wsaData, SOCKET sock, sockaddr servAddr)
{
	_wsaData = wsaData;
	_sock = sock;
	_servAddr = servAddr;
}

SvUserInfo::SvUserInfo()
{
}
SvUserInfo::SvUserInfo(const SvUserInfo & ui)
{
	_utk = ui._utk;
	_curRmNum = ui._curRmNum;
	_sockNum = ui._sockNum;
}
SvUserInfo::SvUserInfo(SvUserInfo && ui)
{
	*this = std::move(ui);
}
SvUserInfo & SvUserInfo::operator=(SvUserInfo && ui)
{
	if (this != &ui)
	{
		_utk = std::move(ui._utk);
		_curRmNum = ui._curRmNum;
		_sockNum = ui._sockNum;
	}

	return *this;
}
SvUserInfo & SvUserInfo::operator=(const SvUserInfo & ui)
{
	_utk = ui._utk;
	_curRmNum = ui._curRmNum;
	_sockNum = ui._sockNum;
}

