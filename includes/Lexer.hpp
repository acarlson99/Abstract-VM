#ifndef LEXER_HPP
# define LEXER_HPP

# include <vector>
# include <regex>
# include <map>
# include <iostream>

enum eCommand {
	PushInt8,
	PushInt16,
	PushInt32,
	PushFloat,
	PushDouble,
	Pop,
	Dump,
	AssertInt8,
	AssertInt16,
	AssertInt32,
	AssertFloat,
	AssertDouble,
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	Print,
	Exit,
	Eof,
	Comment,
	Error,
};

# ifndef OPTYPE
#  define OPTYPE
enum eOperandType {
	Int8,
	Int16,
	Int32,
	Float,
	Double,
};
#endif

class Lexer {

public:
	Lexer( eCommand , eOperandType , std::string );
	Lexer( eCommand );
	Lexer( void );
	Lexer( Lexer const & cp);
	~Lexer( void );
	Lexer& operator=( Lexer const &);

	static Lexer		*generateTokens( std::string& );
	eCommand			getCommand( void ) const;
	eOperandType		getType( void ) const;
	std::string const	&getArg( void ) const;

private:
	eCommand		_command;
	eOperandType	_argType;
	std::string		*_arg;

};

std::ostream	&operator<<( std::ostream &o, Lexer const &f );

#endif
