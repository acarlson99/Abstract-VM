#ifndef OPERAND_HPP
# define OPERAND_HPP

# include <string>
# include <sstream>
# include <iomanip>
# include <float.h>
# include <limits.h>
# include <fenv.h>
# include "IOperand.hpp"
# include "Factory.hpp"

# ifndef MAX
#  define MAX(a, b) (a > b ? a : b)
# endif

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

			try
			{
				if (this->_type <= Int32)
				{
					long long n;
					n = std::stoll(string);
					if (this->isOverflowing(n, this->_type) || string.length() > size_arr[this->_type])
						throw OverflowException();
					this->_value = static_cast<T>(n);
					this->_string = new std::string(std::to_string(this->_value));
				}
				else
				{
					long double			n;

					n = std::stold(string);
					this->_value = static_cast<T>(n);
					this->_string = new std::string(std::to_string(this->_value));
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
			delete this->_string;
		}

	Operand &operator=(Operand const &rhs)
		{
			this->_value = rhs._value;
			this->_type = rhs._type;
			delete this->_string;
			this->_string = new std::string(rhs._string->c_str());
			this->_precision = rhs._precision;
			return *this;
		}

	bool						isOverflowing(long long r, eOperandType type) const
		{
			switch (type)
			{
			case (Int8):
				return (r > SCHAR_MAX || r < SCHAR_MIN);
			case (Int16):
				return (r > SHRT_MAX || r < SHRT_MIN);
			case (Int32):
				return (r > INT_MAX || r < INT_MIN);
			case (Float):;
			case (Double):;
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
			eOperandType            type = MAX(this->getType(), rhs.getType());
			if (type <= Int32)
			{
				return (g_factory.createOperand(type, std::string(std::to_string(std::stoll(this->toString()) + std::stoll(rhs.toString())))));
			}
			else
			{
				return (g_factory.createOperand(type, std::string(std::to_string(std::stold(this->toString()) + std::stold(rhs.toString())))));
			}
		}

	virtual IOperand const *operator-(IOperand const &rhs) const
		{
			eOperandType            type = MAX(this->getType(), rhs.getType());
			if (type <= Int32)
			{
				return (g_factory.createOperand(type, std::string(std::to_string(std::stoll(this->toString()) - std::stoll(rhs.toString())))));
			}
			else
			{
				return (g_factory.createOperand(type, std::string(std::to_string(std::stold(this->toString()) - std::stold(rhs.toString())))));
			}
			return (NULL);
		}

	virtual IOperand const *operator*(IOperand const &rhs) const
		{
			eOperandType            type = MAX(this->getType(), rhs.getType());
			if (type <= Int32)
			{
				return (g_factory.createOperand(type, std::string(std::to_string(std::stoll(this->toString()) * std::stoll(rhs.toString())))));
			}
			else
			{
				return (g_factory.createOperand(type, std::string(std::to_string(std::stold(this->toString()) * std::stold(rhs.toString())))));
			}
			return (NULL);
		}

	virtual IOperand const *operator/(IOperand const &rhs) const
		{
			eOperandType            type = MAX(this->getType(), rhs.getType());
			if (type <= Int32)
			{
				long long a = std::stoll(this->toString());
				long long b = std::stoll(rhs.toString());
				if (b == 0)
					throw DivisionByZeroException();
				std::cout << "A = " << a << std::endl << "B = " << b << std::endl;
				return (g_factory.createOperand(type, std::string(std::to_string(a / b))));
			}
			else
			{
				long double a = std::stold(this->toString());
				long double b = std::stold(rhs.toString());
				if (b == 0.0)
					throw DivisionByZeroException();
				std::cout << "A = " << a << std::endl << "B = " << b << std::endl;
				return (g_factory.createOperand(type, std::string(std::to_string(a / b))));
			}
			return (NULL);
		}

	virtual IOperand const *operator%(IOperand const &rhs) const
		{
			static_cast<void>(rhs);
			eOperandType            type = MAX(this->getType(), rhs.getType());
			if (type <= Int32)	// %
			{
				long long a = std::stoll(this->toString());
				long long b = std::stoll(rhs.toString());
				if (b == 0)
					throw DivisionByZeroException();
				std::cout << "A = " << a << std::endl << "B = " << b << std::endl;
				return (g_factory.createOperand(type, std::string(std::to_string(a % b))));
			}
			else	// fmod
			{
				long double a = std::stold(this->toString());
				long double b = std::stold(rhs.toString());
				if (b == 0.0)
					throw DivisionByZeroException();
				std::cout << "A = " << a << std::endl << "B = " << b << std::endl;
				return (g_factory.createOperand(type, std::string(std::to_string(fmod(a, b)))));
			}
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
