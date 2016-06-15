#include <iostream>
#include <string>

std::string & stringCheck(std::string & str)
{
	for (auto & el : str)
	{
		if (el == '|')
			el = '_';
	}

	return str;
}

int main()
{
	std::string str("asdf|qwer|1234");
	std::cout << str << std::endl;
	
	stringCheck(str);

	std::cout << str << std::endl;

	return 0;
}