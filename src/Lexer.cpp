#include "Lexer.hpp"

Lexer::Lexer( void ) { }
Lexer::Lexer( Lexer const & cp) { *this = cp; }
Lexer::~Lexer( void ) { }
Lexer& Lexer::operator=( Lexer const &) { return *this; }
