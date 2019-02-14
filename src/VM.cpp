#include "VM.hpp"

# define FILEIN (this->_readFromFile ? ifs : std::cin)

VM::VM(std::string s) : _readFromFile(1), _continueReading(false), _filename(s) {
	// this->_nums.push_back(f.createOperand(Int8, "7"));

}
VM::VM(void) : _readFromFile(0), _continueReading(true), _filename("") { }
VM::VM(VM const &cp) { *this = cp; }
VM::~VM(void) { }

VM &VM::operator=(VM const &rhs)
{
	if (this != &rhs)
	{
		this->_nums = rhs._nums;
		for (auto it = rhs._nums.begin(); it < _nums.end(); it++)
			std::cout << (*it)->toString() << std::endl;
		this->_readFromFile = rhs._readFromFile;
		this->_filename = rhs._filename;
	}
	return *this;
}

void		VM::readLoop( void )
{
	std::string		str;
	std::fstream	ifs;
	if (this->_readFromFile)
	{
		ifs.open(this->_filename);
		if (!ifs)
			throw InvalidFileException();
	}
	while ((this->_readFromFile || this->_continueReading) && std::getline(FILEIN, str))
	{
		this->parseIn(this->lexIn(str), str);
	}
}

eCommand	VM::lexIn( std::string &s )
{
	static_cast<void>(s);
	return (Exit);
}

void		VM::parseIn( eCommand c, std::string &s )
{
	static_cast<void>(c);
	static_cast<void>(s);
	if (s == ";;")
		this->_continueReading = false;
}

VM::InvalidFileException::InvalidFileException( void ) { }
VM::InvalidFileException::InvalidFileException( InvalidFileException const & cp) { *this = cp; }
VM::InvalidFileException::~InvalidFileException( void ) throw() { }
VM::InvalidFileException& VM::InvalidFileException::operator=( InvalidFileException const &) { return *this; }
const char* VM::InvalidFileException::what( void ) const throw() {
	return "Invalid file";
}
