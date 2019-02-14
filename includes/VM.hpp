#ifndef VM_HPP
# define VM_HPP

# include <stack>
# include <queue>
# include "IOperand.hpp"

class VM {

public:
	VM( std::string );
	VM( void );
	VM( VM const & cp);
	~VM( void );
	VM& operator=( VM const &);

	void		read_loop(std::string &filename);

private:
	std::stack<IOperand *>			_nums;
	std::queue<std::string *>		_commands;
	bool							_read_from_file;
	std::string						_filename;

};

#endif
