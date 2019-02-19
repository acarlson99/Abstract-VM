#ifndef OPERAND_HPP
# define OPERAND_HPP

# include <string>
# include <sstream>
# include <iomanip>
# include <float.h>
# include <limits.h>
# include "IOperand.hpp"
# include "Factory.hpp"

# ifndef MAX
#  define MAX(a, b) (a > b ? a : b)
# endif

class Factory;

// convert from string to data type
// set stringstream precision
// send int to stream
// check overflow
template <typename T>
class Operand : public IOperand
{

  public:
	Operand(std::string const &string, int precision, eOperandType type)
		: _type(type), _precision(precision), _string(NULL)
	{
		static const size_t size_arr[] = {
			4,
			6,
			11,
		};
		std::stringstream	numStr(std::stringstream::out); // TODO: use stringstream to format with precision
		std::string			tmpStr;

		try
		{
			if (this->_type <= Int32)
			{
				long long n = std::stoll(string);
				if (this->isOverflowing<long long>(n, this->_type) || string.length() > size_arr[this->_type])
					throw OverflowException();
				this->_value = static_cast<T>(n);
				numStr << std::setprecision(this->_precision) << static_cast<long long>(this->_value);
				this->_string = new std::string(numStr.str());
			}
			else
			{
				long double			ldn;

				this->_value = static_cast<T>(std::stod(string));
				std::cout << "VALUE " << this->_value << std::endl;
				std::cout << "PRECISION " << this->_precision << std::endl;
				// numStr << std::setprecision(this->_precision) << this->_value;
				numStr << this->_value;
				std::cout << "NUMSTR " << numStr.str() << std::endl;
				ldn = std::stold(string.c_str());
				if (this->isOverflowing<long double>(ldn, this->_type))
					throw OverflowException();
				this->_string = new std::string(numStr.str());
			}
		}
		catch ( std::out_of_range &e )
		{
			if (this->_string)
				delete this->_string;
			throw TooBigOWOException();
		}
		catch ( std::exception &e )
		{
			if (this->_string)
				delete this->_string;
			throw OverflowException();
		}
	}

	Operand(void) : _value(0), _type(Int8), _string(new std::string(std::to_string(this->_value))) { }

	Operand(Operand const &cp) { *this = cp; }

	~Operand(void)
		{
			// if (this->_string)
			delete this->_string;
		}

	Operand &operator=(Operand const &rhs)
		{
			this->_value = rhs._value;
			this->_type = rhs._type;
			delete this->_string;
			this->_string = new std::string(rhs._string->c_str());
			return *this;
		}

	template <typename U>
	bool						isOverflowing(U r, eOperandType type) const
		{
			switch (type)
			{
			case (Int8):
				return (r > SCHAR_MAX || r < SCHAR_MIN);
			case (Int16):
				return (r > SHRT_MAX || r < SHRT_MIN);
			case (Int32):
				return (r > INT_MAX || r < INT_MIN);
			case (Float):
				return (r > FLT_MAX || r < FLT_MIN);
			case (Double):
				return (r > DBL_MAX || r < DBL_MIN);
			}
			return (true);
		}

	virtual int					getPrecision(void) const
		{
			return (this->_precision);
		}

	virtual eOperandType		getType(void) const
		{
			return (this->_type);
		}

	virtual IOperand const *operator+(IOperand const &rhs) const
		{
			static_cast<void>(rhs);
			// eOperandType		type = MAX(this->getType(), rhs.getType());
			// std::string			newStr;
			// if (type <= Int32)
			// {
			// 	// long long		a = std::stoll(this->toString());
			// 	// long long		b = std::stoll(rhs.toString());
			// }
			// else
			// {
			// 	std::cout << "Float addition.  TODO: DEAL WITH THIS" << std::endl;
			// }
			// IOperand const	*o = g_factory.createOperand(type, newStr);
			// std::cout << o->toString() << std::endl;	// TODO: fix leaks brah
			// // const IOperand		*newOp = Factory::createOperand(type, newStr, 1);
			// // std::cout << newOp->toString() << std::endl;
			// // TODO: create new Operand<type>(string, type)
			// return (o);
			return (NULL);
		}

	virtual IOperand const *operator-(IOperand const &rhs) const
		{
			static_cast<void>(rhs);
			std::cout << "- operator called" << std::endl;
			return (NULL);
		}

	virtual IOperand const *operator*(IOperand const &rhs) const
		{
			static_cast<void>(rhs);
			std::cout << "* operator called" << std::endl;
			return (NULL);
		}

	virtual IOperand const *operator/(IOperand const &rhs) const
		{
			static_cast<void>(rhs);
			std::cout << "/ operator called" << std::endl;
			return (NULL);
		}

	virtual IOperand const *operator%(IOperand const &rhs) const
		{
			static_cast<void>(rhs);
			std::cout << "% operator called" << std::endl;
			return (NULL);
		}

	virtual std::string const		&toString(void) const
		{
			return (*this->_string);
		}

	class OverflowException : public std::exception {
	public:
		OverflowException( void ) { }
		OverflowException( OverflowException const &cp ) { *this = cp; }
		~OverflowException( void ) throw() { }
		OverflowException &operator=( OverflowException const & ) { return *this; }
		virtual const char *what() const throw() { return ("Overflow"); }
	};

	class UnderflowException : public std::exception {
	public:
		UnderflowException( void ) { }
		UnderflowException( UnderflowException const &cp ) { *this = cp; }
		~UnderflowException( void ) throw() { }
		UnderflowException &operator=( UnderflowException const & ) { return *this; }
		virtual const char *what() const throw() { return ("Underflow"); }
	};

	class DivisionByZeroException : public std::exception {
	public:
		DivisionByZeroException( void ) { }
		DivisionByZeroException( DivisionByZeroException const &cp ) { *this = cp; }
		~DivisionByZeroException( void ) throw() { }
		DivisionByZeroException &operator=( DivisionByZeroException const & ) { return *this; }
		virtual const char *what() const throw() { return ("DivisionByZero"); }
	};

	class TooBigOWOException : public std::exception {
	public:
		TooBigOWOException( void ) { }
		TooBigOWOException( TooBigOWOException const & cp) { *this = cp; }
		~TooBigOWOException( void ) throw() { }
		TooBigOWOException& operator=( TooBigOWOException const & ) { return *this; }
		virtual const char* what() const throw() { return ("OwO it's too big on"); }
	};

private:
	T						_value;
	eOperandType			_type;
	int						_precision;
	std::string				*_string;

};

#endif
