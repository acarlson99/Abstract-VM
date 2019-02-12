#ifndef OPERAND_HPP
# define OPERAND_HPP

# include <iostream>
# include <string>

template <typename T>
class Operand: public IOperand {

public:
	Operand( T value, eOperandType type )
		: _value(value), _type(type), _string(new std::string(std::to_string(this->_value)))
		{
		}

	Operand( T value )
		{
			this->_value = value;
		}

	Operand( void )
		{
			this->_value = 0;
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
		{
			return (sizeof(_value));
		}
	
	virtual eOperandType			getType(void) const
		{
			return (this->_type);
		}

	virtual IOperand const *		operator+( IOperand const& rhs ) const {
		static_cast<void>(rhs);
		return (NULL); }
	// { return (new Operand(this->_value + rhs._value)); }	// TODO: how tf do I do this without adding stuff to IOperand?

	virtual IOperand const *		operator-( IOperand const& rhs ) const
		{
			static_cast<void>(rhs);
			return (NULL);
		}

	virtual IOperand const *		operator*( IOperand const& rhs ) const
		{
			static_cast<void>(rhs);
			return (NULL);
		}

	virtual IOperand const *		operator/( IOperand const& rhs ) const
		{
			static_cast<void>(rhs);
			return (NULL);
		}

	virtual IOperand const *		operator%( IOperand const& rhs ) const
		{
			static_cast<void>(rhs);
			return (NULL);
		}

	virtual std::string const &		toString(void) const
		{
			return (*this->_string);
		}

private:
	T					_value;
	eOperandType		_type;
	std::string*		_string;

};

#endif
