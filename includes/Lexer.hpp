#ifndef LEXER_HPP
# define LEXER_HPP

# include <vector>
# include "Token.hpp"

class Lexer {

public:
	Lexer( void );
	Lexer( Lexer const & cp);
	~Lexer( void );
	Lexer& operator=( Lexer const &);

private:
	std::vector<Token *>		_tokens;

};

#endif
