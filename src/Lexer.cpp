#include "Lexer.hpp"

//              command  type     arg
static std::regex		patterns[] = {
	std::regex("^(push) (int8)\\((\\d+)\\) *;?"),
	std::regex("^(push) (int16)\\((\\d+)\\) *;?"),
	std::regex("^(push) (int32)\\((\\d+)\\) *;?"),
	std::regex("^(push) (float)\\((\\d+)\\) *;?"),
	std::regex("^(push) (double)\\((\\d+)\\) *;?"),
	std::regex("^(pop) *;?"),
	std::regex("^(dump) *;?"),
	std::regex("^(assert) (int8)\\((\\d+)\\) *;?"),
	std::regex("^(assert) (int16)\\((\\d+)\\) *;?"),
	std::regex("^(assert) (int32)\\((\\d+)\\) *;?"),
	std::regex("^(assert) (float)\\((\\d+)\\) *;?"),
	std::regex("^(assert) (double)\\((\\d+)\\) *;?"),
	std::regex("^(add) *;?"),
	std::regex("^(sub) *;?"),
	std::regex("^(mul) *;?"),
	std::regex("^(div) *;?"),
	std::regex("^(mod) *;?"),
	std::regex("^(print) *;?"),
	std::regex("^(exit) *;?"),
	std::regex("^(;;) *;?"),
	std::regex("^ *;?"),
};

Lexer::Lexer( void ) { }
Lexer::Lexer( Lexer const & cp) { *this = cp; }
Lexer::~Lexer( void )
{
	for (auto it = this->_tokens.begin(); it != this->_tokens.end(); it++)
		delete *it;
}

Lexer& Lexer::operator=( Lexer const &rhs)
{
	if (this != &rhs)
	{
		for (auto it = this->_tokens.begin(); it != this->_tokens.end(); it++)
			delete *it;
		this->_tokens.clear();
		for (auto it = rhs._tokens.begin(); it != rhs._tokens.end(); it++)
			this->_tokens.push_back(new Token(**it));
	}
	return *this;
}

void		Lexer::generateTokens( std::string &s )
{
	eCommand		c;
	std::smatch		m;
	
	for (int i = PushInt8; i != Error; i++)
	{
		if (std::regex_match(s, m, patterns[i]))
		{
			std::cout << i << std::endl;
			c = static_cast<eCommand>(i);
		}
/*
** 		if (patterns[i].match(s))
** 			std::cout << i << std::endl;
*/
	}
}
