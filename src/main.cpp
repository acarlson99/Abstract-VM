#include "VM.hpp"

Factory		g_factory = Factory();

int main(int argc, char **argv)
{
	std::string		str;
	std::fstream	ifs;
	VM				*machine;

	if (argc > 1)
		machine = new VM(argv[1]);
	else
		machine = new VM();

	try {
		machine->run();
	}
	catch (std::exception &e) {
		delete machine;
		std::cout << "ERROR: " << e.what() << std::endl;
		std::exit(1);
	}
	delete machine;
}
