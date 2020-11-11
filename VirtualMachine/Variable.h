#ifndef VARIABLE_H
#define VARIABLE_H
#include"ErrorPrinter.h"
#include<iostream>
#include<string>
using namespace std;
using namespace Bubblegum::Special;

namespace Bubblegum
{
namespace _VirtualMachine
{	

template<class T>
class Variable: public ErrorPrinter
{
private:
	int type;
	T value;
public:
	Variable(int type,T val)
	{	
		this->type= type;
		value = val;
	}
	~Variable()
	{
	}
	
	T get_value()const
	{
		return value;	
	}
	void set_value(T val)
	{
		value = val;
	}
};
};
};
#endif //VARIABLE_H
