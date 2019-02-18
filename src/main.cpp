#include "VM.hpp"
#include <float.h>

int main(int argc, char **argv)
{
	std::string		str;
	std::fstream	ifs;
	VM				machine;

	if (argc > 1)
		machine = VM(argv[1]);

	std::cout << DBL_MAX << std::endl;
	try {
		machine.run();
	}
	catch (std::exception &e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		std::exit(1);
	}
}
