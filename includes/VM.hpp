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
	Push,
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
	Eof
};

class VM {

public:
	VM( std::string );
	VM( void );
	VM( VM const & cp);
	~VM( void );
	VM& operator=( VM const &);

	void		readLoop( void );
	eCommand	lexIn( std::string& );
	void		parseIn( eCommand, std::string& );

	class InvalidFileException : public std::exception {
	  public:
		InvalidFileException(void);
		InvalidFileException(InvalidFileException const &cp);
		~InvalidFileException(void) throw();
		InvalidFileException &operator=(InvalidFileException const &);
		virtual const char *what() const throw();
	};

private:
	std::vector<IOperand *>			_nums;
	bool							_readFromFile;
	bool							_continueReading;
	std::string						_filename;
	Factory							f;

};

#endif
