#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

enum eTokenTypes {
	ERR,
	ENDREAD,
	COMMAND,
	INT,
	SYMBOL,
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
