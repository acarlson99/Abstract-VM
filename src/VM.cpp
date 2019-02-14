#include "VM.hpp"

VM::VM(std::string s) : _read_from_file(1), _filename(s) { }
VM::VM(void) : _read_from_file(0) { }
VM::VM(VM const &cp) { *this = cp; }
VM::~VM(void) { }

VM &VM::operator=(VM const &rhs)
{
    if (this != &rhs)
    {
        this->_nums = rhs._nums;
        this->_read_from_file = rhs._read_from_file;
        this->_filename = rhs._filename;
    }
    return *this;
}
