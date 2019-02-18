#include "VM.hpp"

# define FILEIN (this->_readFromFile ? ifs : std::cin)

VM::VM( std::string s ) : _readFromFile(1), _continueReading(false), _filename(s), _eval(true)
{
	_funcs[PushInt8] = &VM::VMpush;
	_funcs[PushInt16] = &VM::VMpush;
	_funcs[PushInt32] = &VM::VMpush;
	_funcs[PushFloat] = &VM::VMpush;
	_funcs[PushDouble] = &VM::VMpush;
	_funcs[Pop] = &VM::VMpop;
	_funcs[Dump] = &VM::VMdump;
	_funcs[AssertInt8] = &VM::VMassert;
	_funcs[AssertInt16] = &VM::VMassert;
	_funcs[AssertInt32] = &VM::VMassert;
	_funcs[AssertFloat] = &VM::VMassert;
	_funcs[AssertDouble] = &VM::VMassert;
	_funcs[Add] = &VM::VMadd;
	_funcs[Sub] = &VM::VMsub;
	_funcs[Mul] = &VM::VMmul;
	_funcs[Div] = &VM::VMdiv;
	_funcs[Mod] = &VM::VMmod;
	_funcs[Print] = &VM::VMprint;
	_funcs[Exit] = &VM::VMexit;	// exit
	_funcs[Eof] = &VM::VMcatch;	// ignore
	_funcs[Comment] = &VM::VMcatch;	// ignore
	_funcs[Error] = &VM::VMcatch;	// this should be handled in read_loop
}

VM::VM( void ) : _readFromFile(0), _continueReading(true), _filename(""), _eval(true)
{
	_funcs[PushInt8] = &VM::VMpush;
	_funcs[PushInt16] = &VM::VMpush;
	_funcs[PushInt32] = &VM::VMpush;
	_funcs[PushFloat] = &VM::VMpush;
	_funcs[PushDouble] = &VM::VMpush;
	_funcs[Pop] = &VM::VMpop;
	_funcs[Dump] = &VM::VMdump;
	_funcs[AssertInt8] = &VM::VMassert;
	_funcs[AssertInt16] = &VM::VMassert;
	_funcs[AssertInt32] = &VM::VMassert;
	_funcs[AssertFloat] = &VM::VMassert;
	_funcs[AssertDouble] = &VM::VMassert;
	_funcs[Add] = &VM::VMadd;
	_funcs[Sub] = &VM::VMsub;
	_funcs[Mul] = &VM::VMmul;
	_funcs[Div] = &VM::VMdiv;
	_funcs[Mod] = &VM::VMmod;
	_funcs[Print] = &VM::VMprint;
	_funcs[Exit] = &VM::VMexit;	// exit
	_funcs[Eof] = &VM::VMcatch;	// ignore
	_funcs[Comment] = &VM::VMcatch;	// ignore
	_funcs[Error] = &VM::VMcatch;	// this should be handled in read_loop
}

VM::VM( VM const &cp ) { *this = cp; }

VM::~VM( void )
{
	for (auto it = this->_nums.begin(); it != this->_nums.end(); it++)
	{
		delete *it;
	}
}

VM &VM::operator=(VM const &rhs)
{
	if (this != &rhs)
	{
		for (auto it = this->_nums.begin(); it != this->_nums.end(); it++)
			delete *it;
		this->_nums.clear();
		for (auto it = rhs._nums.begin(); it != rhs._nums.end(); it++)
			this->_nums.push_back(this->_factory.createOperand((*it)->getType(), (*it)->toString()));
		for (auto it = this->_commands.begin(); it != this->_commands.end(); it++)
			delete *it;
		this->_commands.clear();
		for (auto it = rhs._commands.begin(); it != rhs._commands.end(); it++)
			this->_commands.push_back(new Lexer(**it));
		this->_readFromFile = rhs._readFromFile;
		this->_continueReading = rhs._continueReading;
		this->_filename = rhs._filename;
		this->_eval = rhs._eval;
	}
	return *this;
}

void		VM::printCommands( void )	// TODO: remove
{
	for (auto it : this->_commands)
		std::cout << *it << std::endl;
}

