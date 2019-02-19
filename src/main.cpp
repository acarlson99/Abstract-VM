#include "VM.hpp"

Factory		g_factory = Factory();

int main(int argc, char **argv)
{
	std::string		str;
	std::fstream	ifs;
	VM				machine;

	if (argc > 1)
		machine = VM(argv[1]);

	try {
		machine.run();
	}
	catch (std::exception &e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		std::exit(1);
	}
}
