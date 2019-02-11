#ifndef VM_HPP
# define VM_HPP

# include <stack>
# include "IOperand.hpp"

class VM {

public:
	VM( void );
	VM( VM const & cp);
	~VM( void );
	VM& operator=( VM const &);

private:
	std::stack<IOperand *>	_nums;

};

#endif