void		VM::run( void )
{
	try {
		readLoop();
	}
	catch ( std::exception &e ) {
		this->_eval = false;
		std::cout << e.what() << std::endl;
	}
	try {
		if (this->_eval)
			evaluateLoop();
	}
	catch ( std::exception &e ) {
		std::cout << e.what() << std::endl;
	}
}

void		VM::readLoop( void )
{
	std::string		str;
	std::fstream	ifs;
	Lexer			*l;
	bool			pushed;
	size_t			line = 1;
	bool			exited = false;

	if (this->_readFromFile)
	{
		ifs.open(this->_filename);
		if (!ifs)
		{
			exited = true;
			this->_continueReading = false;
			this->_eval = false;
			throw InvalidFileException();
		}
	}
	while ((this->_readFromFile || this->_continueReading) && std::getline(FILEIN, str))
	{
		pushed = 0;
		l = Lexer::generateTokens(str, line);
		std::cout << *l << std::endl;
		try {
			if (l->getCommand() == Error)
			{
				throw InvalidCommandException(line);
				this->_eval = false;
			}
			else if (l->getCommand() == Eof)
				this->_continueReading = false;
			else if (l->getCommand() != Comment && !exited)
			{
				this->_commands.push_back(l);
				if (l->getCommand() == Exit)
					exited = 1;
				pushed = 1;
			}
		}
		catch ( std::exception &e ) {
			this->_eval = false;
			std::cout << e.what() << std::endl;
		}
		if (!pushed)
			delete l;
		++line;
	}
	if (this->_continueReading)
		throw UnexpectedEOFException();
	else if (!exited)
		throw NoExitException();
}

void		VM::evaluateLoop( void )
{
	std::cout << "EVAL" << std::endl;	// TODO: delet this
	for (auto it : this->_commands)
	{
		std::cout << *it << std::endl;	// TODO: delet this
		(this->*_funcs.at(it->getCommand()))(it);
		delete it;
	}
	throw NoExitException();
}

IOperand const		*VM::popUtil( void )
{
	IOperand const		*o;

	if (!this->_nums.size())
		throw PopOnEmptyStackException();
	o = this->_nums.back();
	this->_nums.pop_back();
	return (o);
}

void		VM::VMpush( Lexer const *l )
{
	this->_nums.push_back(this->_factory.createOperand(l->getType(), l->getArg()));
}

void		VM::VMpop( Lexer const* )
{
	delete this->popUtil();
}

void		VM::VMdump( Lexer const* )
{
	for (auto it : this->_nums)
		std::cout << it->toString() << std::endl;
}

void		VM::VMassert( Lexer const *l )
{
	if (!this->_nums.size())
		throw PopOnEmptyStackException();
	IOperand	const *op = this->_nums.back();
	IOperand	const *cmp = this->_factory.createOperand(l->getType(), l->getArg());

	if (op->getType() == cmp->getType() && op->toString() == cmp->toString())
	{
		delete cmp;
		return ;
	}
	delete cmp;
	throw UntrueAssertionException(l->getLine());
}

void		VM::VMadd( Lexer const* )
{
	std::cout << "add called" << std::endl;
}

void		VM::VMsub( Lexer const* )
{
	std::cout << "sub called" << std::endl;
}

void		VM::VMmul( Lexer const* )
{
	std::cout << "mul called" << std::endl;
}

void		VM::VMdiv( Lexer const* )
{
	std::cout << "div called" << std::endl;
}

void		VM::VMmod( Lexer const* )
{
	std::cout << "mod called" << std::endl;
}

void		VM::VMprint( Lexer const *l )
{
	if (!this->_nums.size())
		throw PopOnEmptyStackException();
	IOperand	const *op = this->_nums.back();
	if (op->getType() != Int8)
		throw UntrueAssertionException(l->getLine());
	std::cout << static_cast<char>(std::stoi(op->toString())) << std::endl;
}

void		VM::VMexit( Lexer const* )
{
	std::exit(0);
}

void		VM::VMcatch( Lexer const *l)
{
	std::cout << "VM::VMcatch called on " << *l << " somehow" << std::endl;
}

