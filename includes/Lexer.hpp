#ifndef LEXER_HPP
# define LEXER_HPP

# include <vector>
# include <regex>
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
	Lexer( eCommand , eOperandType , double );
	Lexer( void );
	Lexer( Lexer const & cp);
	~Lexer( void );
	Lexer& operator=( Lexer const &);

	static void		generateTokens( std::string& );

private:
	eCommand		_command;
	eOperandType	_argType;
	double			_arg;

};

#endif
