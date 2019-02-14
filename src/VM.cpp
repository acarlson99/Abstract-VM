#include "VM.hpp"

VM::VM( std::string s )
{
    this->_read_from_file = 1;
    this->_filename = s;
}

VM::VM( void ) : _read_from_file(0) { }
VM::VM( VM const &cp ) { *this = cp; }
VM::~VM( void ) // TODO: free everything
{
    while (!this->_nums.empty())
    {
        delete this->_nums.top();
        this->_nums.pop();
    }
}

VM &VM::operator=( VM const &rhs )  // TODO: copy EVERYTHING
{
    this->_nums = rhs._nums;
    return *this;
}
