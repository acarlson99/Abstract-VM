#include "Token.hpp"

Token::Token( void )
{
	this->_s = new std::string("");
	this->_type = ERR;
}

Token::Token( Token const & cp) { *this = cp; }

Token::~Token( void )
{
	if (this->_s)
		delete this->_s;
}

Token& Token::operator=( Token const &rhs)
{
	if (this != &rhs)
	{
		delete this->_s;
		this->_s = new std::string(rhs._s->c_str());
		this->_type = rhs._type;
	}
	return *this;
}
