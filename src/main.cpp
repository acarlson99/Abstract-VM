#include "VM.hpp"
#include "Operand.hpp"
#include "Factory.hpp"

int main()
{
	Operand<int>	a = 32;

	std::cout << "Hello, world!" << std::endl;
	std::cout << a.getValue() << std::endl;
}
