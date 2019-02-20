#include "Lexer.hpp"

//             command type    arg
static std::regex		patterns[] = {
	std::regex("^push (int8)\\((-?[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^push (int16)\\((-?[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^push (int32)\\((-?[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^push (float)\\((-?[0-9]+\\.[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^push (double)\\((-?[0-9]+\\.[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^pop[ \t]*;?.*"),
	std::regex("^dump[ \t]*;?.*"),
	std::regex("^assert (int8)\\((-?[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^assert (int16)\\((-?[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^assert (int32)\\((-?[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^assert (float)\\((-?[0-9]+\\.[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^assert (double)\\((-?[0-9]+\\.[0-9]+)\\)[ \t]*;?.*"),
	std::regex("^add[ \t]*;?.*"),
	std::regex("^sub[ \t]*;?.*"),
	std::regex("^mul[ \t]*;?.*"),
	std::regex("^div[ \t]*;?.*"),
	std::regex("^mod[ \t]*;?.*"),
	std::regex("^print[ \t]*;?.*"),
	std::regex("^exit[ \t]*;?.*"),
	std::regex("^;;[ \t]*(?:;.*)?"),
	std::regex("^[ \t]*(?:;.*)?"),
};

Lexer::Lexer( eCommand c, eOperandType o, std::string arg, size_t line ) : _command(c), _argType(o), _arg(new std::string(arg)), _line(line) { }

Lexer::Lexer( eCommand c, size_t line ) : Lexer(c, Double, "nan", line) { }

Lexer::Lexer( void ) : Lexer(Error, Double, "nan", 0) { }

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
		delete this->_arg;
		this->_arg = new std::string(rhs._arg->c_str());
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

size_t				Lexer::getLine( void ) const
{
	return (this->_line);
}

Lexer		*Lexer::generateTokens( std::string &s, size_t line )
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
			if (m.size() == 3)
			{
				type = m[1];
				arg = m[2];
				return (new Lexer(c, _strToOp.at(type), arg, line));
			}
			return (new Lexer(c, line));
		}
	return (new Lexer());
}

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

std::ostream	&operator<<( std::ostream &o, Lexer const &l )
{
	o << opToStr[l.getCommand()] << "(" << l.getArg() << ")";
	return (o);
}
