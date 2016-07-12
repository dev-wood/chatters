#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	std::stringstream ss;
	ss << "0000" << "asdf" << "1234";

	int i = 100;
	char ch[] = "qwer";

	ss.write(ch, 4);
	cout << ss.str() << endl;

	auto prevPos = ss.tellp();
	ss.seekp(ss.beg);

	// int를 stream으로 복사
	ss.write((const char*)&i, sizeof(i));
	ss.seekp(prevPos);

	char ich[4];
	memcpy(ich, ss.str().c_str(), sizeof(i));

	cout << *((int *)ich) << endl;

	return 0;
}
