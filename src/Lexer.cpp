#include "Lexer.hpp"

static std::string		opToStr[] = {
	"PushInt8",
	"PushInt16",
	"PushInt32",
	"PushFloat",
	"PushDouble",
	"Pop",
	"Dump",
	"AssertInt8",
	"AssertInt16",
	"AssertInt32",
	"AssertFloat",
	"AssertDouble",
	"Add",
	"Sub",
	"Mul",
	"Div",
	"Mod",
	"Print",
	"Exit",
	"Eof",
	"Comment",
	"Error",
};

//             command type    arg
static std::regex		patterns[] = {
	std::regex("^push (int8)\\((\\d+)\\) *;?"),
	std::regex("^push (int16)\\((\\d+)\\) *;?"),
	std::regex("^push (int32)\\((\\d+)\\) *;?"),
	std::regex("^push (float)\\((\\d+)\\) *;?"),
	std::regex("^push (double)\\((\\d+)\\) *;?"),
	std::regex("^pop *;?"),
	std::regex("^dump *;?"),
	std::regex("^assert (int8)\\((\\d+)\\) *;?"),
	std::regex("^assert (int16)\\((\\d+)\\) *;?"),
	std::regex("^assert (int32)\\((\\d+)\\) *;?"),
	std::regex("^assert (float)\\((\\d+)\\) *;?"),
	std::regex("^assert (double)\\((\\d+)\\) *;?"),
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

//	return ((this->*_f.at(type))(value));

Lexer::Lexer( eCommand c, eOperandType o, std::string arg) : _command(c), _argType(o), _arg(new std::string(arg)) { }

Lexer::Lexer( eCommand c ) : Lexer(c, Double, "nan") { }

Lexer::Lexer( void ) : Lexer(Error, Double, "nan") { }

Lexer::Lexer( Lexer const & cp) { *this = cp; }

Lexer::~Lexer( void )
{
	delete this->_arg;
}

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

eCommand			Lexer::getCommand( void ) const
{
	return (this->_command);
}

eOperandType		Lexer::getType( void ) const
{
	return (this->_argType);
}

std::string const	&Lexer::getArg( void ) const
{
	return (*this->_arg);
}

Lexer		*Lexer::generateTokens( std::string &s )
{
	eCommand		c;
	std::string		type;
	std::smatch		m;
	int				i = 0;
	std::string		arg = "nan";
	static std::map<std::string, eOperandType>		_strToOp;
	_strToOp["int8"] = Int8;
	_strToOp["int16"] = Int16;
	_strToOp["int32"] = Int32;
	_strToOp["float"] = Float;
	_strToOp["double"] = Double;

	for (; i != Error; i++)
		if (std::regex_match(s, m, patterns[i]))
		{
			c = static_cast<eCommand>(i);
			std::cout << c << std::endl;
			std::cout << m.size() << std::endl;
			if (m.size() == 3)
			{
				type = m[1];
				arg = m[2];
				return (new Lexer(c, _strToOp.at(type), arg));
			}
			return (new Lexer(c));
		}
	return (new Lexer());
}

std::ostream	&operator<<( std::ostream &o, Lexer const &l )
{
	o << opToStr[l.getCommand()] << "(" << l.getArg() << ")";
	return (o);
}
