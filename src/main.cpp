#include "VM.hpp"
#include "Operand.hpp"
#include "Factory.hpp"
#include <fstream>

#define FILEIN (argc > 1 ? ifs : std::cin)

int main(int argc, char **argv)
{
	std::string		str;
	std::fstream	ifs;
	VM				machine;

	static_cast<void>(argc);
	static_cast<void>(argv);

	if (argc > 1)
	{
		machine = VM(argv[1]);
		ifs.open(argv[1]);
		if (!ifs)
		{
			std::cout << "Invalid file" << std::endl;
			std::exit(1);
		}
	}
	while (std::getline(FILEIN, str))
		std::cout << str << std::endl;
}
