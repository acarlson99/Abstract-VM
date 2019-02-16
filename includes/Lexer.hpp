#ifndef LEXER_HPP
# define LEXER_HPP

# include <vector>
# include <regex>
# include <iostream>
# include "Token.hpp"

class Lexer {

public:
	Lexer( void );
	Lexer( Lexer const & cp);
	~Lexer( void );
	Lexer& operator=( Lexer const &);

	static void		generateTokens( std::string& );

private:
	std::vector<Token *>		_tokens;

};

#endif
