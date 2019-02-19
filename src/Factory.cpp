#include "Factory.hpp"

Factory::Factory(void)
{
	_f[Int8] = &Factory::createInt8;
	_f[Int16] = &Factory::createInt16;
	_f[Int32] = &Factory::createInt32;
	_f[Float] = &Factory::createFloat;
	_f[Double] = &Factory::createDouble;
}

Factory::Factory(Factory const &cp) { *this = cp; }
Factory::~Factory(void) {}
Factory &Factory::operator=(Factory const &) { return *this; }

// static IOperand const *Factory::createOperand(eOperandType type, std::string const &value, int)
// {
// return ((this->*_f.at(type))(value));
// }

IOperand const *Factory::createOperand(eOperandType type, std::string const &value) const
{
/*
**	IOperand const		*op;
**
**	try {
**		op = (this->*_f.at(type))(value);
**	}
**	catch ( std::exception &e ) {
**		std::cout << e.what() << std::endl;
**		delete op;
**		std::exit(1);
**	}
**	return (op);
**
*/
	return ((this->*_f.at(type))(value));
}

IOperand const *Factory::createInt8(std::string const &value) const
{
	return (new Operand<int8_t>(value, Int8));
}

IOperand const *Factory::createInt16(std::string const &value) const
{
	return (new Operand<int16_t>(value, Int16));
}

IOperand const *Factory::createInt32(std::string const &value) const
{
	return (new Operand<int32_t>(value, Int32));
}

IOperand const *Factory::createFloat(std::string const &value) const
{
	return (new Operand<float>(value, Float));
}

IOperand const *Factory::createDouble(std::string const &value) const
{
	return (new Operand<double>(value, Double));
}
