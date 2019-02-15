#include "VM.hpp"

int main(int argc, char **argv)
{
	std::string		str;
	std::fstream	ifs;
	VM				machine;
	Factory			f;

	if (argc > 1)
	{
		machine = VM(argv[1]);
	}

	try {
		machine.readLoop();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		exit(0);
	}
}
