#include "VM.hpp"

VM::VM( void ) { }
VM::VM( VM const &cp ) { *this = cp; }
VM::~VM( void ) { }
VM &VM::operator=( VM const &rhs )
{
    this->_nums = rhs._nums;
    return *this;
}
