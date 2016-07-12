#include <iostream>
#include <array>
#include <sstream>

int main()
{
	char ch[sizeof(size_t) + 1];
	std::fill_n(ch, sizeof(size_t), '0');
	ch[sizeof(size_t)] = 0;

	std::stringstream ss;
	ss << ch;
	ss << "asdf";
	std::cout << ss.str() << std::endl;



	/*char ch[sizeof(int)];

	std::cout << ch << std::endl;

	int val = 1234;

	memcpy(ch, &val, sizeof(int));

	std::cout << *((int*)ch) << std::endl;
*/
	return 0;
}