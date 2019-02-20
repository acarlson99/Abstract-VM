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
	for (auto it : this->_nums)
		delete it;
	for (auto it : this->_commands)
		delete it;
}

VM &VM::operator=(VM const &rhs)
{
	if (this != &rhs)
	{
		for (auto it = this->_nums.begin(); it != this->_nums.end(); it++)
			delete *it;
		this->_nums.clear();
		for (auto it = rhs._nums.begin(); it != rhs._nums.end(); it++)
			this->_nums.push_back(g_factory.createOperand((*it)->getType(), (*it)->toString()));
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
	unsigned		numErrs = 0;

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
	while (numErrs < 20 && (this->_readFromFile || this->_continueReading) && std::getline(FILEIN, str))
	{
		pushed = 0;
		l = Lexer::generateTokens(str, line);
		try {
			if (l->getCommand() == Error)
			{
				this->_eval = false;
				throw InvalidCommandException();
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
			++numErrs;
			std::cout << e.what() << " line " << line << std::endl;
		}
		if (!pushed)
			delete l;
		++line;
	}
	if (numErrs)
	{
		std::cout << "\033[1;31mFATAL: " << numErrs << " errors detected\033[0m" << std::endl;
		std::exit(1);
	}
	else if (this->_continueReading)
		throw UnexpectedEOFException();
	else if (!exited)
		throw NoExitException();
}

void		VM::checkExceptions( void )
{
	if(fetestexcept(FE_DIVBYZERO))	//     printf(" FE_DIVBYZERO");
		throw DivisionByZeroException();
	// if(fetestexcept(FE_INEXACT))       printf(" FE_INEXACT");
	// if(fetestexcept(FE_INVALID))       printf(" FE_INVALID");
	if(fetestexcept(FE_OVERFLOW))	//      printf(" FE_OVERFLOW");
		throw OverflowException();
	if(fetestexcept(FE_UNDERFLOW))	//     printf(" FE_UNDERFLOW");
		throw UnderflowException();
	feclearexcept(FE_ALL_EXCEPT);
}

void		VM::evaluateLoop( void )
{
	feraiseexcept(FE_ALL_EXCEPT);
	for (auto it : this->_commands)
	{
		try {
			feclearexcept(FE_ALL_EXCEPT);
			(this->*_funcs.at(it->getCommand()))(it);
			checkExceptions();
		}
		catch ( std::exception &e ) {
			std::cout << e.what() << " line " << it->getLine() << std::endl;
			std::exit(1);
		}
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
	this->_nums.push_back(g_factory.createOperand(l->getType(), l->getArg()));
}

void		VM::VMpop( Lexer const* )
{
	delete this->popUtil();
}

void		VM::VMdump( Lexer const* )
{
	for (auto it = this->_nums.rbegin(); it != this->_nums.rend(); it++)
		std::cout << (*it)->toString() << std::endl;
}

void		VM::VMassert( Lexer const *l )
{
	if (!this->_nums.size())
		throw PopOnEmptyStackException();
	IOperand	const *op = this->_nums.back();
	IOperand	const *cmp = g_factory.createOperand(l->getType(), l->getArg());

	if (op->getType() == cmp->getType() && op->toString() == cmp->toString())
	{
		delete cmp;
		return ;
	}
	delete cmp;
	throw UntrueAssertionException();
}

void		VM::VMadd( Lexer const *l )
{
	IOperand const		*a = NULL;
	IOperand const		*b = NULL;

	try {
		a = this->popUtil();
		b = this->popUtil();

		IOperand const	*r = *a + *b;
		this->_nums.push_back(r);
		delete a;
		a = NULL;
		delete b;
		b = NULL;
	}
	catch ( std::exception &e ) {
		if (a)
			delete a;
		if (b)
			delete b;
		std::cout << e.what() << " line " << l->getLine() << std::endl;
		std::exit(1);
	}
}

void		VM::VMsub( Lexer const *l )
{
	IOperand const		*a = NULL;
	IOperand const		*b = NULL;

	try {
		b = this->popUtil();
		a = this->popUtil();

		IOperand const	*r = *a - *b;
		this->_nums.push_back(r);
		delete a;
		a = NULL;
		delete b;
		b = NULL;
	}
	catch ( std::exception &e ) {
		if (a)
			delete a;
		if (b)
			delete b;
		std::cout << e.what() << " line " << l->getLine() << std::endl;
		std::exit(1);
	}
}

void		VM::VMmul( Lexer const *l )
{
	IOperand const		*a = NULL;
	IOperand const		*b = NULL;

	try {
		b = this->popUtil();
		a = this->popUtil();

		IOperand const	*r = *a * *b;
		this->_nums.push_back(r);
		delete a;
		a = NULL;
		delete b;
		b = NULL;
	}
	catch ( std::exception &e ) {
		if (a)
			delete a;
		if (b)
			delete b;
		std::cout << e.what() << " line " << l->getLine() << std::endl;
		std::exit(1);
	}
}

void		VM::VMdiv( Lexer const *l )
{
	IOperand const		*a = NULL;
	IOperand const		*b = NULL;

	try {
		b = this->popUtil();
		a = this->popUtil();

		IOperand const	*r = *a / *b;
		this->_nums.push_back(r);
		delete a;
		a = NULL;
		delete b;
		b = NULL;
	}
	catch ( std::exception &e ) {
		if (a)
			delete a;
		if (b)
			delete b;
		std::cout << e.what() << " line " << l->getLine() << std::endl;
		std::exit(1);
	}
}

void		VM::VMmod( Lexer const *l )
{
	IOperand const		*a = NULL;
	IOperand const		*b = NULL;

	try {
		b = this->popUtil();
		a = this->popUtil();

		IOperand const	*r = *a % *b;
		this->_nums.push_back(r);
		delete a;
		a = NULL;
		delete b;
		b = NULL;
	}
	catch ( std::exception &e ) {
		if (a)
			delete a;
		if (b)
			delete b;
		std::cout << e.what() << " line " << l->getLine() << std::endl;
		std::exit(1);
	}
}

void		VM::VMprint( Lexer const* )
{
	if (!this->_nums.size())
		throw PopOnEmptyStackException();
	IOperand	const *op = this->_nums.back();
	if (op->getType() != Int8)
		throw UntrueAssertionException();
	std::cout << static_cast<char>(std::stoi(op->toString())) << std::endl;
}

void		VM::VMexit( Lexer const* )
{
	std::exit(0);
}

void		VM::VMcatch( Lexer const *l)
{
	std::cout << "VM::VMcatch called on " << *l << " line " << l->getLine() << " somehow" << std::endl;
}

VM::InvalidFileException::InvalidFileException( void ) { }
VM::InvalidFileException::InvalidFileException( InvalidFileException const & cp) { *this = cp; }
VM::InvalidFileException::~InvalidFileException( void ) throw() { }
VM::InvalidFileException& VM::InvalidFileException::operator=( InvalidFileException const &) { return *this; }
const char* VM::InvalidFileException::what( void ) const throw() {
	return "InvalidFile";
}

VM::PopOnEmptyStackException::PopOnEmptyStackException( void ) { }
VM::PopOnEmptyStackException::PopOnEmptyStackException( PopOnEmptyStackException const & cp) { *this = cp; }
VM::PopOnEmptyStackException::~PopOnEmptyStackException( void ) throw() { }
VM::PopOnEmptyStackException& VM::PopOnEmptyStackException::operator=( PopOnEmptyStackException const& ) { return *this; }
const char* VM::PopOnEmptyStackException::what( void ) const throw() {
	return "PopOnEmptyStack";
}

VM::UnexpectedEOFException::UnexpectedEOFException( void ) { }
VM::UnexpectedEOFException::UnexpectedEOFException( UnexpectedEOFException const & cp) { *this = cp; }
VM::UnexpectedEOFException::~UnexpectedEOFException( void ) throw() { }
VM::UnexpectedEOFException& VM::UnexpectedEOFException::operator=( UnexpectedEOFException const &) { return *this; }
const char* VM::UnexpectedEOFException::what( void ) const throw() {
	return "UnexpectedEOF";
}

VM::UntrueAssertionException::UntrueAssertionException( void ) { }
VM::UntrueAssertionException::UntrueAssertionException( UntrueAssertionException const & cp) { *this = cp; }
VM::UntrueAssertionException::~UntrueAssertionException( void ) throw() { }
VM::UntrueAssertionException& VM::UntrueAssertionException::operator=( UntrueAssertionException const& ) { return *this; }
const char* VM::UntrueAssertionException::what( void ) const throw() {
	return "UntrueAssertion";
}

VM::NoExitException::NoExitException( void ) { }
VM::NoExitException::NoExitException( NoExitException const & cp) { *this = cp; }
VM::NoExitException::~NoExitException( void ) throw() { }
VM::NoExitException& VM::NoExitException::operator=( NoExitException const &) { return *this; }
const char* VM::NoExitException::what( void ) const throw() {
	return "NoExit";
}

VM::InvalidCommandException::InvalidCommandException( void ) { }
VM::InvalidCommandException::InvalidCommandException( InvalidCommandException const & cp) { *this = cp; }
VM::InvalidCommandException::~InvalidCommandException( void ) throw() { }
VM::InvalidCommandException& VM::InvalidCommandException::operator=( InvalidCommandException const& ) { return *this; }
const char* VM::InvalidCommandException::what( void ) const throw() {
	return "InvalidCommand";
}

VM::OverflowException::OverflowException( void ) { }
VM::OverflowException::OverflowException( OverflowException const &cp ) { *this = cp; }
VM::OverflowException::~OverflowException( void ) throw() { }
VM::OverflowException& VM::OverflowException::operator=( OverflowException const& ) { return *this; }
const char	*VM::OverflowException::what( void ) const throw() {
	return "OverflowException";
}

VM::UnderflowException::UnderflowException( void ) { }
VM::UnderflowException::UnderflowException( UnderflowException const &cp ) { *this = cp; }
VM::UnderflowException::~UnderflowException( void ) throw() { }
VM::UnderflowException& VM::UnderflowException::operator=( UnderflowException const& ) { return *this; }
const char	*VM::UnderflowException::what( void ) const throw() {
	return "UnderflowException";
}

VM::DivisionByZeroException::DivisionByZeroException( void ) { }
VM::DivisionByZeroException::DivisionByZeroException( DivisionByZeroException const &cp ) { *this = cp; }
VM::DivisionByZeroException::~DivisionByZeroException( void ) throw() { }
VM::DivisionByZeroException& VM::DivisionByZeroException::operator=( DivisionByZeroException const& ) { return *this; }
const char	*VM::DivisionByZeroException::what( void ) const throw() {
	return "DivisionByZeroException";
}

VM::TooBigOWOException::TooBigOWOException( void ) { }
VM::TooBigOWOException::TooBigOWOException( TooBigOWOException const &cp ) { *this = cp; }
VM::TooBigOWOException::~TooBigOWOException( void ) throw() { }
VM::TooBigOWOException& VM::TooBigOWOException::operator=( TooBigOWOException const& ) { return *this; }
const char	*VM::TooBigOWOException::what( void ) const throw() {
	return "OwO it's too bit on";
}
