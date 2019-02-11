#ifndef OPERAND_HPP
# define OPERAND_HPP

template <typename T>
class Operand: IOperand {

public:
	Operand( T value )
		{
			this->_value = value;
		}

	Operand( void )
		{
		}

	Operand( Operand const & cp) { *this = cp; }

	~Operand( void )
		{
		}

	Operand& operator=( Operand const &rhs)
		{
			this->_value = rhs._value;
			return *this;
		}

	T		getValue( void ) { return (_value); }

	virtual int						getPrecision(void) const
		{return (sizeof(_value));}
	
	virtual eOperandType			getType(void) const {}

	virtual IOperand const *		operator+( IOperand const& rhs ) const {}
	// { return (new Operand(this->_value + rhs._value)); }	// TODO: how tf do I do this without adding stuff to IOperand?

	virtual IOperand const *		operator-( IOperand const& rhs ) const {}
	virtual IOperand const *		operator*( IOperand const& rhs ) const {}
	virtual IOperand const *		operator/( IOperand const& rhs ) const {}
	virtual IOperand const *		operator%( IOperand const& rhs ) const {}

	virtual std::string const &		toString(void) const {}

private:
	T		_value;

};

#endif