VM::InvalidFileException::InvalidFileException( void ) { }
VM::InvalidFileException::InvalidFileException( InvalidFileException const & cp) { *this = cp; }
VM::InvalidFileException::~InvalidFileException( void ) throw() { }
VM::InvalidFileException& VM::InvalidFileException::operator=( InvalidFileException const &) { return *this; }
const char* VM::InvalidFileException::what( void ) const throw() {
	return "InvalidFileException";
}

VM::PopOnEmptyStackException::PopOnEmptyStackException( size_t line ) : _line(line) { }
VM::PopOnEmptyStackException::PopOnEmptyStackException( void ) : _line(0) { }
VM::PopOnEmptyStackException::PopOnEmptyStackException( PopOnEmptyStackException const & cp) { *this = cp; }
VM::PopOnEmptyStackException::~PopOnEmptyStackException( void ) throw() { }
VM::PopOnEmptyStackException& VM::PopOnEmptyStackException::operator=( PopOnEmptyStackException const &rhs)
{ this->_line = rhs._line; return *this; }

const char* VM::PopOnEmptyStackException::what( void ) const throw() {
	if (this->_line)
	{
		std::stringstream	o;
		o << "PopOnEmptyStackException on line " << this->_line;
		std::cout << o.str().c_str() << std::endl;	// TODO: holy tits remove this
		return o.str().c_str();
	}
	return "PopOnEmptyStackException";
}

VM::UnexpectedEOFException::UnexpectedEOFException( void ) { }
VM::UnexpectedEOFException::UnexpectedEOFException( UnexpectedEOFException const & cp) { *this = cp; }
VM::UnexpectedEOFException::~UnexpectedEOFException( void ) throw() { }
VM::UnexpectedEOFException& VM::UnexpectedEOFException::operator=( UnexpectedEOFException const &) { return *this; }
const char* VM::UnexpectedEOFException::what( void ) const throw() {
	return "UnexpectedEOFException";
}

VM::UntrueAssertionException::UntrueAssertionException( size_t line ) : _line(line) { }
VM::UntrueAssertionException::UntrueAssertionException( void ) : _line(0) { }
VM::UntrueAssertionException::UntrueAssertionException( UntrueAssertionException const & cp) { *this = cp; }
VM::UntrueAssertionException::~UntrueAssertionException( void ) throw() { }
VM::UntrueAssertionException& VM::UntrueAssertionException::operator=( UntrueAssertionException const &rhs)
{ this->_line = rhs._line; return *this; }

const char* VM::UntrueAssertionException::what( void ) const throw() {
	if (this->_line)
	{
		std::stringstream	o;
		o << "UntrueAssertionException on line " << this->_line;
		std::cout << o.str().c_str() << std::endl;	// TODO: remove
		return o.str().c_str();
	}
	return "UntrueAssertionException";
}

VM::NoExitException::NoExitException( void ) { }
VM::NoExitException::NoExitException( NoExitException const & cp) { *this = cp; }
VM::NoExitException::~NoExitException( void ) throw() { }
VM::NoExitException& VM::NoExitException::operator=( NoExitException const &) { return *this; }
const char* VM::NoExitException::what( void ) const throw() {
	return "NoExitException";
}

VM::InvalidCommandException::InvalidCommandException( size_t line ) : _line(line) { }
VM::InvalidCommandException::InvalidCommandException( void ) : _line(0) { }
VM::InvalidCommandException::InvalidCommandException( InvalidCommandException const & cp) { *this = cp; }
VM::InvalidCommandException::~InvalidCommandException( void ) throw() { }
VM::InvalidCommandException& VM::InvalidCommandException::operator=( InvalidCommandException const &rhs)
{ this->_line = rhs._line; return *this; }

const char* VM::InvalidCommandException::what( void ) const throw() {
	if (this->_line)
	{
		std::ostringstream	o;
		o << "InvalidCommandException on line " << this->_line;
		std::cout << o.str().c_str() << std::endl;	// TODO: KILLLLLLLLLLL
		std::string		s = o.str();
		const char		*c = s.c_str();
		return (c);	// TODO: make this print the good output
	}
	return "InvalidCommandException";
}
