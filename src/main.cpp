#include "VM.hpp"

Factory		g_factory = Factory();

int main(int argc, char **argv)
{
	std::string		str;
	std::fstream	ifs;
	VM				*machine;

	if (argc == 2)
	{
		if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")
		{
			std::cout << "Commands =" << std::endl
					  << "\tpush arg" << std::endl
					  << "\tpop" << std::endl
					  << "\tdump" << std::endl
					  << "\tassert arg" << std::endl
					  << "\tadd" << std::endl
					  << "\tsub" << std::endl
					  << "\tmul" << std::endl
					  << "\tdiv" << std::endl
					  << "\tmod" << std::endl
					  << "\tprint" << std::endl
					  << "\texit" << std::endl
					  << std::endl
					  << "arg =" << std::endl
					  << "\tint8(-?[0-9]+)" << std::endl
					  << "\tint16(-?[0-9]+)" << std::endl
					  << "\tint32(-?[0-9]+)" << std::endl
					  << "\tfloat(-?[0-9]+\\.[0-9]+)" << std::endl
					  << "\tdouble(-?[0-9]+\\.[0-9]+)" << std::endl;
			std::exit(0);
		}
		machine = new VM(argv[1]);
	}
	else if (argc == 1)
		machine = new VM();
	else
	{
		std::cout << "usage: ./avm [file]" << std::endl;
		std::exit(1);
	}

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
