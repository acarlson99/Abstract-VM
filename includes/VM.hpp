#ifndef VM_HPP
# define VM_HPP

# include <vector>
# include <fstream>
# include "Factory.hpp"

/*
** COMMANDS
** ; - start of comment
** push v - push value to stack
** 		int8
** 		int16
** 		int32
** 		float
** 		double
** pop - unstack top value.  Errors on empty stack
** dump - display eack value of stack
** assert v - checks value against top value of stack.  Errors if they are not equal
**
** add - unstacks top two values, adds them, and stacks result
** sub - subtracts top two values
** mul - multiplies top two values
** div - divides top two values.  Errors if divisor is equal to 2
** mod - stacks modulus of top two values.  Errors if divisor is equal to 0
** 		All operators raise errors on an empty stack
** print - asserts that the top value is an int8 and prints the ASCII character it represents
** exit - terminates execution
** ;; - EOF for stdin
*/

enum eCommand {
	PushInt8,
	PushInt16,
	PushInt32,
	PushFloat,
	PushDouble,
	Pop,
	Dump,
	Assert,
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	Print,
	Exit,
	Eof,
};

class VM {

public:
	VM( std::string );
	VM( void );
	VM( VM const & cp);
	~VM( void );
	VM& operator=( VM const &);

	void		run( void );

	class InvalidFileException : public std::exception {
	public:
		InvalidFileException(void);
		InvalidFileException(InvalidFileException const &cp);
		~InvalidFileException(void) throw();
		InvalidFileException &operator=(InvalidFileException const &);
		virtual const char *what() const throw();
	};

	class PopOnEmptyStackException : public std::exception {
	public:
		PopOnEmptyStackException(void);
		PopOnEmptyStackException(PopOnEmptyStackException const &cp);
		~PopOnEmptyStackException(void) throw();
		PopOnEmptyStackException &operator=(PopOnEmptyStackException const &);
		virtual const char *what() const throw();
	};

	class UnexpectedEOFException : public std::exception {
	public:
		UnexpectedEOFException( void );
		UnexpectedEOFException( UnexpectedEOFException const & cp);
		~UnexpectedEOFException( void ) throw();
		UnexpectedEOFException& operator=( UnexpectedEOFException const & e);
		virtual const char* what() const throw();
	};

	class UntrueAssertionException : public std::exception {
	public:
		UntrueAssertionException( void );
		UntrueAssertionException( UntrueAssertionException const & cp);
		~UntrueAssertionException( void ) throw();
		UntrueAssertionException& operator=( UntrueAssertionException const & e);
		virtual const char* what() const throw();
	};

	class NoExitException : public std::exception {
	public:
		NoExitException( void );
		NoExitException( NoExitException const & cp);
		~NoExitException( void ) throw();
		NoExitException& operator=( NoExitException const & e);
		virtual const char* what() const throw();
	};

private:
	void				readLoop( void );
	void				evaluateLoop( void );
	eCommand			lexIn( std::string& );
	void				parseIn( eCommand, std::string& );
	IOperand const		*popUtil( void );

	void				push( void );
	void				pop( void );
	void				dump( void );
	void				assert( void );
	void				add( void );
	void				sub( void );
	void				mul( void );
	void				div( void );
	void				mod( void );
	void				print( void );

	std::vector<IOperand const *>		_nums;
	// std::vector<Lex const *>			_commands;
	bool								_readFromFile;
	bool								_continueReading;
	std::string							_filename;
	Factory								_f;

};

#endif
