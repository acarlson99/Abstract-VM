#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <map>

# ifndef OPTYPE
#  define OPTYPE
enum eOperandType {
	Int8,
	Int16,
	Int32,
	Float,
	Double,
};
# endif

class IOperand;

class Factory {

public:
	Factory(void);
	Factory(Factory const &cp);
	~Factory(void);
	Factory &operator=(Factory const &);

	// static IOperand const *createOperand(eOperandType type, std::string const &value, int);
	IOperand const *createOperand(eOperandType type, std::string const &value) const;

private:
	IOperand const *createInt8(std::string const &value) const;
	IOperand const *createInt16(std::string const &value) const;
	IOperand const *createInt32(std::string const &value) const;
	IOperand const *createFloat(std::string const &value) const;
	IOperand const *createDouble(std::string const &value) const;

	std::map<eOperandType, IOperand const *(Factory::*)(std::string const &)const> _f;
};

extern Factory g_factory;

#endif
