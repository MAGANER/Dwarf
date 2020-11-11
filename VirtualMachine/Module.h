#ifndef MODULE_H
#define MODULE_H
#include"Function.h"
namespace Bubblegum
{
namespace  _VirtualMachine
{
struct Module
{
	Memory memory;
	map<string,Function*> functions;
};
};
};
#endif //MODULE_H
