#include "Lexer.hpp"

//             command type    arg
static std::regex		patterns[] = {
	std::regex("^push int8\\((\\d+)\\) *;?"),
	std::regex("^push int16\\((\\d+)\\) *;?"),
	std::regex("^push int32\\((\\d+)\\) *;?"),
	std::regex("^push float\\((\\d+)\\) *;?"),
	std::regex("^push double\\((\\d+)\\) *;?"),
	std::regex("^pop *;?"),
	std::regex("^dump *;?"),
	std::regex("^assert int8\\((\\d+)\\) *;?"),
	std::regex("^assert int16\\((\\d+)\\) *;?"),
	std::regex("^assert int32\\((\\d+)\\) *;?"),
	std::regex("^assert float\\((\\d+)\\) *;?"),
	std::regex("^assert double\\((\\d+)\\) *;?"),
	std::regex("^add *;?"),
	std::regex("^sub *;?"),
	std::regex("^mul *;?"),
	std::regex("^div *;?"),
	std::regex("^mod *;?"),
	std::regex("^print *;?"),
	std::regex("^exit *;?"),
	std::regex("^;; *;?"),
	std::regex("^ *(?:;.*)?"),
};

Lexer::Lexer( eCommand c, eOperandType o, double arg) : _command(c), _argType(o), _arg(arg) { }
Lexer::Lexer( void ) { }
Lexer::Lexer( Lexer const & cp) { *this = cp; }
Lexer::~Lexer( void ) { }

Lexer& Lexer::operator=( Lexer const &rhs)
{
	if (this != &rhs)
	{
		this->_command = rhs._command;
		this->_argType = rhs._argType;
		this->_arg = rhs._arg;
	}
	return *this;
}

void		Lexer::generateTokens( std::string &s )
{
	eCommand		c;
	std::smatch		m;
	int				i = 0;
	double			arg = 0.0/0.0;
	
	for (; i != Error; i++)
		if (std::regex_match(s, m, patterns[i]))
		{
			c = static_cast<eCommand>(i);
			std::cout << c << std::endl;
			std::cout << m.size() << std::endl;
			if (m.size() == 2)
				arg = std::stod(m[1]);
			std::cout << arg << std::endl;
		}
}
