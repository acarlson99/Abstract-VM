#ifndef VM_HPP
# define VM_HPP

# include <vector>
# include <fstream>
# include "Factory.hpp"
# include "Lexer.hpp"

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

	class InvalidCommandException : public std::exception {
	public:
		InvalidCommandException( void );
		InvalidCommandException( InvalidCommandException const & cp);
		~InvalidCommandException( void ) throw();
		InvalidCommandException& operator=( InvalidCommandException const & e);
		virtual const char* what() const throw();
	};

	void	printCommands();

private:
	void				readLoop( void );
	void				evaluateLoop( void );
	eCommand			lexIn( std::string& );
	void				parseIn( eCommand, std::string& );
	IOperand const		*popUtil( void );

	void				VMpush( void );
	void				VMpop( void );
	void				VMdump( void );
	void				VMassert( void );
	void				VMadd( void );
	void				VMsub( void );
	void				VMmul( void );
	void				VMdiv( void );
	void				VMmod( void );
	void				VMprint( void );

	std::vector<IOperand const *>		_nums;
	std::vector<Lexer const *>			_commands;
	bool								_readFromFile;
	bool								_continueReading;
	std::string							_filename;
	Factory								_f;

};

#endif
