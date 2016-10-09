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



// Functional �Լ��� �ڷᱸ���� ���� manip. �ϴ� �Լ� ���̿� ������ �־��..
//		�ڷ� ����(userList, roomList) ��ü�� �ϳ��� Ŭ������ �и�. �̸� manip.�ϴ� �Լ���
//		���⿡ ���� �и� �����ؾ�..