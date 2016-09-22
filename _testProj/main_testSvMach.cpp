#include "../chatters/header_server.h"

using std::cin;
using std::cout;
using std::endl;

void main()
{
	SvMach agent;		// dsn name: sqlODBC
						// user id : (none)
						// user pw : (none)

	agent.db_signin("a", "1234");

	UserKey uKey_a = agent.addUser("a", 9000);
	UserKey uKey_b = agent.addUser("b", 9001);

	RoomKey rKey = agent.openRoom(uKey_a, "Welcome to new room!");
	agent.joinRoom(rKey, uKey_b);

	int br1;

	agent.leaveRoom(rKey, uKey_a);

	int br2;

	agent.openRoom(uKey_a, "Another room");

	int br3;

	agent.removeUser(uKey_a);

	int br4;

	return;
}



// Functional 함수와 자료구조를 직접 manip. 하는 함수 사이에 구분이 있어야..
//		자료 구조(userList, roomList) 자체를 하나의 클래스로 분리. 이를 manip.하는 함수를
//		여기에 따로 분리 구현해야..