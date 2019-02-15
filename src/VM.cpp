#include "VM.hpp"

# define FILEIN (this->_readFromFile ? ifs : std::cin)

VM::VM(std::string s) : _readFromFile(1), _continueReading(false), _filename(s) {
	this->_nums.push_back(f.createOperand(Int8, "7"));
}
VM::VM(void) : _readFromFile(0), _continueReading(true), _filename("") { }
VM::VM(VM const &cp) { *this = cp; }
VM::~VM(void) { }

VM &VM::operator=(VM const &rhs)
{
	if (this != &rhs)
	{
		for (auto it = this->_nums.begin(); it != this->_nums.end(); it++)
			delete *it;
		this->_nums.clear();
		this->_nums = rhs._nums;
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
	if (this->_continueReading)
		throw UnexpectedEOFException();
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

VM::PopOnEmptyStackException::PopOnEmptyStackException( void ) { }
VM::PopOnEmptyStackException::PopOnEmptyStackException( PopOnEmptyStackException const & cp) { *this = cp; }
VM::PopOnEmptyStackException::~PopOnEmptyStackException( void ) throw() { }
VM::PopOnEmptyStackException& VM::PopOnEmptyStackException::operator=( PopOnEmptyStackException const &) { return *this; }
const char* VM::PopOnEmptyStackException::what( void ) const throw() {
	return "Pop on empty stack";
}

VM::UnexpectedEOFException::UnexpectedEOFException( void ) { }
VM::UnexpectedEOFException::UnexpectedEOFException( UnexpectedEOFException const & cp) { *this = cp; }
VM::UnexpectedEOFException::~UnexpectedEOFException( void ) throw() { }
VM::UnexpectedEOFException& VM::UnexpectedEOFException::operator=( UnexpectedEOFException const &) { return *this; }
const char* VM::UnexpectedEOFException::what( void ) const throw() {
	return "Unexpected EOF while parsing";
}
