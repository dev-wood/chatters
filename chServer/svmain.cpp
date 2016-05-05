#include "../chatters/header_server.h"

//// test1: SvRoomInfo class test
//int main()
//{
//	UserKey a(1);
//	UserKey b(2);
//	UserKey c(3);
//	UserKey d(4);
//	UserKey e(5);
//
//	SOCKET as = INVALID_SOCKET;
//	SOCKET bs = INVALID_SOCKET;
//	SOCKET cs = INVALID_SOCKET;
//	SOCKET ds = INVALID_SOCKET;
//	SOCKET es = INVALID_SOCKET;
//
//	// construction test
//	std::cout << "Construction test" << std::endl;
//	SvRoomInfo rm1("test room");
//
//	std::cout << rm1.addUser(a, as) << std::endl;
//	std::cout << rm1.addUser(b, bs) << std::endl;
//	std::cout << rm1.addUser(c, cs) << std::endl;
//	std::cout << rm1.addUser(d, ds) << std::endl;
//	std::cout << rm1.addUser(e, es) << std::endl;
//
//	std::cout << rm1.removeUser(a) << std::endl;
//	std::cout << rm1.removeUser(e) << std::endl;
//
//	return 0;
//}