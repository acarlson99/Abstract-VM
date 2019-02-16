#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

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

enum eTokenType {
	Command,
	Symbol,
	Number,
	Invalid,
};

class Token {

public:
	Token( void );
	Token( Token const & cp);
	~Token( void );
	Token& operator=( Token const &);

private:
	std::string			*_s;
	int					_type;

};

#endif
